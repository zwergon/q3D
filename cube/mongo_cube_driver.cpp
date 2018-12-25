#include "mongo_cube_driver.h"

#include <QDebug>
#include <QUuid>
#include <QSharedMemory>

#include <q3D/cube/cube_model.h>
#include <q3D/cube/cube_renderer.h>

namespace Q3D {

//QString MongoCubeOpenInfo::URI("mongodb://IRPCF328421/");
QString MongoCubeOpenInfo::URI("mongodb://localhost/");
QString MongoCubeOpenInfo::DATABASE("tim8");

MongoCubeOpenInfo::MongoCubeOpenInfo(
        const QString& uri,
        const QString& database)
    : uri_(uri),
      database_(database),
      experience_("cell1"),
      numero_(24),
      serie_(0)
{
}

QString MongoCubeOpenInfo::getName() const {
    return QString("%1_%2_%3").arg(experience_).arg(numero_).arg(serie_);
}

const QString& MongoCubeOpenInfo::getURI() const{
    return uri_;
}

void MongoCubeOpenInfo::setURI(const QString &uri){
    uri_ = uri;
}

const QString& MongoCubeOpenInfo::getDatabase() const {
    return database_;
}

void MongoCubeOpenInfo::setDatabase(const QString &database){
    database_ = database;
}

const QString& MongoCubeOpenInfo::getExperience() const{
    return experience_;
}

void MongoCubeOpenInfo::setExperience(const QString &experience){
    experience_ = experience;
}

int MongoCubeOpenInfo::getNumero() const {
    return numero_;
}

void MongoCubeOpenInfo::setNumero(int numero){
    numero_ = numero;
}

int MongoCubeOpenInfo::getSerie() const {
    return serie_;
}

void MongoCubeOpenInfo::setSerie(int serie){
    serie_ = serie;
}

mongoc_client_t* MongoCubeOpenInfo::createClient() const {
    bson_error_t error;

    mongoc_uri_t* uri = mongoc_uri_new_with_error ( uri_.toUtf8(), &error);
    if (uri == nullptr) {
        qDebug() << "failed to parse URI " << uri_
                 << "\n error message " << error.message;
        return nullptr;
    }

    mongoc_client_t* client =  mongoc_client_new_from_uri (uri);
    if ( client == nullptr ){
        return nullptr;
    }

    mongoc_client_set_error_api (client, 2);
    mongoc_uri_destroy (uri);

    return client;

}

/*************************************************/

MongoCubeDriver::MongoCubeDriver()
{
    setDescription("MongoCubeDriver");
    renderer_factory_.registerFactory<CubeRenderer>( "Cube" );
}

void MongoCubeDriver::getCubeInfo(
        mongoc_client_t* client,
        const MongoCubeOpenInfo& moi,
        bson_oid_t& cube_id,
        Cube& cube)
{

    bson_t* query = BCON_NEW(
                "experience", BCON_UTF8(moi.getExperience().toUtf8()),
                "numero", BCON_INT32(moi.getNumero()),
                "serie", BCON_INT32(moi.getSerie()));

    mongoc_collection_t *collection =
            mongoc_client_get_collection (client, moi.getDatabase().toUtf8(), "headers");
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (
                collection,
                query,
                nullptr,  /* additional options */
                nullptr); /* read prefs, NULL for default */
    bson_destroy (query);

    const bson_t *doc;
    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_t iter;
        if ( bson_iter_init (&iter, doc)){
            while (bson_iter_next (&iter)){
                if (BSON_ITER_IS_KEY(&iter, "cube_id") &&
                    BSON_ITER_HOLDS_OID (&iter)) {
                    bson_oid_copy( bson_iter_oid (&iter), &cube_id );
                }
                else {

                    if (BSON_ITER_IS_KEY(&iter, "cube_size") &&
                        BSON_ITER_HOLDS_ARRAY(&iter)){
                        const uint8_t * data = nullptr;
                        uint32_t len = 0;
                        bson_iter_array(&iter, &len, &data);

                        bson_iter_t array_itr;
                        bson_t* bson_array = bson_new_from_data(data, len);
                        bson_iter_init(&array_itr, bson_array);
                        int dim[3];
                        for(auto& v : dim){
                            bson_iter_next(&array_itr);
                            v = bson_iter_int32(&array_itr);
                        }
                        cube.setSize(dim[0], dim[1], dim[2]);
                    }

                }
            }
        }
    }

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);

}

void MongoCubeDriver::loadCube(
        mongoc_client_t* client,
        const MongoCubeOpenInfo& moi,
        const bson_oid_t& cube_id,
        Cube& cube){

    bson_error_t error;
    mongoc_gridfs_t* gridfs =
            mongoc_client_get_gridfs (client, moi.getDatabase().toUtf8(), "cubes", &error);
    Q_ASSERT(  nullptr != gridfs );

    bson_t* query = BCON_NEW("_id", BCON_OID(&cube_id));
    mongoc_gridfs_file_t* file = mongoc_gridfs_find_one_with_opts(gridfs, query, nullptr, &error);

    mongoc_stream_t* stream = mongoc_stream_gridfs_new(file);

    QSharedMemory sharedMemory(QUuid::createUuid().toString());
    sharedMemory.create(cube.size());

    sharedMemory.lock();
    quint8* buffer = static_cast<quint8*>(sharedMemory.data());
    mongoc_stream_read(stream, buffer, cube.size(), cube.size(), -1 );
    sharedMemory.unlock();

    cube.attach(sharedMemory);

    mongoc_stream_destroy (stream);
    mongoc_gridfs_file_destroy (file);
    mongoc_gridfs_destroy (gridfs);
}

Model* MongoCubeDriver::open(const ModelOpenInfo &openInfo ){

    CubeModel* cube_model = nullptr;
    try {
        const MongoCubeOpenInfo& moi = dynamic_cast<const MongoCubeOpenInfo&>(openInfo);

        mongoc_client_t* client = moi.createClient();
        if (client == nullptr) {
            return nullptr;
        };

        cube_model = new CubeModel();
        cube_model->setObjectName(moi.getName());
        cube_model->setDriver(this);

        Cube& cube = cube_model->cube();

        bson_oid_t cube_id;
        getCubeInfo(client, moi, cube_id, cube);
        char oidstr[25];
        bson_oid_to_string (&cube_id, oidstr);
        qDebug() << "cube_id: " << oidstr << " dims: "
                 << cube.getNx()  << ", " << cube.getNy() << ", " << cube.getNz();

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        loadCube(client, moi, cube_id, cube);
        QApplication::restoreOverrideCursor();

        cube_model->update();


        mongoc_client_destroy (client);

    }
    catch(bad_cast){
        return nullptr;
    }

    return cube_model;
}

void MongoCubeDriver::save(const Model &, const QString &)
{
}

}
#include "mongo_cube_driver.h"

#include <QDebug>
#include <QUuid>
#include <QSettings>

#include <q3D/cube/cube.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/drivers/cube/cube_renderer.h>

namespace Q3D {

MongoCubeOpenInfo::MongoCubeOpenInfo()
    : uri_(QString::null),
      database_(QString::null),
      headers_("Headers")
{}

const QString& MongoCubeOpenInfo::getURI() const{
    return uri_;
}

void MongoCubeOpenInfo::setURI(const QString &uri){
    uri_ = uri;
}

const QString& MongoCubeOpenInfo::getHeaders() const {
    return headers_;
}

const QString& MongoCubeOpenInfo::getDatabase() const {
    return database_;
}

void MongoCubeOpenInfo::setDatabase(const QString &database){
    database_ = database;
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

MongoIdOpenInfo::MongoIdOpenInfo()
    :  MongoCubeOpenInfo(),
      id_("")
{
    QSettings settings( "ifp", "q3D" );
    uri_ = settings.value( "mongodb/uri", "mongodb://localhost" ).toString();
    database_ = settings.value("mongodb/database", "tim8").toString();
}

void MongoIdOpenInfo::setId(const QString& id){
    id_ = id;
}

const QString& MongoIdOpenInfo::getId() const {
    return id_;
}

QString MongoIdOpenInfo::getName() const {
    return id_;
}


bson_t* MongoIdOpenInfo::getQuery() const {
    bson_oid_t oid;
    bson_oid_init(&oid, NULL);
    bson_oid_init_from_string(&oid, id_.toUtf8().constData());
    return BCON_NEW("_id", BCON_OID(&oid));
}




/*************************************************/

bool MongoCubeDriver::canHandle(Model* model) const {
    return dynamic_cast<CubeModel*>(model) != nullptr;
}


void MongoCubeDriver::readCubeDescription(
        mongoc_cursor_t *cursor,
        const char*& cube_type,
        bson_oid_t& cube_id,
        int* cube_dim) const
{
    const bson_t *doc;
    mongoc_cursor_t* local_cur = mongoc_cursor_clone(cursor);
    while (mongoc_cursor_next (local_cur, &doc)) {
        bson_iter_t iter;
        if ( bson_iter_init (&iter, doc)){
            while (bson_iter_next (&iter)){
                if (BSON_ITER_IS_KEY(&iter, "cube_id") &&
                    BSON_ITER_HOLDS_OID (&iter)) {
                    bson_oid_copy( bson_iter_oid (&iter), &cube_id );
                }
                else {

                    if (BSON_ITER_IS_KEY(&iter, "cube_type") &&
                        BSON_ITER_HOLDS_UTF8 (&iter)) {
                        uint32_t length = 0;
                        cube_type = bson_iter_utf8(&iter, &length);

                    }
                    else if (BSON_ITER_IS_KEY(&iter, "cube_dims") &&
                        BSON_ITER_HOLDS_ARRAY(&iter)){
                        const uint8_t * data = nullptr;
                        uint32_t len = 0;
                        bson_iter_array(&iter, &len, &data);

                        bson_iter_t array_itr;
                        bson_t* bson_array = bson_new_from_data(data, len);
                        bson_iter_init(&array_itr, bson_array);

                        for(int i=0; i<3; i++){
                            bson_iter_next(&array_itr);
                            cube_dim[i] = bson_iter_int32(&array_itr);
                        }

                    }

                }
            }
        }
    }
}

Cube* MongoCubeDriver::createCube(
        mongoc_client_t* client,
        const MongoCubeOpenInfo& moi,
        bson_oid_t& cube_id) const
{

    bson_t* query = moi.getQuery();
    mongoc_collection_t *collection =
            mongoc_client_get_collection (client, moi.getDatabase().toUtf8(), moi.getHeaders().toUtf8());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (
                collection,
                query,
                nullptr,  /* additional options */
                nullptr); /* read prefs, NULL for default */
    bson_destroy (query);


    float orig[3] = {0., 0.,  0.};
    float pixdim[3] = {1., 1., 1.};
    if (!readAffine(cursor, orig, pixdim)){
        mongoc_cursor_destroy (cursor);
        return nullptr;
    }

    const char* cube_type;
    int   cube_dim[3];
    readCubeDescription(cursor, cube_type, cube_id, cube_dim);


    Cube* cube = nullptr;
    if ( strcmp(cube_type, "uint8") == 0){
        cube = new CubeT<uint8_t>();
    }
    else if ( strcmp(cube_type, "float32") == 0 ){
        cube = new CubeT<float>();
    }
    else if ( strcmp(cube_type, "float64") == 0 ){
        cube = new CubeT<double>();
    }

    if ( cube == nullptr ){
        qDebug() << "unable to allocate cube";
        return nullptr;
    }

    cube->allocate(cube_dim[0], cube_dim[1], cube_dim[2]);
    cube->setPixelSize(pixdim[0], pixdim[1], pixdim[2]);
    cube->setOrigin(orig[0], orig[1], orig[2]);

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);

    return cube;

}

void MongoCubeDriver::loadCube(
        mongoc_client_t* client,
        const MongoCubeOpenInfo& moi,
        const bson_oid_t& cube_id,
        Cube& cube) const
{

    bson_error_t error;
    mongoc_gridfs_t* gridfs =
            mongoc_client_get_gridfs (client, moi.getDatabase().toUtf8(), "cubes", &error);
    Q_ASSERT(  nullptr != gridfs );

    bson_t* query = BCON_NEW("_id", BCON_OID(&cube_id));
    mongoc_gridfs_file_t* file = mongoc_gridfs_find_one_with_opts(gridfs, query, nullptr, &error);

    mongoc_stream_t* stream = mongoc_stream_gridfs_new(file);


    long byte_size = cube.byteSize();

    mongoc_stream_read(stream, cube.data(), byte_size, byte_size, -1 );


    mongoc_stream_destroy (stream);
    mongoc_gridfs_file_destroy (file);
    mongoc_gridfs_destroy (gridfs);
}




Model* MongoCubeDriver::open(const ModelOpenInfo &openInfo ){

    if (!isValid(&openInfo)){
        return nullptr;
    }

     CubeModel* cube_model = nullptr;
    try {
        const MongoCubeOpenInfo& moi = dynamic_cast<const MongoCubeOpenInfo&>(openInfo);

        mongoc_client_t* client = moi.createClient();
        if (client == nullptr) {
            mongoc_client_destroy (client);
            return nullptr;
        };

        bson_oid_t cube_id;
        Cube* cube = createCube(client, moi, cube_id);
        if ( cube == nullptr ){
            mongoc_client_destroy (client);
            return nullptr;
        }


        cube_model = new CubeModel();
        cube_model->setObjectName(moi.getName());
        cube_model->setDriver(this);
        cube_model->setCube(cube);

        char oidstr[25];
        bson_oid_to_string (&cube_id, oidstr);
        qDebug() << "cube_id: " << oidstr << " dims: "
                 << cube->nx()  << ", " << cube->ny() << ", " << cube->nz();



        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        loadCube(client, moi, cube_id, *cube);
        QApplication::restoreOverrideCursor();

        cube_model->update();


        mongoc_client_destroy (client);

    }
    catch(bad_cast){
        return nullptr;
    }

    return cube_model;
}

void MongoCubeDriver::save(const Model &, const ModelOpenInfo& ){
    qDebug() << "Save for " << description() << " is not yet implemented";

}

/*************************************************/


MongoFoamDriver::MongoFoamDriver()
{
    setDescription("MongoFoamDriver");
    renderer_factory_.registerFactory<CubeRenderer>( "Cube" );
}

ModelOpenInfo* MongoFoamDriver::openInfo() const {
    return new MongoIdOpenInfo();
}

bool MongoFoamDriver::isValid( const ModelOpenInfo* moi ) const {
    return dynamic_cast<const MongoIdOpenInfo*>(moi) != nullptr;
}

bool MongoFoamDriver::readAffine(
        mongoc_cursor_t *cursor,
        float* orig,
        float* pixdim) const {
    return true;
}


}

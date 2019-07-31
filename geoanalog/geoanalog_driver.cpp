#include "geoanalog_driver.h"

#include <QSettings>

#include <q3D/drivers/cube/cube_renderer.h>

/*************************************************/

QString GeoAnalogOpenInfo::exam_collect_ = "Exam";

GeoAnalogOpenInfo::GeoAnalogOpenInfo()
    :  Q3D::MongoCubeOpenInfo(),
      study_("03397"),
      fov_(420.),
      index_(46)
{
    QSettings settings( "ifp", "q3D" );
    uri_ = settings.value( "geoanalog/uri", "mongodb://localhost" ).toString();
    database_ = settings.value("geonalog/database", "geoanalog").toString();
    headers_ = "Headers";
}

const QString& GeoAnalogOpenInfo::getExams() const {
    return exam_collect_;
}

QString GeoAnalogOpenInfo::getName() const {
    return QString("geoanalog_%1_%2_%3").arg(study_).arg(fov_).arg(index_);
}

const QString& GeoAnalogOpenInfo::getStudy() const{
    return study_;
}

void GeoAnalogOpenInfo::setStudy(const QString &study){
    study_ = study;
}

float GeoAnalogOpenInfo::getFov() const {
    return fov_;
}

void GeoAnalogOpenInfo::setFov(float fov){
    fov_ = fov;
}

int GeoAnalogOpenInfo::getIndex() const {
    return index_;
}

void GeoAnalogOpenInfo::setIndex(int index){
    index_ = index;
}

bson_t* GeoAnalogOpenInfo::getQuery() const {
    return BCON_NEW(
                    "Exam number", BCON_UTF8(study_.toUtf8()),
                    "fov", BCON_DOUBLE(fov_),
                    "index", BCON_INT32(index_)
                );
}


/*************************************************/


GeoanalogDriver::GeoanalogDriver()
{
    setDescription("GeoAnalogDriver");
    renderer_factory_.registerFactory<Q3D::CubeRenderer>( "Cube" );
}

Q3D::ModelOpenInfo* GeoanalogDriver::openInfo() const {
    return new GeoAnalogOpenInfo();
}

bool GeoanalogDriver::isValid( const Q3D::ModelOpenInfo* moi ) const {
    return dynamic_cast<const GeoAnalogOpenInfo*>(moi) != nullptr;
}

bool GeoanalogDriver::readAffine(
        mongoc_cursor_t *cursor,
        float* orig,
        float* pixdim) const
{
    bool valid = false;
    const bson_t *doc;

    mongoc_cursor_t* local_cur = mongoc_cursor_clone(cursor);
    while (mongoc_cursor_next (local_cur, &doc)) {
        bson_iter_t iter;
        if ( bson_iter_init (&iter, doc)){
            while (bson_iter_next (&iter)){
                if (BSON_ITER_IS_KEY(&iter, "delta_y") &&
                    BSON_ITER_HOLDS_DOUBLE (&iter)) {
                    pixdim[1] = bson_iter_double(&iter);
                    valid = true;
                }
                else if (BSON_ITER_IS_KEY(&iter, "orig") &&
                         BSON_ITER_HOLDS_ARRAY(&iter)){
                    const uint8_t * data = nullptr;
                    uint32_t len = 0;
                    bson_iter_array(&iter, &len, &data);

                    bson_iter_t array_itr;
                    bson_t* bson_array = bson_new_from_data(data, len);
                    bson_iter_init(&array_itr, bson_array);

                    for(int i=0; i<3; i++){
                        bson_iter_next(&array_itr);
                        orig[i] = bson_iter_double(&array_itr);
                    }
                }
                else if (BSON_ITER_IS_KEY(&iter, "xz_dim") &&
                         BSON_ITER_HOLDS_ARRAY(&iter)){
                    const uint8_t * data = nullptr;
                    uint32_t len = 0;
                    bson_iter_array(&iter, &len, &data);
                    bson_iter_t array_itr;
                    bson_t* bson_array = bson_new_from_data(data, len);
                    bson_iter_init(&array_itr, bson_array);

                    bson_iter_next(&array_itr);
                    pixdim[0] = bson_iter_double(&array_itr);

                    bson_iter_next(&array_itr);
                    pixdim[2] = bson_iter_double(&array_itr);

                }
            }
        }
    }

    return valid;
}

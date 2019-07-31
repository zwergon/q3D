#ifndef GEOANALOG_DRIVER_H
#define GEOANALOG_DRIVER_H

#include <q3D/geoanalog/geoanalog_global.h>

#include <q3D/drivers/cube/mongo_cube_driver.h>


/*************************************************/
class GEOANALOGSHARED_EXPORT GeoAnalogOpenInfo : public Q3D::MongoCubeOpenInfo {
public:
    GeoAnalogOpenInfo();

    const QString& getStudy() const;
    void setStudy(const QString& study);

    float getFov() const;
    void setFov(float fov);

    int getIndex() const;
    void setIndex( int index );

    const QString& getExams() const;

    virtual QString getName() const override;
    virtual bson_t* getQuery() const override;

private:
    static QString exam_collect_;

private:
    QString study_;
    float fov_;
    int index_;
};



class GEOANALOGSHARED_EXPORT GeoanalogDriver  : public Q3D::MongoCubeDriver
{
public:
    GeoanalogDriver();
    virtual Q3D::ModelOpenInfo* openInfo() const override;
    virtual bool isValid( const Q3D::ModelOpenInfo* ) const override;


protected:
    virtual bool readAffine(
            mongoc_cursor_t *cursor,
            float* orig,
            float* pixdim) const override;

};


#endif // GEOANALOG_DRIVER_H

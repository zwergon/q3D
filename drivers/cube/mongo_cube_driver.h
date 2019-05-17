#ifndef MONGO_CUBE_DRIVER_H
#define MONGO_CUBE_DRIVER_H

#include <q3D/drivers/drivers_global.h>

#include <mongoc.h>

#include <q3D/model/model_open_info.h>
#include <q3D/model/model_driver.h>
#include <q3D/cube/cube.h>

namespace Q3D {


/*************************************************/

class DRIVERSSHARED_EXPORT MongoCubeOpenInfo : public ModelOpenInfo {
public:
    MongoCubeOpenInfo();

    const QString& getURI() const;
    void setURI( const QString& uri );

    const QString& getDatabase() const;
    void setDatabase(const QString& database);

    virtual QString getName() const = 0;
    virtual bson_t* getQuery() const = 0;

    mongoc_client_t* createClient() const;

protected:
    QString uri_;
    QString database_;
};


/*************************************************/
class DRIVERSSHARED_EXPORT MongoFoamOpenInfo : public MongoCubeOpenInfo {
public:
    MongoFoamOpenInfo();

    const QString& getExperience() const;
    void setExperience(const QString& experience);

    int getNumero() const;
    void setNumero(int numero);

    int getSerie() const;
    void setSerie( int serie );

    virtual QString getName() const override;
    virtual bson_t* getQuery() const override;

private:
    QString experience_;
    int numero_;
    int serie_;
};



/*************************************************/
class DRIVERSSHARED_EXPORT GeoAnalogOpenInfo : public MongoCubeOpenInfo {
public:
    GeoAnalogOpenInfo();

    const QString& getStudy() const;
    void setStudy(const QString& study);

    float getFov() const;
    void setFov(float fov);

    int getIndex() const;
    void setIndex( int index );

    virtual QString getName() const override;
    virtual bson_t* getQuery() const override;

private:
    QString study_;
    float fov_;
    int index_;
};

/*************************************************/
class DRIVERSSHARED_EXPORT MongoCubeDriver  : public ModelDriver
{
public:
    virtual ModelOpenInfo* openInfo() const = 0;
    virtual bool canHandle(Model *) const override;
    virtual Model* open( const ModelOpenInfo& ) override;
    virtual void save( const Model& model, const ModelOpenInfo&  ) override;


protected:
    Cube* createCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            bson_oid_t& cube_id);
    void loadCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            const bson_oid_t& cube_id,
            Cube& cube);
};

class DRIVERSSHARED_EXPORT MongoFoamDriver  : public MongoCubeDriver
{
public:
    MongoFoamDriver();
    virtual ModelOpenInfo* openInfo() const override;
};

class DRIVERSSHARED_EXPORT GeoAnalogDriver  : public MongoCubeDriver
{
public:
    GeoAnalogDriver();
    virtual ModelOpenInfo* openInfo() const override;
};

}

#endif // MONGO_CUBE_DRIVER_H

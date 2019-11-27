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

    const QString& getHeaders() const;

    const QString& getDatabase() const;
    void setDatabase(const QString& database);

    virtual QString getName() const = 0;
    virtual bson_t* getQuery() const = 0;

    mongoc_client_t* createClient() const;

protected:
    QString uri_;
    QString database_;
    QString headers_;
};


/*************************************************/
class DRIVERSSHARED_EXPORT MongoIdOpenInfo : public MongoCubeOpenInfo {
public:
    MongoIdOpenInfo();

    const QString& getId() const;
    void setId(const QString& experience);

    virtual QString getName() const override;
    virtual bson_t* getQuery() const override;

private:
    QString id_;
};



/*************************************************/
class DRIVERSSHARED_EXPORT MongoCubeDriver  : public ModelDriver
{
public:
    virtual bool isValid( const ModelOpenInfo* ) const = 0;
    virtual ModelOpenInfo* openInfo() const = 0;
    virtual bool canHandle(Model *) const override;
    virtual Model* open( const ModelOpenInfo& ) override;
    virtual void save( const Model& model, const ModelOpenInfo&  ) override;


protected:
    Cube* createCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            bson_oid_t& cube_id) const;
    void loadCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            const bson_oid_t& cube_id,
            Cube& cube) const;
    void readCubeDescription(
            mongoc_cursor_t *cursor,
            const char*& cube_type,
            bson_oid_t& cube_id,
            int* cube_dim) const;
    virtual bool readAffine(
            mongoc_cursor_t *cursor,
            float* orig,
            float* pixdim) const = 0;


};

class DRIVERSSHARED_EXPORT MongoFoamDriver  : public MongoCubeDriver
{
public:
    MongoFoamDriver();
    virtual ModelOpenInfo* openInfo() const override;
    virtual bool isValid( const ModelOpenInfo* ) const override;

protected:
    virtual bool readAffine(
            mongoc_cursor_t *cursor,
            float* orig,
            float* pixdim) const;
};

}

#endif // MONGO_CUBE_DRIVER_H

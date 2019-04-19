#ifndef MONGO_CUBE_DRIVER_H
#define MONGO_CUBE_DRIVER_H

#include <q3D/drivers/drivers_global.h>

#include <mongoc.h>

#include <q3D/model/model_open_info.h>
#include <q3D/model/model_driver.h>
#include <q3D/cube/cube.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT MongoCubeOpenInfo : public ModelOpenInfo {
public:
    MongoCubeOpenInfo();

    const QString& getURI() const;
    void setURI( const QString& uri );

    const QString& getDatabase() const;
    void setDatabase(const QString& database);

    const QString& getExperience() const;
    void setExperience(const QString& experience);

    int getNumero() const;
    void setNumero(int numero);

    int getSerie() const;
    void setSerie( int serie );

    QString getName() const;

    mongoc_client_t* createClient() const;


private:
    QString uri_;
    QString database_;
    QString experience_;
    int numero_;
    int serie_;
};


class DRIVERSSHARED_EXPORT MongoCubeDriver  : public ModelDriver
{
public:
    MongoCubeDriver();

    virtual ModelOpenInfo* openInfo() const override;
    virtual bool canHandle(Model *) const override;
    virtual Model* open( const ModelOpenInfo& ) override;
    virtual void save( const Model& model, const ModelOpenInfo&  ) override;

private:
    Cube* getCubeInfo(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            bson_oid_t& cube_id);
    void loadCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            const bson_oid_t& cube_id,
            Cube& cube);
};

}

#endif // MONGO_CUBE_DRIVER_H

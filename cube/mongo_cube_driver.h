#ifndef MONGO_CUBE_DRIVER_H
#define MONGO_CUBE_DRIVER_H

#include <q3D/cube/cube_global.h>

#include <mongoc.h>

#include <q3D/model/model_open_info.h>
#include <q3D/model/model_driver.h>
#include <q3D/cube/cube.h>

namespace Q3D {

class CUBESHARED_EXPORT MongoCubeOpenInfo : public ModelOpenInfo {
public:
    static QString URI;
    static QString DATABASE;

public:
    MongoCubeOpenInfo(
            const QString& uri = MongoCubeOpenInfo::URI,
            const QString& database = MongoCubeOpenInfo::DATABASE);

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


class CUBESHARED_EXPORT MongoCubeDriver  : public ModelDriver
{
public:
    MongoCubeDriver();

    virtual Model* open( const ModelOpenInfo& );
    virtual void save( const Model& model, const QString& fileName  );

private:
    void getCubeInfo(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            bson_oid_t& cube_id,
            Cube& cube);
    void loadCube(
            mongoc_client_t* client,
            const MongoCubeOpenInfo& moi,
            const bson_oid_t& cube_id,
            Cube& cube);
};

}

#endif // MONGO_CUBE_DRIVER_H

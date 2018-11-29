#ifndef REGION_DB_H
#define REGION_DB_H

#include <q3D/mesh/mesh_global.h>

#include <QObject>

#include <q3D/mesh/mesh.h>
#include <q3D/mesh/region.h>
#include <q3D/model/factory.hpp>

namespace Q3D {



class MESHSHARED_EXPORT RegionDB : public QObject
{
    Q_OBJECT

    class Factory : public ObjectFactory<Region, QString> {
    public:
        Factory();
    };

public:
    static RegionDB::Factory& factory();


public:
    explicit RegionDB( Mesh *parent, Element::Type type );

    Mesh* get_mesh() const;

    const Element::Type& get_type() const;

    int next_available_region_id();
    int last_region_id() const;
    void set_last_region_id( int index );

    Region* create_region(
            const QString& region_key,
            const QString& visible_name,
            const QString& aster_name = QString::null
            );
    Region* find_region( const QString& region_name, bool use_visible_name = false ) const;

private:
    static Factory region_factory_;


private:
    Element::Type type_;
    int last_id_;
    
};

inline Mesh* RegionDB::get_mesh() const {
    return static_cast<Mesh*>( parent() );
}

inline const Element::Type& RegionDB::get_type() const {
    return type_;
}


}

#endif // REGION_DB_H

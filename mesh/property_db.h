#ifndef PROPERTY_DB_H
#define PROPERTY_DB_H

#include <q3D/mesh/mesh_global.h>

#include <QObject>
#include <QSet>

#include <q3D/mesh/mesh.h>
#include <q3D/mesh/property.h>
#include <q3D/mesh/property_cube.h>

namespace Q3D {


class MESHSHARED_EXPORT PropertyDB : public QObject
{
    Q_OBJECT
public:
    explicit PropertyDB( Mesh* mesh, Element::Type type );

    Mesh* get_mesh() const;

    const QSet<Property>& properties() const;

    Property add_property( const PropertyInfo& pinfo );
    bool remove_property( const PropertyInfo& pinfo );

    Property find_property( const PropertyInfo& pinfo );

    PropertyCube& cube();

signals:
    void propertyAdded( const Property& );
    void propertyRemoved( const Property& );

private:
    Element::Type type_;
    int offset_;
    QSet<Property> properties_;
    PropertyCube cube_;

};


inline Mesh* PropertyDB::get_mesh() const {
    return static_cast<Mesh*>( parent() );
}

inline PropertyCube& PropertyDB::cube() {
    return cube_;
}

inline const QSet<Property>& PropertyDB::properties() const  {
    return properties_;
}

}
#endif // PROPERTY_DB_H

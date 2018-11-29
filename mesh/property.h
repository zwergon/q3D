#ifndef PROPERTY_H
#define PROPERTY_H

#include <q3D/mesh/mesh_global.h>

#include <QString>
#include <QPointer>

#include <q3D/mesh/types.h>

namespace Q3D {

class PropertyDB;

/****************************************************************/
class MESHSHARED_EXPORT PropertyInfo
{
public:
    PropertyInfo();
    PropertyInfo( const QString& name, int size = 1, Element::Type type = Element::CELL );

    void set_name( const QString& name );
    const QString& name() const;

    void set_size( int size );
    int size() const;

    void set_type( Element::Type type );
    Element::Type type() const;

    bool operator ==(const PropertyInfo& ) const;
    bool operator !=(const PropertyInfo& ) const;

protected:
    QString name_;
    int size_;
    Element::Type type_;

};

inline void PropertyInfo::set_name(const QString &name){
    name_ = name;
}

inline const QString& PropertyInfo::name() const {
    return name_;
}

inline void PropertyInfo::set_size(int size) {
    size_ = size;
}

inline int PropertyInfo::size() const {
    return size_;
}

inline Element::Type PropertyInfo::type() const {
    return type_;
}

inline void PropertyInfo::set_type( Element::Type type ) {
    type_ = type;
}


/****************************************************************/
class MESHSHARED_EXPORT Property : public PropertyInfo {
public:
    static Property null;

public:
    PropertyDB* property_db() const;

    void get_value( qint64, QVector<double>& value ) const;
    void set_value( qint64, const QVector<double>& value );

private:
    Property( const PropertyInfo& pinfo );

private:
    int offset_;
    QPointer<PropertyDB> db_;

    friend class PropertyDB;

};

MESHSHARED_EXPORT uint qHash(const Q3D::PropertyInfo& p);

}



#endif // PROPERTY_H

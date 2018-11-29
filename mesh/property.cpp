#include "property.h"

#include <QHash>
#include <QPair>
#include <QVector>
#include <q3D/mesh/property_db.h>
#include <q3D/mesh/property_cube.h>



namespace Q3D {
/**********************************************************/
uint qHash(const Q3D::PropertyInfo& p){
    return ::qHash( QPair<QString, int>(p.name(), p.size() ) );
}


PropertyInfo::PropertyInfo()
    : name_(QString::null),
      size_(0)
{
}

PropertyInfo::PropertyInfo( const QString& name, int size, Element::Type type )
    : name_( name),
      size_( size),
      type_( type )
{
}

bool PropertyInfo::operator==( const PropertyInfo& p ) const {
    return ( p.name_ == name_ ) && ( p.size_ == size_ ) && ( p.type_ == type_ );
}

bool PropertyInfo::operator!=( const PropertyInfo& p ) const {
    return ( p.name_ != name_ ) || ( p.size_ != size_ ) || ( p.type_ != type_ );
}

/*******************************************************************/

Property Property::null = Property( PropertyInfo("dummy", -1) );

Property::Property(const PropertyInfo &pinfo)
    : PropertyInfo( pinfo.name(), pinfo.size() ),
      offset_(-1),
      db_(0)
{
}

PropertyDB* Property::property_db() const {
    return db_.data();
}

void Property::get_value( qint64 id, QVector<double>& value ) const {

    Q_CHECK_PTR( db_ );

    value.resize( size_ );
    for( int i=0; i<size_; i++){
        value[i] = db_->cube().value( id, offset_+i);
    }
}

void Property::set_value( qint64 id, const QVector<double>& value ) {
    Q_ASSERT( value.size() == size_ );
    for( int i=0; i<size_; i++){
        db_->cube().set_value( id, offset_+i, value[i] );
    }
}




}

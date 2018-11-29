#include "property_db.h"

#include <QDebug>

namespace Q3D {


PropertyDB::PropertyDB(Mesh* mesh, Element::Type type )
    : QObject( mesh),
      type_( type ),
      offset_(0)


{
     setObjectName( Element::name( type ) );
}


Property PropertyDB::add_property( const PropertyInfo& pinfo ){

    if ( properties_.contains(pinfo) ){
        qWarning() << "property " << pinfo.name() << " already exists in PropertyDB";
        return Property::null;
    }


    Property prop( pinfo );
    prop.offset_= offset_;
    prop.db_ = this;
    properties_.insert( prop );

    offset_ += pinfo.size();

    emit propertyAdded( prop );

    return prop;
}

bool PropertyDB::remove_property( const PropertyInfo& pinfo ){

    Property prop = find_property(( pinfo ) );
    if ( prop == Property::null ){
        return false;
    }

    emit propertyRemoved( prop );

    properties_.remove( pinfo );

    return true;

}

Property PropertyDB::find_property(const PropertyInfo &pinfo){
    QSet<Property>::iterator itp = properties_.find(pinfo);
    if ( itp != properties_.end() ){
        return *itp;
    }

    return Property::null;
}


}

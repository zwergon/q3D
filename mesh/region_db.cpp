#include "region_db.h"

#include <QDebug>

#include "region.h"


namespace Q3D {

RegionDB::Factory::Factory(){
    registerFactory<RegionGroup>( "RegionGroup" );
    registerFactory<NodeRegion>( "NodeRegion" );
    registerFactory<CellRegion>( "CellRegion" );
}

RegionDB::Factory RegionDB::region_factory_;

RegionDB::Factory& RegionDB::factory() {
    return region_factory_;
}

RegionDB::RegionDB( Mesh* mesh, Element::Type type ) :
    QObject(mesh),
    type_(type),
    last_id_(0)
{
    setObjectName( Element::name( type ) );
}

int RegionDB::next_available_region_id() {
      return ++last_id_;
  }

  int RegionDB::last_region_id() const {
      return last_id_;
  }

  void RegionDB::set_last_region_id( int index ) {
      last_id_ = index;
  }

  Region* RegionDB::create_region(
          const QString& region_key,
          const QString& visible_name,
          const QString& a_name )
  {

      Region* region = region_factory_.create( region_key );
      if ( 0 == region ){
          return 0;
      }
      QString aster_name(a_name);
      if ( aster_name == QString::null ){

          int r_id = next_available_region_id();

          switch( type_ ){
          case Element::NODE:
              aster_name = QString("N%1").arg(r_id, 0, 16).toUpper();
              break;
          case Element::EDGE:
              aster_name = QString("R%1").arg(r_id, 0, 16).toUpper();
              break;
          case Element::FACE:
              aster_name = QString("S%1").arg(r_id, 0, 16).toUpper();
              break;
          case Element::CELL:
          default:
              aster_name = QString("T%1").arg(r_id, 0, 16).toUpper();
              break;
          }
      }

      region->set_visible_name( visible_name );
      region->setParent( this );
      region->set_aster_name(aster_name);

      qDebug() << "region " << aster_name << " added to mesh";

      return region;

  }


  Region* RegionDB::find_region( const QString& region_name, bool use_visible_name ) const {
      QListIterator<Region*> itr( findChildren<Region*>() );
      while( itr.hasNext() ){
          Region* region = itr.next();
          if ( ( use_visible_name ) && (region->visible_name() == region_name ) ){
              return region;
          }
          else if ( region->objectName() == region_name ){
              return region;
          }
      }

      return 0;
  }


}

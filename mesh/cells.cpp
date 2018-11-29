#include "cells.h"

#include <q3D/mesh/types.h>



namespace Q3D {

void ICell::set_id( quint64 ce_id )
{
    id_ = ce_id;
}

quint64 ICell::get_id() const
{
    return id_;
}

void ICell::get_sorted( QVector<corner_id>& corners ) const {
    corners = *this;
    qSort( corners.begin(), corners.end() );
}

/**************************************************************************/
Edge::Edge( corner_id i1, corner_id i2)
    : ICell( LocalCellTopology::EDGE2, 2)
{
    if( i1 == i2 ){
        replace(0, -1 );
        replace(1, -1 );
        return;
    }

    replace(0, i1 );
    replace(1, i2 );
}

/**
   *  use 2 elements to create hash key
   */
uint qHash(const Edge &e){
    QVector<corner_id> corners; e.get_sorted( corners );
    quint64 hash = ( ( corners[1] << 24 ) | corners[0] );
    return ::qHash( hash );
}

/**************************************************************************/
 Facet::Facet() :ICell( LocalCellTopology::UNDEFINED_CELL, 0 ){
 }

 /**
 * make a sorted vector of corner_id, to be able to segregate faces.
 */
 Facet::Facet( corner_id id1, corner_id id2, corner_id id3 )
     : ICell( LocalCellTopology::TRIA3, 3)
 {
     replace(0, id1);
     replace(1, id2);
     replace(2, id3);
 }


 Facet::Facet( corner_id id1, corner_id id2, corner_id id3, corner_id id4 )
     : ICell( LocalCellTopology::QUAD4, 4)
 {
     replace(0, id1);
     replace(1, id2);
     replace(2, id3);
     replace(3, id4);
}

 Facet::Facet( const CornerList& corners )
     : ICell( LocalCellTopology::UNDEFINED_CELL, corners.count() )
 {
     QListIterator<corner_id> itco( corners );
     int i=0;
     while ( itco.hasNext() ){
         replace(i++, itco.next());
     }

 }

 Facet::Facet( const CornerSet& corners )
     : ICell( LocalCellTopology::UNDEFINED_CELL, corners.count() )
 {
     QSetIterator<corner_id> itco( corners );
     int i=0;
     while ( itco.hasNext() ){
         replace(i++, itco.next());
     }

 }


 /**
  * only use 3 first elements to create hask key
  */
 uint qHash(const Facet &f){
     QVector<corner_id> corners; f.get_sorted( corners );
     quint64 hash = ( corners[2] << 40 ) | ( corners[1] << 20 ) | corners[0];
     return ::qHash( hash );
 }

 /**************************************************************************/
 Triangle::Triangle( corner_id i1, corner_id i2, corner_id i3 )
     : Facet( i1, i2, i3 )
 {
 }

  /**************************************************************************/
 Quad::Quad( corner_id i1, corner_id i2, corner_id i3, corner_id i4 )
     : Facet( i1, i2, i3, i4 )
 {
 }

 /**************************************************************************/
 Tetra::Tetra( corner_id i1, corner_id i2, corner_id i3, corner_id i4 )
     :  ICell( LocalCellTopology::TETRAEDRON4, 4)
 {
     replace(0, i1);
     replace(1, i2);
     replace(2, i3);
     replace(3, i4);
 }



}

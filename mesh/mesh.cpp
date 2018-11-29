/*
 * Mesh.cpp
 *
 *  Created on: 23 avr. 2013
 *      Author: lecomtje
 */

#include <q3D/mesh/mesh.h>
#include <q3D/mesh/property_db.h>
#include <q3D/mesh/region_db.h>

namespace Q3D {

/**********************************************************/

Mesh::Mesh() {

    new PropertyDB(this, Element::NODE );
    new PropertyDB(this, Element::CELL );

    new RegionDB(this, Element::NODE);
    new RegionDB(this, Element::FACE);
    new RegionDB(this, Element::CELL);

}

Mesh::~Mesh() {

    QHashIterator<cell_id, Point3d*> it_n( nodes_ );
    while( it_n.hasNext() ){
        it_n.next();
        delete it_n.value();
    }

    QHashIterator<cell_id, ICell*> it_e( cells_ );
    while( it_e.hasNext() ){
        it_e.next();
        delete it_e.value();
    }
}

const Box3d Mesh::box() const {
    return box_;
}

/**
  * return cell_id and face index on this cell for one face_id.
  * \see get_face_id
  */
cell_id  Mesh::get_cell_and_face( const face_id& f_id, int& face ) const {
    face = f_id & 7;
    return (f_id >> 3);
}

/**
  * return face_id constructed on cell_id and face index.
  */
face_id  Mesh::get_face_id( const cell_id& c_id, int face ) const {
    return ( ( c_id << 3 ) |  face  );
}

int Mesh::size( Element::Type type ) const {
    switch( type ){
    case Element::NODE:
        return nodes_.size();
    case Element::CELL:
        return cells_.size();
    default:
        return 0;
    }
}

static bool caseInsensitiveLessThan(const Property &s1, const Property &s2){
    return s1.name().toLower() < s2.name().toLower();
}

/**
 * fill properties with all Property hold by all PropertyDB.
 */
void Mesh::get_all_properties( QList<Property>& properties ){

    properties.clear();
    QListIterator<PropertyDB*> it_pdb( findChildren<PropertyDB*>() );
    while( it_pdb.hasNext() ){
        PropertyDB* pdb = it_pdb.next();
        properties.append( pdb->properties().toList() );
    }

    qSort( properties.begin(), properties.end(), caseInsensitiveLessThan );
}

/**
 * @brief Mesh::find_property find Property corresponding to PropertyInfo
 * @param pinfo Description of the property to find.
 * @param create if not found create it. ( true by default )
 * @return the found Property or Property::null
 */
Property Mesh::find_property( const PropertyInfo& pinfo, bool create ){

    PropertyDB* prop_db = findChild<PropertyDB*>( Element::name(pinfo.type()) );
    if ( 0 == prop_db ){
        return Property::null;
    }

    Property prop = prop_db->find_property( pinfo );
    if ( prop != Property::null ){
        return prop;
    }

    if ( create ){
        return prop_db->add_property( pinfo );
    }

    return Property::null;
}


RegionDB* Mesh::find_region_db( Element::Type type ){
    return findChild<RegionDB*>( Element::name(type) );
}

void Mesh::get_all_corners( QList<corner_id>& corners ) const{
    corners =  nodes_.keys();
}

void Mesh::get_all_cells( QList<cell_id>& cells ) const {
    cells = cells_.keys();
}

const Point3d& Mesh::get_point( corner_id index ) const {
    return *(nodes_[index]);
}

bool Mesh::get_point( corner_id index, Point3d& point ) const {
    if ( nodes_.contains( index ) ){
        point = *(nodes_[index]);
        return true;
    }
    return false;
}

bool Mesh::set_point( corner_id index, const Point3d& target ){
    if ( nodes_.contains( index ) ){
        *(nodes_[index]) = target;
        return true;
    }
    return false;
}

corner_id Mesh::add_point( const Point3d& point, corner_id id ){
    corner_id node_index = id;
    if ( -1 == node_index ){
        node_index = next_available_corner();
    }

    nodes_.insert(node_index, new Point3d(point) );

    return node_index;

}

cell_id Mesh::add_cell( ICell* cell, cell_id id  ) {
    cell_id cell_index = id;
    if ( -1 == cell_index ){
        cell_index = next_available_cell();
    }

    cell->set_id( cell_index );
    cells_.insert(cell_index, cell);

    return cell_index;

}

const ICell& Mesh::get_cell( cell_id ce_id ) const {
    DLIB_ASSERT( cells_.contains( ce_id ), "mesh doesn't contain this cell index" );
    return *(cells_[ce_id]);
}

bool  Mesh::get_cell( cell_id index, ICell*& cell ) const {
    if ( cells_.contains( index ) ){
        cell = cells_[index];
        return true;
    }
    return false;
}

void Mesh::update()
{

}

void Mesh::update_bounding_box(){

    Point3d& min_ = box_[0];
    Point3d& max_ = box_[1];

    min_ = wykobi::positive_infinite_point3d<double>();
    max_ = wykobi::negative_infinite_point3d<double>();


    QHashIterator<corner_id, Point3d*> it_points( nodes_ );
    while( it_points.hasNext() ){
        it_points.next();
        Point3d* pt = it_points.value();

        if ( pt->x < min_[0] )
            min_[0] = pt->x;
        else if ( pt->x > max_[0] )
            max_[0] = pt->x;

        if ( pt->y < min_[1] )
            min_[1] = pt->y;
        else if ( pt->y > max_[1] )
            max_[1] = pt->y;

        if ( pt->z < min_[2] )
            min_[2] = pt->z;
        else if ( pt->z > max_[2] )
            max_[2] = pt->z;

    }
}



std::ostream& operator<< (std::ostream &out, Mesh& mesh)
{
    CornerList corners;
    mesh.get_all_corners( corners );

    out << "n_nodes:" << corners.count() << std::endl;

    QListIterator<corner_id> it_co( corners );
    while( it_co.hasNext() ){
        corner_id co_id = it_co.next();
        const Point3d& pt = mesh.get_point( co_id );
        out << co_id << ": " <<  pt.x << "," << pt.y << "," << pt.z << std::endl;

    }

    QList<cell_id> cells;
    mesh.get_all_cells( cells );

    out << "n_cells:" << cells.count() << std::endl;

    QListIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext() ){
        cell_id ce_id = it_ce.next();
        const ICell& cell = mesh.get_cell( ce_id );
        out << cell.get_id() << ": ";
        for( int i= 0; i< cell.count(); i++ ){
            out <<  cell[i] << " ";
        }
        out << std::endl;
    }

    return out;
}


}

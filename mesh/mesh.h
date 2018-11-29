/*
 * Mesh.h
 *
 *  Created on: 23 avr. 2013
 *      Author: lecomtje
 */

#ifndef MESH_H_
#define MESH_H_

#include <q3D/mesh/mesh_global.h>

#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QHash>

#include <q3D/mesh/types.h>
#include <q3D/mesh/cells.h>
#include <q3D/mesh/property.h>

namespace Q3D {

class RegionDB;

/**********************************************************/

class MESHSHARED_EXPORT Mesh : public QObject {
    Q_OBJECT
public:
    Mesh();
    virtual ~Mesh();

    const Box3d box() const;

    void get_ids(
            corner_id& co_id,
            cell_id& ce_id
            ) const;

    void reset_ids(
            corner_id co_id = -1,
            cell_id = -1 );

    cell_id  get_cell_and_face( const face_id& f_id, int& face ) const;
    face_id  get_face_id( const cell_id& c_id, int face ) const;

    void get_all_corners( QList<corner_id>& corners ) const;
    void get_all_cells( QList<cell_id>& cells ) const;

    int size( Element::Type ) const;

    void get_all_properties( QList<Property>& properties );
    Property find_property(
            const PropertyInfo& pinfo,
            bool create = true );

    RegionDB* find_region_db( Element::Type type );

    corner_id add_point( const Point3d& point, corner_id co_id = -1 );
    const Point3d& get_point( corner_id index ) const;
    bool get_point( corner_id index, Point3d& point ) const;
    bool set_point( corner_id index, const Point3d& target );

    cell_id add_cell( ICell* cell, cell_id ce_id = -1 );
    bool get_cell( cell_id ce_id, ICell*& cell ) const;
    const ICell& get_cell( cell_id ce_id ) const;


    void update();
    void update_bounding_box();

protected:
    corner_id next_available_corner();
    cell_id   next_available_cell();

protected:
    Box3d box_;
    corner_id co_id_; /**< used to give an unique node id */
    cell_id   ce_id_; /**< used to give an unique cell id */
    QHash<corner_id, Point3d*> nodes_;
    QHash<cell_id, ICell*> cells_;

};

inline void Mesh::get_ids ( corner_id& co_id, cell_id& ce_id ) const{
    co_id = co_id_;
    ce_id = ce_id_;
}

/**
    * reset indexation counter for  node, face and cell id
    */
inline void Mesh::reset_ids( corner_id co_id, cell_id ce_id ){
    co_id_ = co_id;
    ce_id_ = ce_id;
}

/**
    * give the next id free and available for indexing a node
    */
inline corner_id Mesh::next_available_corner(){
    return ++co_id_;
}

/**
    * give the next id free and available for indexing a cell
    */
inline cell_id  Mesh::next_available_cell(){
    return ++ce_id_;
}


std::ostream& operator<< (std::ostream &out, Mesh& mesh);


}

#endif /* MESH_H_ */

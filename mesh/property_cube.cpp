/**
 * \file 
 * Copyright (C) 2004-2009 by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Devoitine Herve
 *
 * $Id: property_cube.cpp 242432 2012-08-09 13:50:03Z lecomte $
 */
#include <q3D/mesh/property_cube.h>

#include <q3D/mesh/mesh.h>
#include <limits>

namespace Q3D {

/**
 * \class PropertyCube
 * @ingroup topology
 *
 * base class for storing all properties values of one Indexation.
 * There is one PropertyCube for each kind (NODE, FACE, CELL) of topological element dimension
 * PropertyCube is owned as a a child by Mesh and use for specific PropertyDB
 */


    void PropertyCube::reserve( int size ){
    	cube_.reserve( size );
    }

    /*!
     * return all id used by this PropertyCube (corner_id, face_id, cell_id)
     */
    QList<qint64> PropertyCube::get_all() const {
        return cube_.keys();
    }

    /*!
     * get value at offset for element defined by id
     */
    double PropertyCube::value( qint64 id, int offset ) const {

        if ( cube_.contains(id) ){
    	    const QVector<double>& data = cube_[id];
    	    return data[offset];
    	}

        return std::numeric_limits<double>::infinity();
    }


    /*!
     * set value at offset for element defined by id
     */
    void PropertyCube::set_value( qint64 id, int offset, double value ) {

        QVector<double>& data = cube_[id];
    	if ( offset >= data.size() ){
                data.resize( data.size()+20 );
    	}
    	data[offset] = value;
    }


   
} // namespace Q3D


/*
 * gmsh_mesh_importer.h
 *
 *  Created on: 8 nov. 2011
 *      Author: lecomtje
 */

#ifndef GMSH_MESH_IMPORTER_H_
#define GMSH_MESH_IMPORTER_H_

#include <q3D/filters/filters_global.h>

#include <q3D/mesh/mesh.h>
#include <q3D/filters/mesh_importer.h>
#include <q3D/mesh/property.h>

#include <QFileInfo>

namespace Q3D {

typedef enum State_ {
    UNKNOWN,
    POINTS,
    CELLS,
    DATA_STRING_TAG,
    DATA_REAL_TAG,
    DATA_INTEGER_TAG,
    DATA
} GmshState;




class GmshContext {
public:
    GmshState state_;
    int n_steps_;
    int i_step_;
    MeshImporter::Data* data_;


    GmshContext() : state_(UNKNOWN), n_steps_(0), i_step_(0), data_(0) {
    }

    void clear(){
        state_ = UNKNOWN;
        i_step_ = 0;
        n_steps_ = 0;
        data_ = 0;
    }

    bool isFinished(){
        return i_step_ == n_steps_;
    }
};

class FILTERSSHARED_EXPORT GmshMeshImporter : public MeshImporter {
public:
	GmshMeshImporter();

private:


protected:
	virtual bool parse( const QString& line );

private:
	void get_state( const QString& );
	void load_vertex( const QString& );
	void load_cell( const QString& );
    void load_data_string( const QString& );
    void load_data_real( const QString& );
    void load_data_int( const QString& );
    void load_data( const QString& );

	void load_one_cell( const QStringList& tokens );
	void load_one_face( const QStringList& tokens );

private:
    GmshContext context_;

};
}

#endif /* GMSH_MESH_IMPORTER_H_ */

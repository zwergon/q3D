#ifndef GMSH_MESH_EXPORTER_H
#define GMSH_MESH_EXPORTER_H

#include <q3D/filters/filters_global.h>

#include <q3D/mesh/mesh.h>

#include <QTextStream>
#include <QFileInfo>
#include <QList>
#include <QMap>


namespace Q3D {

/***************************************************************/
class FILTERSSHARED_EXPORT GmshMeshExporter {

public:
    GmshMeshExporter();
    virtual ~GmshMeshExporter(){}

    void set_filename( const QString& filename );

    bool to_file( Mesh* mesh );

    const char* suffix() { return ".gmsh"; }
    bool serialize( QTextStream& mesh_ostream, Mesh* mesh );

    void set_only_tetra( bool only_tetra );


protected:
    void build_elements();
    void write_header( QTextStream& mesh_ostream );
    void write_nodes( QTextStream& mesh_ostream );
    void write_elements( QTextStream& mesh_ostream );
    void write_property( QTextStream& mesh_ostream );

    virtual void export_cell(
            QTextStream& mesh_ostream,
            quint64 key, cell_id ce_id,
            const QList<quint64>& tags );

    virtual int export_tetra_cells(
            QTextStream& mesh_ostream,
            quint64 key, cell_id ce_id,
            const QList<quint64>& tags );


    protected:
        QFileInfo file_info_;

    private:
        Mesh* mesh_;
        QMap<quint64, QList<quint64> > elements_;
        bool only_tetra_;

    };

    inline void GmshMeshExporter::set_only_tetra( bool only_tetra ){
        only_tetra_ = only_tetra;
    }


}

#endif // GMSH_MESH_EXPORTER_H

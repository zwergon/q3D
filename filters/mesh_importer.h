/*
 * mesh_importer.h
 *
 *  Created on: 4 nov. 2011
 *      Author: lecomtje
 */

#ifndef MESH_IMPORTER_H_
#define MESH_IMPORTER_H_

#include <q3D/filters/filters_global.h>

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QFileInfo>


#include <q3D/mesh/mesh.h>
#include <q3D/mesh/cells.h>
#include <q3D/mesh/property.h>


namespace Q3D {

class FILTERSSHARED_EXPORT MeshImporter {

public:
    class Data {
    public:
        PropertyInfo pinfo_;
        QHash< qint64, QVector<double> > values_;
    };

public:
	MeshImporter();
    virtual ~MeshImporter();

	void set_filename( const QString& filename );

    bool create_mesh( Mesh* mesh );

protected:
	bool import();
	virtual bool parse( const QString& line ) = 0;

    void register_universe( Mesh* );
    void reorient_tetra( Tetra& tetra );

protected:
	QFileInfo file_info_;
    QHash< quint64, Point3d> vertices_;
    QHash< quint64, ICell*> cells_;
    QHash< quint64, QSet<cell_id> > regions2d_;
    QHash< quint64, QSet<cell_id> > regions3d_;
    QList<Data*> datas_;


};

}

#endif /* MESH_IMPORTER_H_ */

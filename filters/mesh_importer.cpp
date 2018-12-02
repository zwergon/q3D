/*
 * mesh_importer.cpp
 *
 *  Created on: 4 nov. 2011
 *      Author: lecomtje
 */

#include <filters/mesh_importer.h>

#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtGui/QGenericMatrix>

#include <q3D/mesh/region.h>
#include <q3D/mesh/region_db.h>

#include <q3D/model/matrix.h>


namespace Q3D {


	MeshImporter::MeshImporter(){
	}

    MeshImporter::~MeshImporter(){
        QListIterator<Data*> it_d( datas_);
        while( it_d.hasNext() ){
            delete it_d.next();
        }
    }

	void MeshImporter::set_filename( const QString& filename ){
		file_info_.setFile( filename );
	}

	bool MeshImporter::import(){

		QFile file( file_info_.absoluteFilePath() );
		if (!file.open( QIODevice::ReadOnly | QIODevice::Text )){
            qWarning() << QString("Unable to open file %1 Return\n").arg( file.fileName() ) ;
			return false;
		}

		QTextStream istream( &file );
		while ( !istream.atEnd() ){

            QString line = istream.readLine().trimmed();

			if ( !parse( line ) ){
                qWarning() << QString("unable to parse %1").arg( line );
			}

		}

		file.close();

		return true;

	}

    void MeshImporter::register_universe( Mesh* mesh ){

        RegionDB* region_db = mesh->find_region_db( Element::CELL );
        QList<cell_id> cells; mesh->get_all_cells( cells );


        CellRegion* region = static_cast<CellRegion*>(
                    region_db->create_region( "CellRegion", "Universe" ) );
        region->set_cells( cells.toSet()  );


	}

    bool MeshImporter::create_mesh( Mesh* mesh ) {

        if (!import()){
            return false;
        }

        QHashIterator<quint64, Point3d> itp( vertices_ );
		corner_id max_co_id = -1;
		while( itp.hasNext() ){
			itp.next();
            corner_id co_id = mesh->add_point( itp.value(), itp.key() );
			if ( co_id > max_co_id ){
				max_co_id = co_id;
			}
		}

        QHashIterator<quint64, ICell*> itc( cells_ );
		cell_id max_ce_id = -1;
		while( itc.hasNext() ){
			itc.next();
            cell_id ce_id = mesh->add_cell( itc.value(), itc.key() );
			if ( ce_id > max_ce_id ){
				max_ce_id = ce_id;
			}
		}

        mesh->reset_ids( max_co_id, max_ce_id );


        QListIterator<Data*> it_d (datas_);
        while( it_d.hasNext() ){
            Data* data = it_d.next();
            Property prop = mesh->find_property( data->pinfo_ );
            if ( prop != Property::null ){
                QHashIterator<qint64, QVector<double> > it_v( data->values_ );
                while( it_v.hasNext() ){
                    it_v.next();
                    prop.set_value( it_v.key(), it_v.value() );
                }
            }

        }


        {
            /*Region3d*/
            RegionDB* region_db = mesh->find_region_db( Element::CELL );
            QHashIterator<quint64, QSet<cell_id> > it_region(regions3d_);
            while( it_region.hasNext() ){
                it_region.next();
                CellRegion* region = static_cast<CellRegion*>(
                            region_db->create_region( "CellRegion", QString::number( it_region.key() )  ) );
                region->set_cells( it_region.value() );
            }
        }

        {
            /*Region2d*/
            RegionDB* region_db = mesh->find_region_db( Element::FACE );
            QHashIterator<quint64, QSet<cell_id> > it_region(regions2d_);
            while( it_region.hasNext() ){
                it_region.next();
                CellRegion* region = static_cast<CellRegion*>(
                            region_db->create_region( "CellRegion", QString::number( it_region.key() )  ) );
                region->set_cells( it_region.value() );
            }
         }

        return true;
	}

    /**
     * @brief MeshImporter::reorient_tetra reorder tetra nodes to ensure jacobian computed
     * on this tetra is positiv.
     *
     */
    void MeshImporter::reorient_tetra(Tetra& tetra){
        corner_id co_id1 = tetra[0];
        corner_id co_id2 = tetra[1];
        corner_id co_id3 = tetra[2];
        corner_id co_id4 = tetra[3];

        const Point3d& pt1 = vertices_[co_id1];
        const Point3d& pt2 = vertices_[co_id2];
        const Point3d& pt3 = vertices_[co_id3];
        const Point3d& pt4 = vertices_[co_id4];


        Matrix3d A;
        A(0,0) = pt1[0]-pt4[0];
        A(1,0) = pt1[1]-pt4[1];
        A(2,0) = pt1[2]-pt4[2];
        A(0,1) = pt2[0]-pt4[0];
        A(1,1) = pt2[1]-pt4[1];
        A(2,1) = pt2[2]-pt4[2];
        A(0,2) = pt3[0]-pt4[0];
        A(1,2) = pt3[1]-pt4[1];
        A(2,2) = pt3[2]-pt4[2];

        // computes the inverse of a matrix m
        double det = A.det();
         if ( det < 0 ){
             tetra.replace(1, co_id3);
             tetra.replace(2, co_id2);
         }

    }





}




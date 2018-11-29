#include "gmsh_mesh_exporter.h"


#include <q3D/mesh/types.h>
#include <q3D/mesh/mesh.h>
#include <q3D/mesh/local_cell_topology.h>
#include <q3D/mesh/property.h>
#include <q3D/mesh/region.h>
#include <q3D/mesh/region_db.h>

#include <QRegExp>
#include <QFile>
#include <QDebug>

namespace Q3D {



/**
* aster_name is {R,S,T}+ Hexa number...
* convert to a long long :
* to_decimal( Hexa ) * 100 + Ascii_code( R or S or T )
* S20 -> 32*100 + 83 = 3283
* R9A -> 154*100 + 82 = 15482
*/
quint64 aster2decimal( const QString& aster_name ){
    QRegExp rx( "(\\w)(.*)");
    if ( rx.indexIn( aster_name) != -1 ){
        char ascii_code = rx.cap(1).at(0).toLatin1();
        return ( rx.cap(2).toUInt(0, 16 )*100 + (int)ascii_code );
    }

    return -1;
}

qint64 id2decimal( quint64 id, Element::Type type ){
    switch( type ){
    case Element::CELL:
        return id*10 + 3;
    case Element::FACE:
        return id*10 + 2;
    case Element::NODE:
        return id*10;
    default:
        break;
    }

    return -1;
}

Element::Type decimal2type( quint64 decimal, quint64& id ){
    quint64 type = decimal % 10;
    id = decimal / 10;
    switch( type ){
    case 0:
        return Element::NODE;
    case 2:
        return Element::FACE;
    case 3:
        return Element::CELL;
    default:
        return Element::UNKNOWN_ELEMENT;
    }
}


/*!
   * constructor
   */
GmshMeshExporter::GmshMeshExporter() : only_tetra_(false) {
}

void GmshMeshExporter::set_filename( const QString& filename ){
    file_info_.setFile( filename );
    if ( file_info_.suffix().isEmpty() ){
        file_info_.setFile( QString("%1%2").arg(filename).arg(suffix()) );
    }
}

bool GmshMeshExporter::to_file( Mesh* mesh ){

    QFile file( file_info_.absoluteFilePath() );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        bool ok = serialize( stream, mesh );

        file.close();
        if ( ! ok ){
            qDebug() << "Writing non correct data on file " <<  file.fileName();
            return false;
        }
    }
    else {
        qDebug() << "Unable to open file" <<  file.fileName() <<" for writing";
        return false;
    }

    return true;
}

bool GmshMeshExporter::serialize( QTextStream& mesh_ostream, Mesh* mesh ){
     mesh_ = mesh;

     build_elements();
     write_header( mesh_ostream );
     write_nodes( mesh_ostream );
     write_elements( mesh_ostream );
     write_property( mesh_ostream );

     return true;
 }

 void GmshMeshExporter::write_header( QTextStream& mesh_ostream ) {
     mesh_ostream << "$MeshFormat" << endl;
     mesh_ostream << "2.2 0 8" << endl;
     mesh_ostream << "$EndMeshFormat" << endl;
 }

 void GmshMeshExporter::write_property( QTextStream& mesh_ostream ) {
     //do nothing so far
 }

 void GmshMeshExporter::write_nodes( QTextStream& mesh_ostream ) {

     CornerList corners;
     mesh_->get_all_corners( corners );

     mesh_ostream << "$Nodes" << endl;
     mesh_ostream << corners.count() << endl;
     QListIterator<corner_id> it_co( corners );
     while ( it_co.hasNext() ){
         corner_id co_id = it_co.next();

         const Point3d& point = mesh_->get_point( co_id );
         mesh_ostream << co_id << " " << point.x << " " << point.y << " " << point.z << endl;
     }
     mesh_ostream << "$EndNodes" << endl;
 }


 int GmshMeshExporter::export_tetra_cells(
         QTextStream& mesh_ostream,
         quint64 key, cell_id ce_id,
         const QList<quint64>& tags ){


     const ICell& cell = mesh_->get_cell(ce_id);
     LocalCellTopology* topo = LocalCellTopology::get_desc( cell.type() );

     for( int i_tetra = 0; i_tetra< topo->get_n_tetras(); i_tetra++){

         //. id of the cell
         mesh_ostream << key*10+i_tetra << " 4 ";
         //. tags
         mesh_ostream << tags.count() << " " ;
         QListIterator<quint64> it_tag( tags );
         while( it_tag.hasNext() ){
             mesh_ostream << it_tag.next() << " ";
         }

         for( int i = 0; i<4; i++ ){
             int i_c = topo->get_tetra_indice(i_tetra, i);
             mesh_ostream << cell[i_c] << " ";
         }

         mesh_ostream << endl;
     }

     return topo->get_n_tetras();

 }


 void GmshMeshExporter::export_cell(
         QTextStream& mesh_ostream,
         quint64 key, cell_id ce_id,
         const QList<quint64>& tags ){

     //. id of the cell
     mesh_ostream << key << " ";

     const ICell& cell = mesh_->get_cell(ce_id);

     //. type of the cell
     switch( cell.type() ){
     case LocalCellTopology::TETRAEDRON4:
         mesh_ostream << "4 ";
         break;
     case LocalCellTopology::HEXAEDRON8:
         mesh_ostream << "5 ";
         break;
     case LocalCellTopology::PENTAEDRON6:
         mesh_ostream << "6 ";
         break;
     case LocalCellTopology::PYRAMID5:
         mesh_ostream << "7 ";
         break;
     default:
         break;
     }

     //. tags
     mesh_ostream << tags.count() << " " ;
     QListIterator<quint64> it_tag( tags );
     while( it_tag.hasNext() ){
         mesh_ostream << it_tag.next() << " ";
     }

     //. corners
     LocalCellTopology* topo = LocalCellTopology::get_desc( cell.type() );

     NodeTab& node_order = topo->get_nodes_order( LocalCellTopology::ArcTemOrder );

     for( int i = 0; i<cell.size(); ++i ){
         mesh_ostream << cell[node_order[i]] << " ";
     }

     mesh_ostream << endl;
 }



 void GmshMeshExporter::write_elements( QTextStream& mesh_ostream ){

     int n_elements = 0;
     QString element_str;
     QTextStream element_stream(&element_str);

     QMapIterator<quint64, QList<quint64> > it_elements ( elements_ );
     while( it_elements.hasNext() ){
         it_elements.next();
         quint64 id;
         Element::Type type = decimal2type( it_elements.key(), id );
         switch( type ){
         case Element::CELL:
             {
                 if ( only_tetra_ ){
                     n_elements += export_tetra_cells( element_stream,
                             it_elements.key(),
                             (cell_id)id,
                             it_elements.value() );
                 }
                 else {
                     export_cell( element_stream,
                             it_elements.key(),
                             (cell_id)id,
                             it_elements.value() );
                     n_elements++;
                 }
                 break;
             }

         default:
             break;
         }

     }

     mesh_ostream << "$Elements" << endl;
     mesh_ostream << n_elements << endl;
     mesh_ostream << element_str << endl;
     mesh_ostream << "$EndElements" << endl;
 }

 void GmshMeshExporter::build_elements(){

     RegionDB* region_db = mesh_->find_region_db( Element::CELL );


     QListIterator<CellRegion*> it_r( region_db->findChildren<CellRegion*>() );
     while( it_r.hasNext() ){
         CellRegion* region = it_r.next();

         CellSet cells;
         region->get_cells( cells );

         if ( cells.isEmpty() ){
             continue;
         }

         QString aster_name = region->aster_name();

         quint64 tag = aster2decimal( aster_name );

         QSetIterator<cell_id> it_ce( cells );
         while( it_ce.hasNext() ){
             cell_id ce_id = it_ce.next();

             const ICell& cell = mesh_->get_cell( ce_id );

             /*
              * if all cells are exported decomposed in tetras, no need to test
              * cell type.
              */
             if ( !only_tetra_ ){

                 switch( cell.type() ){
                 case LocalCellTopology::TETRAEDRON4:
                 case LocalCellTopology::HEXAEDRON8:
                 case LocalCellTopology::PENTAEDRON6:
                 case LocalCellTopology::PYRAMID5:
                     break;
                 default:
                     //not valid cell for Gmsh
                     continue;
                 }
             }

             quint64 key = id2decimal( ce_id, Element::CELL );

             elements_[key].append( 99 );
             elements_[key].append( tag );
         }
     }




 }




}

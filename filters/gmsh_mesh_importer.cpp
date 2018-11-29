/*
 * gmsh_mesh_importer.cpp
 *
 *  Created on: 8 nov. 2011
 *      Author: lecomtje
 */

#include <filters/gmsh_mesh_importer.h>

#include <mesh/cells.h>
#include <QtCore/QStringList>
#include <QtCore/QDebug>


namespace Q3D {


GmshMeshImporter::GmshMeshImporter() {
}

bool GmshMeshImporter::parse( const QString& line ){

    switch( context_.state_ ){
	case POINTS:
		load_vertex( line );
		break;
	case CELLS:
		load_cell( line );
		break;
    case DATA_STRING_TAG:
        load_data_string( line );
        break;
    case DATA_REAL_TAG:
        load_data_real( line );
        break;
    case DATA_INTEGER_TAG:
        load_data_int( line );
        break;
    case DATA:
        load_data( line );
        break;
	default:
		break;
	}

	get_state( line );

	return true;

}

void GmshMeshImporter::load_data_string( const QString& line ){

    switch( context_.i_step_ ){
    case 0:
        context_.n_steps_ = line.toInt();
        break;
    case 1:
    {
        //according gmsh documentation, first field is property name
        QString name = line;
        context_.data_->pinfo_.set_name( name.remove('\"') );
        break;
    }
    default:
        break;
    }
    if ( context_.isFinished() ){
        context_.state_ = DATA_REAL_TAG;
        context_.i_step_ = 0;
        context_.n_steps_ = 0;
        return;
    }
    else {
        context_.i_step_++;
    }
}

void GmshMeshImporter::load_data_real( const QString& line ){

    switch( context_.i_step_ ){
    case 0:
        context_.n_steps_ = line.toInt();
        break;
    default:
        //no time step used so far.
        break;
    }
    if ( context_.isFinished() ){
        context_.state_ = DATA_INTEGER_TAG;
        context_.i_step_ = 0;
        context_.n_steps_ = 0;
        return;
    }
    else  {
        context_.i_step_++;
    }
}


void GmshMeshImporter::load_data_int( const QString& line ){

    switch( context_.i_step_ ){
    case 0:
        context_.n_steps_ = line.toInt();
        break;
    case 2:
        context_.data_->pinfo_.set_size( line.toInt() );
        break;
    case 1:
        //according gmsh doc, first item is time step, skip it !
        break;
    default:
        break;
    }

    if ( context_.isFinished() ){
        context_.state_ = DATA;
        context_.i_step_ = 0;
        context_.n_steps_ = 0;
        return;
    }
    else {
        context_.i_step_++;
    }
}

void GmshMeshImporter::load_data( const QString& line ){


   QListIterator<QString> it_t( line.split(" "));
   if ( !it_t.hasNext() ){
       qWarning() << "find no data associated to this line " << line;
       return;
   }

   //first item is id.
   qint64 id = it_t.next().toLongLong();
   QVector<double>& values = context_.data_->values_[id];
   values.resize( context_.data_->pinfo_.size() );

   int index = 0;
   while( it_t.hasNext() ){
       values[index] = it_t.next().toDouble();
       index++;
   }

}


void GmshMeshImporter::get_state( const QString& line ){

    if ( line.contains( "$Nodes") ){
        context_.state_ = POINTS;
	}
	else if ( line.contains("$EndNodes") ){
        context_.clear();
	}
	else if ( line.contains("$Elements") ){
        context_.state_ = CELLS;
	}
	else if ( line.contains("$EndElements") ){
        context_.clear();
	}
    else if ( line.contains("$ElementData") ){
        context_.state_   = DATA_STRING_TAG;
        context_.i_step_  = 0;
        context_.n_steps_ = 0;
        context_.data_ = new GmshMeshImporter::Data;
        context_.data_->pinfo_.set_type( Element::CELL );
    }
    else if  ( line.contains("$EndElementData") ){
        qWarning() << "adding data " << context_.data_->pinfo_.name() << " with size " << context_.data_->pinfo_.size();
        datas_.append( context_.data_ );
        context_.clear();
    }
}

void GmshMeshImporter::load_vertex( const QString& line ){
	 QStringList token = line.split(" ");
	 if ( token.size() == 4 ){
        quint64 id = token.at(0).toLongLong();
		Point3d point;
		for(int i = 0; i<3; ++i ){
			point[i] = token.at(1+i).toDouble();

		}
		vertices_[id] = point;
	}
}

void GmshMeshImporter::load_one_cell( const QStringList& tokens ){

            quint64 id = tokens.at(0).toLongLong();
			int type = tokens.at(1).toInt();
			int n_tags = tokens.at(2).toInt();

			for( int i_tag = 1; i_tag < n_tags; i_tag++ ){
                quint64 tag = tokens.at(3+i_tag).toLongLong();
                switch( type ){
                case 2:
                case 3:
                    regions2d_[tag].insert( (cell_id)id );
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    regions3d_[tag].insert( (cell_id)id );
                    break;
                default:
                    //do nothing
                    break;
                }
			}


            switch( type ){
            case 1: /*Edge*/
            {
                Edge* edge = new Edge(-1,-1);
                edge->replace( 0, tokens.at(3+n_tags).toLongLong() );
                edge->replace( 1, tokens.at(3+n_tags+1).toLongLong() );
                cells_[id] = edge;
                break;
            }

            case 2: /*Tria3*/
            {
                Triangle* trgl = new Triangle(-1,-1,-1);
                for( int i=0; i<3; i++){
                    trgl->replace( i, tokens.at(3+n_tags+i).toLongLong() );
                }
                cells_[id] = trgl;
                break;
            }

            case 3: /*Quad4*/
            {
                Quad* quad = new Quad(-1,-1,-1,-1);
                for( int i=0; i<4; i++){
                    quad->replace( i, tokens.at(3+n_tags+i).toLongLong() );
                }
                cells_[id] = quad;
                break;
            }

            case 4: /*Tetraedra*/
            {
                Tetra* tetra = new Tetra(-1,-1,-1,-1);
                for( int i=0; i<4; i++){
                    tetra->replace( i, tokens.at(3+n_tags+i).toLongLong() );
                }
                reorient_tetra( *tetra );
                cells_[id] = tetra;
                break;
            }

            case 5: /*Hexaedra*/
            {
                break;
            }

            case 6: /*Pentaedron6*/
            {
                break;
            }

            case 7: /*Pyramid5*/
            {
                break;
            }

            }


}



void GmshMeshImporter::load_one_face( const QStringList&  ){

#ifdef LATER
    quint64 id = tokens.at(0).toLongLong();
	int type = tokens.at(1).toInt();
	int n_tags = tokens.at(2).toInt();
	int n_corners;
	switch( type ){
	case 2: /*Trgl*/
		n_corners = 3;
		break;
	case 3: /*Quad*/
		n_corners = 4;
		break;
	default:
		return;
	}

	CornerSet corners;
	for( int i=0; i<n_corners; i++ ){
		corners.insert( (corner_id) tokens.at(3+n_tags+i).toLongLong() );
	}
	Facet facet( corners );
	for( int i_tag = 1; i_tag < n_tags; i_tag++ ){
        quint64 tag = tokens.at(3+i_tag).toLongLong();
		facets_[facet].insert( tag );
	}
#endif
}

void GmshMeshImporter::load_cell( const QString& line){

	QStringList token = line.split(" ");
		if ( token.size() > 3 ){
			bool ok;
            token.at(0).toLongLong(&ok);
			if ( !ok ){
				return;
			}
			int type = token.at(1).toInt();
			switch( type ){
            case 1:
            case 2:
            case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				load_one_cell( token );
				break;
			default:
				break;
			}
		}
}

}




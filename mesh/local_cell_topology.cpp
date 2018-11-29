/**
* \file 
* Copyright (C) 2004-2009 by IFP-GROUP.
* All rights reserved.
*
* IFP Headquarters:
* 1 & 4, avenue de Bois-Preau
* 92852 Rueil-Malmaison Cedex - France
*
* \author Lecomte Jean-Francois
*
* $Id: local_cell_topology.cpp 242816 2012-08-30 08:06:14Z lecomte $
*/

#include <q3D/mesh/local_cell_topology.h>


namespace Q3D {

    /***************************************************************/
    LocalCellTopologyManager::LocalCellTopologyManager() : QVector<LocalCellTopology*>(LocalCellTopology::LAST_CELL_TYPE) {

        LocalCellTopology* desc =
            new LocalCellTopology(LocalCellTopology::UNDEFINED_CELL);
        desc->set_n_nodes( 0 );
        desc->set_n_faces( 0 );
        desc->set_n_edges( 0 );
        desc->set_n_tetras( 0 );;
        replace(LocalCellTopology::UNDEFINED_CELL, desc);


        /***************************************************************/
        /**
        * Hexaedron8Cell is compatible with Cell3d
        *                 6----------------7
        *                / |              / |
        *              /   |            /   |
        *            /     |          /     |
        *          /       |        /       |
        *        4----------------5         |
        *        |         |      |         |
        *        |         |      |         |
        *     ^  |     /\  |      |         |
        *   w |  |    v/   2------|---------3
        *     |  |    /  /        |       /
        *        |     /          |     /
        *        |   /            |   /
        *        | /              | /
        *        0----------------1
        *             --> u
        *
        * face_direction is from 0-6
        * 0,1,2 are the near faces and hence plus faces
        * 3,4,5 are the far faces and hence minus faces
        * C is the center of the hexahedral
        *              (+w) (+v)
        *                5  4
        *                 | /
        *                 |/
        *      (-u)  0 ---C--- 3  (+u)
        *                 /|
        *               / |
        *              1  2
        *           (-v) (-w)
        *
        * refer to http://irlin201/WikiArcaneInfra/index.php/Gestion_des_maillages for description of the meshes
        */

        desc = new LocalCellTopology(LocalCellTopology::HEXAEDRON8);
        desc->set_name("HEXAEDRON8");
        desc->set_n_nodes( 8 );
        desc->set_n_faces( 6);
        desc->set_n_edges( 12);
        desc->set_n_tetras(5);

        desc->set_node( LocalCellTopology::GocadOrder, 0, 1, 2, 3, 4, 5, 6, 7 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 3, 2, 4, 5, 7, 6 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase, 4, 6, 7, 5, 0, 2, 3, 1 );
        desc->set_node( LocalCellTopology::AsterOrder, 0, 1, 3, 2, 4, 5, 7, 6 );

        desc->set_face( 0, 4, 0, 2, 6 );
        desc->set_face( 1, 0, 1, 5, 4 );
        desc->set_face( 2, 0, 1, 3, 2 );
        desc->set_face( 3, 1, 3, 7, 5 );
        desc->set_face( 4, 2, 3, 7, 6 );
        desc->set_face( 5, 4, 5, 7, 6 );

        desc->set_tetra( 0, 0, 1, 3, 5 );
        desc->set_tetra( 1, 0, 3, 2, 6 );
        desc->set_tetra( 2, 4, 5, 6, 0 );
        desc->set_tetra( 3, 5, 6, 7, 3 );
        desc->set_tetra( 4, 0, 3, 6, 5 );

        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,3);
        desc->set_edge( 2, 3,2);
        desc->set_edge( 3, 2,0);
        desc->set_edge( 4, 0,4);
        desc->set_edge( 5, 1,5);
        desc->set_edge( 6, 2,6);
        desc->set_edge( 7, 3,7);
        desc->set_edge( 8, 4,5);
        desc->set_edge( 9, 5,7);
        desc->set_edge( 10,7,6);
        desc->set_edge( 11,6,4);

        replace(LocalCellTopology::HEXAEDRON8, desc );


        desc = new LocalCellTopology(LocalCellTopology::HEMIHEXA7);
        desc->set_name("HEMIHEXA7");

        desc->set_n_nodes( 7);
        desc->set_n_faces( 6);
        desc->set_n_edges( 11);

        desc->set_n_tetras(4);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4, 5, 6 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4, 5, 6 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4, 5, 6 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3, 4, 5, 6 );

        desc->set_face( 0, 0, 1, 2, -1 );
        desc->set_face( 1, 0, 2, 3, 4  );
        desc->set_face( 2, 0, 5, 6, 1  );
        desc->set_face( 3, 0, 4, 5, -1 );
        desc->set_face( 4, 1, 6, 3, 2  );
        desc->set_face( 5, 3, 6, 5, 4  );
        desc->set_tetra(0, 0, 1, 2, 6);
        desc->set_tetra(1, 0, 4, 5, 6);
        desc->set_tetra(2, 4, 6, 3, 2);
        desc->set_tetra(3, 4, 2, 6, 0);
        desc->set_edge( 0, 0, 1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 2,3);
        desc->set_edge( 4, 3,4);
        desc->set_edge( 5, 4,5);
        desc->set_edge( 6, 5,0);
        desc->set_edge( 7, 0,4);
        desc->set_edge( 8, 5,6);
        desc->set_edge( 9, 6,1);
        desc->set_edge( 10,3,6);

        replace(LocalCellTopology::HEMIHEXA7, desc );


        desc = new LocalCellTopology(LocalCellTopology::PENTAEDRON6);
        desc->set_name("PENTAEDRON6");

        desc->set_n_nodes( 6);
        desc->set_n_faces( 5);
        desc->set_n_edges( 9);

        desc->set_n_tetras(3);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3,4,5 );

        desc->set_face( 0, 0, 2, 1, -1 );
        desc->set_face( 1, 0, 3, 5, 2  );
        desc->set_face( 2, 0, 1, 4, 3  );
        desc->set_face( 3, 3, 4, 5, -1 );
        desc->set_face( 4, 1, 2, 5, 4  );

        desc->set_tetra(0, 0, 1, 2, 3 );
        desc->set_tetra(1, 1, 2, 3, 4 );
        desc->set_tetra(2, 3, 4, 5, 2 );

        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 0,3);
        desc->set_edge( 4, 1,4);
        desc->set_edge( 5, 2,5);
        desc->set_edge( 6, 3,4);
        desc->set_edge( 7, 4,5);
        desc->set_edge( 8, 5,3);

        replace(LocalCellTopology::PENTAEDRON6, desc );


        desc = new LocalCellTopology(LocalCellTopology::HEMIHEXA6);
        desc->set_name("HEMIHEXA6");
        desc->set_n_nodes( 6);
        desc->set_n_faces( 6);
        desc->set_n_edges( 10);

        desc->set_n_tetras( 2 );

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3 ,4,5);

        desc->set_face( 0, 0, 1, 2, -1 );
        desc->set_face( 1, 0, 2, 3, 4  );
        desc->set_face( 2, 0, 5, 3, 1  );
        desc->set_face( 3, 0, 4, 5, -1 );
        desc->set_face( 4, 1, 3, 2, -1 );
        desc->set_face( 5, 3, 5, 4, -1 );
        desc->set_tetra(0, 0, 1, 2, 3);
        desc->set_tetra(1, 0, 3, 4, 5);
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 2,3);
        desc->set_edge( 4, 3,4);
        desc->set_edge( 5, 4,5);
        desc->set_edge( 6, 5,0);
        desc->set_edge( 7, 0,4);
        desc->set_edge( 8, 5,3);
        desc->set_edge( 9, 3,1);


        replace(LocalCellTopology::HEMIHEXA6, desc );


        desc = new LocalCellTopology(LocalCellTopology::HEMIHEXA5);
        desc->set_name("HEMIHEXA5");
        desc->set_n_nodes( 5);
        desc->set_n_faces( 4);
        desc->set_n_edges( 7);

        desc->set_n_tetras(1);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3 ,4);

        desc->set_face( 0, 0, 1, 2, -1 );
        desc->set_face( 1, 0, 2, 3, 4  );
        desc->set_face( 2, 0, 4, 3, 1  );
        desc->set_face( 3, 1, 3, 2, -1 );
        desc->set_tetra(0, 0,1,2,3);
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 2,3);
        desc->set_edge( 4, 3,1);
        desc->set_edge( 5, 3,4);
        desc->set_edge( 6, 4,0);

        replace(LocalCellTopology::HEMIHEXA5, desc );


        desc = new LocalCellTopology(LocalCellTopology::ANTIWEDGERIGHT6);
        desc->set_name("ANTIWEDGERIGHT6");
        desc->set_n_nodes( 6);
        desc->set_n_faces( 6);
        desc->set_n_edges( 10);

        desc->set_n_tetras(3);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3,4,5 );

        desc->set_face( 0, 0, 2, 1, -1);
        desc->set_face( 1, 0, 3, 5, 2 );
        desc->set_face( 2, 0, 1, 4, 3 );
        desc->set_face( 3, 3, 4, 5, -1);
        desc->set_face( 4, 1, 2, 5, -1);
        desc->set_face( 5, 1, 5, 4, -1);
        desc->set_tetra(0, 0 ,1 , 2, 5);
        desc->set_tetra(1, 1, 3, 4, 5);
        desc->set_tetra(2, 3, 1, 0, 5);
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 0,3);
        desc->set_edge( 4, 1,4);
        desc->set_edge( 5, 2,5);
        desc->set_edge( 6, 3,4);
        desc->set_edge( 7, 4,5);
        desc->set_edge( 8, 5,3);
        desc->set_edge( 9, 1,5);

        replace(LocalCellTopology::ANTIWEDGERIGHT6, desc );


        desc = new LocalCellTopology(LocalCellTopology::ANTIWEDGELEFT6);
        desc->set_name("ANTIWEDGELEFT6");
        desc->set_n_nodes( 6);
        desc->set_n_faces( 6);
        desc->set_n_edges( 10);

        desc->set_n_tetras(3);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4, 5 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3 ,4,5);

        desc->set_face( 0, 0, 2, 1, -1 );
        desc->set_face( 1, 0, 3, 5, 2  );
        desc->set_face( 2, 0, 1, 4, 3  );
        desc->set_face( 3, 3, 4, 5, -1 );
        desc->set_face( 4, 1, 2, 4, -1 );
        desc->set_face( 5, 2, 5, 4, -1 );
        desc->set_tetra(0, 0 ,1 , 2, 4);
        desc->set_tetra(1, 2, 3, 4, 5 );
        desc->set_tetra(2, 3, 4, 0, 5 );
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 0,3);
        desc->set_edge( 4, 1,4);
        desc->set_edge( 5, 2,5);
        desc->set_edge( 6, 3,4);
        desc->set_edge( 7, 4,5);
        desc->set_edge( 8, 5,3);
        desc->set_edge( 9, 2,4);

        replace(LocalCellTopology::ANTIWEDGELEFT6, desc );


        desc = new LocalCellTopology(LocalCellTopology::PYRAMID5);
        desc->set_name("PYRAMID5");
        desc->set_n_nodes( 5);
        desc->set_n_faces( 5);
        desc->set_n_edges( 8);

        desc->set_n_tetras(2);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3, 4 );

        desc->set_face( 0, 0, 3, 2, 1  );
        desc->set_face( 1, 0, 4, 3, -1 );
        desc->set_face( 2, 0, 1, 4, -1 );
        desc->set_face( 3, 1, 2, 4, -1 );
        desc->set_face( 4, 2, 3, 4, -1 );

        desc->set_tetra( 0, 0, 1, 3, 4 );
        desc->set_tetra( 1, 1, 3, 2, 4 );
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,3);
        desc->set_edge( 3, 3,0);
        desc->set_edge( 4, 0,4);
        desc->set_edge( 5, 1,4);
        desc->set_edge( 6, 2,4);
        desc->set_edge( 7, 3,4);

        replace(LocalCellTopology::PYRAMID5, desc );


        desc = new LocalCellTopology(LocalCellTopology::DITETRA5);
        desc->set_name("DITETRA5");
        desc->set_n_nodes( 5);
        desc->set_n_faces( 6);
        desc->set_n_edges( 9);

        desc->set_n_tetras(2);

        desc->set_node( LocalCellTopology::GocadOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::AsterOrder,  0, 1, 2, 3, 4 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3 ,4);

        desc->set_face( 0, 0, 1, 3, -1 );
        desc->set_face( 1, 1, 2, 3, -1 );
        desc->set_face( 2, 2, 0, 3, -1 );
        desc->set_face( 3, 1, 0, 4, -1 );
        desc->set_face( 4, 2, 1, 4, -1 );
        desc->set_face( 5, 0, 2, 4, -1 );


        desc->set_tetra(0,0,1,2,3);
        desc->set_tetra(1,0,1,2,4);
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 0,3);
        desc->set_edge( 4, 1,3);
        desc->set_edge( 5, 2,3);
        desc->set_edge( 6, 0,4);
        desc->set_edge( 7, 1,4);
        desc->set_edge( 8, 2,4);

        replace(LocalCellTopology::DITETRA5, desc);

        desc = new LocalCellTopology(LocalCellTopology::TETRAEDRON4);
        desc->set_name("TETRAEDRON4");
        desc->set_n_nodes( 4);
        desc->set_n_faces( 4);
        desc->set_n_edges( 6);

        desc->set_n_tetras( 1);
        desc->set_tetra(0, 0, 1, 2, 3 );

        desc->set_node( LocalCellTopology::GocadOrder, 0, 1, 2, 3 );
        desc->set_node( LocalCellTopology::ArcTemOrder, 0, 1, 2, 3 );
        desc->set_node( LocalCellTopology::AsterOrder, 0, 1, 2, 3 );
        desc->set_node( LocalCellTopology::ArcTemOrderTopToBase,0, 1, 2, 3 );

        desc->set_face( 0, 0, 2, 1, -1 );
        desc->set_face( 1, 0, 3, 2, -1 );
        desc->set_face( 2, 0, 1, 3, -1 );
        desc->set_face( 3, 1, 2, 3, -1 );

        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);
        desc->set_edge( 3, 0,3);
        desc->set_edge( 4, 1,3);
        desc->set_edge( 5, 2,3);

        replace(LocalCellTopology::TETRAEDRON4, desc);


        desc = new LocalCellTopology(LocalCellTopology::EDGE2);
        desc->set_name("EDGE2");
        desc->set_n_nodes( 2);
        desc->set_n_faces( 0);
        desc->set_n_edges( 1);

        desc->set_n_tetras( 0);
        desc->set_node( LocalCellTopology::GocadOrder, 0, 1 );
        desc->set_edge( 0, 0,1);

        replace(LocalCellTopology::EDGE2, desc);

        desc = new LocalCellTopology(LocalCellTopology::TRIA3);
        desc->set_name("TRIA3");
        desc->set_n_nodes( 3);
        desc->set_n_faces( 1);
        desc->set_n_edges( 3);

        desc->set_n_tetras( 0);
        desc->set_node( LocalCellTopology::GocadOrder, 0, 1, 2 );
        desc->set_face( 0, 0, 1, 2, -1 );
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,0);

        replace(LocalCellTopology::TRIA3, desc);

        desc = new LocalCellTopology(LocalCellTopology::QUAD4);
        desc->set_name("QUAD4");
        desc->set_n_nodes( 4);
        desc->set_n_faces( 1);
        desc->set_n_edges( 4);

        desc->set_n_tetras( 0);
        desc->set_node( LocalCellTopology::GocadOrder, 0, 1, 2, 3 );
        desc->set_face( 0, 0, 1, 2, 3 );
        desc->set_edge( 0, 0,1);
        desc->set_edge( 1, 1,2);
        desc->set_edge( 2, 2,3);
        desc->set_edge( 2, 3,0);

        replace(LocalCellTopology::QUAD4, desc);



    }

    /***************************************************************/
    LocalCellTopologyManager LocalCellTopology::manager_;

    LocalCellTopology::LocalCellTopology( LocalCellTopology::Type type )
        : type_( type ), n_nodes_(0), n_edges_(0), n_faces_(0), n_tetras_(0) {}

    LocalCellTopology* LocalCellTopology::get_desc( LocalCellTopology::Type type ){
        return manager_[type];
    }

    void LocalCellTopology::set_face( int face, int i1, int i2, int i3, int i4 ){
        face_indices_[face*4] = i1;
        face_indices_[face*4+1] = i2;
        face_indices_[face*4+2] = i3;
        face_indices_[face*4+3] = i4;
    }

    void LocalCellTopology::set_edge( int edge, int i1, int i2 ){
        edge_indices_[edge*2] = i1;
        edge_indices_[edge*2+1] = i2;
    }


    void LocalCellTopology::set_node( int order, ... ){

        NodeTab& tab = node_indices_[order];

        va_list vl;
        va_start( vl, order );

        for( int i=0; i<n_nodes_; i++ ){
            int node =va_arg(vl, int);
            tab[i] = node;
        }
        va_end( vl );

    }



    void LocalCellTopology::set_tetra( int tetra, int i1, int i2, int i3, int i4 ){
        tetra_indices_[tetra*4] = i1;
        tetra_indices_[tetra*4+1] = i2;
        tetra_indices_[tetra*4+2] = i3;
        tetra_indices_[tetra*4+3] = i4;
    }

}

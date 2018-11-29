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
 * $Id: local_cell_topology.h 242816 2012-08-30 08:06:14Z lecomte $
 */

#ifndef LOCAL_CELL_TOPOLOGY_H_
#define LOCAL_CELL_TOPOLOGY_H_

#include <q3D/mesh/mesh_global.h>

#include <QtCore/QVector>
#include <QtCore/QHash>
#include <QtCore/QString>

namespace Q3D {

    typedef QVector<int> NodeTab;

    // if you change the enum below, upgrade the array of QString accordingly
static QString cell_type_name[14]={
    "UNDEFINED_CELL",
    "Edge2",
    "Tria3",
    "Quad4",
    "Hexaedron8",
    "HemiHexa7",
    "Pentaedron6",
    "HemiHexa6",
    "HemiHexa5",
    "AntiWedgeRight6",
    "AntiWedgeLeft6",
    "Pyramid5",
    "DiTetra5",
    "Tetra4"
};

    /***************************************************************/
    class LocalCellTopology;

    class  MESHSHARED_EXPORT LocalCellTopologyManager : public QVector<LocalCellTopology*> {
    public:
        LocalCellTopologyManager();
    };

    /***************************************************************/
    class  MESHSHARED_EXPORT LocalCellTopology {
    public:

        enum NodesOrder {
            GocadOrder,
            AsterOrder,
            ArcTemOrder,
            ArcTemOrderTopToBase,
            LastOrder
        };

        enum Type {
            UNDEFINED_CELL=0,
            EDGE2,
            TRIA3,
            QUAD4,
            HEXAEDRON8,
            HEMIHEXA7,
            PENTAEDRON6,
            HEMIHEXA6,
            HEMIHEXA5,
            ANTIWEDGERIGHT6,
            ANTIWEDGELEFT6,
            PYRAMID5,
            DITETRA5,
            TETRAEDRON4,
            LAST_CELL_TYPE
        };

    public:
        static LocalCellTopology* get_desc( LocalCellTopology::Type type );

    public:

        Type get_type() const;

        int get_n_nodes() const;
        int get_n_edges() const;
        int get_n_faces() const;
        int get_n_tetras() const ;
        const QString & get_name();

        NodeTab& get_nodes_order( int order );
        int get_face_indice( int face, int index );
        int get_edge_indice( int edge, int index );
        int get_tetra_indice( int tetra, int index );


    private:
        LocalCellTopology( Type type );
        void set_name(const char *);
        void set_n_nodes( int nnode );
        void set_n_faces( int nface );
        void set_n_edges( int nedge );
        void set_n_tetras( int  n);

        void set_face( int face, int i1, int i2, int i3, int i4 );
        void set_edge( int edge, int i1, int i2 );
        void set_tetra( int n, int i1 , int i2 , int i3,int i4);
        void set_node( int order, ... );


    private:
        static LocalCellTopologyManager manager_;

    private:
        Type type_;
        QString name_;
        int n_nodes_;
        int n_edges_;
        int n_faces_;

        int n_tetras_;

        QHash<int, NodeTab> node_indices_;

        QVector<int> face_indices_;
        QVector<int> edge_indices_;
        QVector<int> tetra_indices_;

        friend class LocalCellTopologyManager;
    };

    inline LocalCellTopology::Type LocalCellTopology::get_type() const {
        return type_;
    }

    inline int LocalCellTopology::get_n_nodes() const {
        return n_nodes_;
    }

    inline int LocalCellTopology::get_n_edges() const {
        return n_edges_;
    }
    inline int LocalCellTopology::get_n_faces() const {
        return n_faces_;
    }
    inline int LocalCellTopology::get_n_tetras() const {
        return n_tetras_;
    }

    inline NodeTab& LocalCellTopology::get_nodes_order( int order ){
        return node_indices_[order];
    }

    inline int LocalCellTopology::get_face_indice( int face, int index ){
        return face_indices_[face*4 + index];
    }

    inline int LocalCellTopology::get_edge_indice( int edge, int index ){
        return edge_indices_[edge*2 + index];
    }

    inline int LocalCellTopology::get_tetra_indice( int tetra, int index ){
        return tetra_indices_[tetra*4 + index];
    }

    inline void LocalCellTopology::set_n_faces( int nface ){
        n_faces_ = nface;
        if ( n_faces_ > 0 ){
            face_indices_.resize( 4*n_faces_ );
        }
    }

    inline void LocalCellTopology::set_n_tetras( int n ){
        n_tetras_ = n;
        if ( n > 0 ){
            tetra_indices_.resize( 4*n );
        }
    }

    inline void LocalCellTopology::set_n_edges( int nedge ){
        n_edges_ = nedge;
        if ( n_edges_> 0 ){
            edge_indices_.resize( 2*n_edges_ );
        }
    }

    inline void LocalCellTopology::set_n_nodes( int nnode ){
        n_nodes_ = nnode;
        if ( n_nodes_> 0 ){
            for( int i=GocadOrder; i<LastOrder; ++i){
                node_indices_[i].resize( n_nodes_ );
            }
        }
    }
    inline void LocalCellTopology::set_name(const char * t){
        name_ = t;
    }

    inline const QString &  LocalCellTopology::get_name(){
        return name_;
    }
}

#endif /* LOCAL_CELL_TOPOLOGY_H_ */

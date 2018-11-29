#ifndef ELEMENT_H
#define ELEMENT_H

#include <q3D/mesh/mesh_global.h>

#include <QtCore/QVector>

#include <q3D/mesh/types.h>
#include <q3D/mesh/local_cell_topology.h>

namespace Q3D {

/**********************************************************/

class MESHSHARED_EXPORT ICell : public QVector<corner_id> {
public:

    LocalCellTopology::Type type() const { return type_; }

    void set_id( quint64 ce_id );
    quint64 get_id() const;

    void get_sorted( QVector<corner_id>& corners ) const;

protected:
    ICell( LocalCellTopology::Type type, int size )
        : QVector<corner_id>( size ),
          type_(type)
    {
    }

private:
    LocalCellTopology::Type type_;
    quint64 id_;
};

/**********************************************************/

class MESHSHARED_EXPORT Edge : public ICell {
public:
    Edge( corner_id i1, corner_id i2 );
};

MESHSHARED_EXPORT uint qHash(const Edge& f);

/*******************************************************************/
class MESHSHARED_EXPORT Facet : public ICell {
public:
    Facet();
    Facet( corner_id id1, corner_id id2, corner_id id3 );
    Facet( corner_id id1, corner_id id2, corner_id id3, corner_id id4 );
    Facet( const CornerList& corners );
    Facet( const CornerSet& corners );

    void set_face_id( face_id face );
    face_id get_face_id() const;

};

MESHSHARED_EXPORT uint qHash(const Facet& f);

inline void Facet::set_face_id( face_id face ){
    set_id( face );
}

inline face_id Facet::get_face_id() const {
    return (face_id)get_id();
}

/**********************************************************/
class MESHSHARED_EXPORT  Triangle : public Facet {
public:
    Triangle( corner_id i1, corner_id i2, corner_id i3 );
};

/**********************************************************/
class MESHSHARED_EXPORT  Quad : public Facet {
public:
    Quad( corner_id i1, corner_id i2, corner_id i3, corner_id i4 );
};


/**********************************************************/
class MESHSHARED_EXPORT  Tetra : public ICell {
public:
    Tetra( corner_id i1, corner_id i2, corner_id i3, corner_id i4 );


};






}


#endif // ELEMENT_H

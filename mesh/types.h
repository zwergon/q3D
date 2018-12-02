#ifndef MESH_TYPE_H
#define MESH_TYPE_H

#include <q3D/mesh/mesh_global.h>

#include <QtCore/QSet>
#include <QtCore/QList>

#include <q3D/model/point3d.h>

namespace Q3D {

typedef qint64  corner_id; /**< type used for node indices */
typedef qint64  face_id; /**< type used for face indices */
typedef qint64  cell_id; /**< type used for cell indices */

typedef QSet<corner_id>  CornerSet; /**< type used for node set (unordered)*/
typedef QList<corner_id> CornerList; /**< type used for node list */
typedef QSet<face_id> FaceSet; /**< type used for face set (unordered)*/
typedef QSet<cell_id> CellSet; /**< type used for cell set (unordered)*/


class MESHSHARED_EXPORT Element {
public:

    typedef enum Type_ {
        UNKNOWN_ELEMENT = -1,
        CELL = 0,
        FACE,
        EDGE,
        NODE,
        ELEMENT_LAST_TYPE
    } Type;

    static const char* name( Type type ){
        switch(type){
        case CELL:
            return "Cell";
        case NODE:
            return "Node";
        case FACE:
            return "Face";
        case EDGE:
            return "Edge";
        default:
            return "Unknown";
        }
    }

};

}

#endif // MESH_TYPE_H

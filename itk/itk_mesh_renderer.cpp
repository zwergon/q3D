#include "itk_mesh_renderer.h"

#include <QtOpenGL>
#include <QDebug>

#include <q3D/itk/itk_mesh_model.h>
#include <q3D/itk/itk_mesh_renderer_attribute.h>

namespace Q3D {

ItkMeshRenderer::ItkMeshRenderer()
{
}

RendererAttribute* ItkMeshRenderer::createAttribute(){
    return new ItkMeshRendererAttribute(this);
}

void ItkMeshRenderer::drawEdges( ItkMeshModel* mesh_model ) const {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1.0);

    ItkMeshRendererAttribute* attr = static_cast<ItkMeshRendererAttribute*>(attribute());
    const QColor& grid_color = attr->gridColor();
    glColor3f( grid_color.redF(), grid_color.greenF(), grid_color.blueF() );


    MeshType::Pointer mesh = mesh_model->getMesh();

    if ( mesh->GetNumberOfCells() == 0 ){
        qDebug() << "no cells in this mesh";
        return;
    }

    for( CellIterator it_cell = mesh->GetCells()->Begin();
         it_cell != mesh->GetCells()->End();
         ++it_cell ){
        CellType* cell = it_cell.Value();

        PointIdIterator it_point = cell->PointIdsBegin();

        MeshType::PointIdentifier start = *it_point;

        MeshType::PointIdentifier previous = start;
        ++it_point;
        for( ; it_point != cell->PointIdsEnd(); ++it_point ){

            MeshType::PointIdentifier current = *it_point;

            MeshType::PointType pt1 = mesh_model->transformed(mesh->GetPoint( previous ));
            MeshType::PointType pt2 = mesh_model->transformed(mesh->GetPoint( current ));

            glBegin(GL_LINES);
            glVertex3d(pt1[0], pt1[1], pt1[2]);
            glVertex3d(pt2[0], pt2[1], pt2[2]);
            glEnd();
        }
    }
}


void ItkMeshRenderer::drawSurface( ItkMeshModel* mesh_model ) const{
    glLineWidth(1.0);

    ItkMeshRendererAttribute* attr = static_cast<ItkMeshRendererAttribute*>(attribute());
    const QColor& face_color = attr->faceColor();

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f( face_color.redF(), face_color.greenF(), face_color.blueF(), face_color.alphaF() );


   MeshType::Pointer mesh = mesh_model->getMesh();

    if ( mesh->GetNumberOfCells() == 0 ){
        qDebug() << "no cells in this mesh";
        return;
    }

    for( CellIterator it_cell = mesh->GetCells()->Begin();
         it_cell != mesh->GetCells()->End();
         ++it_cell ){
        CellType* cell = it_cell.Value();

        switch ( cell->GetNumberOfPoints() ){
        case 3:
        {
            auto id_container = cell->GetPointIdsContainer();

            MeshType::PointType pts[3];
            for( int i=0; i<3; i ++ ){
                pts[i] = mesh_model->transformed(mesh->GetPoint(id_container[i]));
            }
            Point3d u = { pts[1][0] - pts[0][0], pts[1][1] - pts[0][1], pts[1][2] - pts[0][2] };
            Point3d v = { pts[2][0] - pts[0][0], pts[2][1] - pts[0][1], pts[2][2] - pts[0][2] };
            Point3d uv = cross<double>(u, v);

            glNormal3d( uv[0], uv[1], uv[2] );
            glBegin(GL_TRIANGLES);
            glVertex3d(pts[0][0], pts[0][1], pts[0][2]);
            glVertex3d(pts[1][0], pts[1][1], pts[1][2]);
            glVertex3d(pts[2][0], pts[2][1], pts[2][2]);
            glEnd();
            break;
        }
        default:
            break;
        }
    }

    glDisable( GL_BLEND );

}


void ItkMeshRenderer::update(RendererArea *){
    ItkMeshModel* mmodel = dynamic_cast<ItkMeshModel*>( model() );
    if ( 0 == mmodel ){
        return;
    }

    ItkMeshRendererAttribute* attr = static_cast<ItkMeshRendererAttribute*>(attribute());

    if (glIsList(gl_list_))
        glDeleteLists(gl_list_,1);

    gl_list_=glGenLists(1);

    glNewList( gl_list_, GL_COMPILE );

    if ( attr->withFaces() ){
        glPolygonOffset( 1.0, 1.0 );

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawSurface( mmodel );
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if ( attr->withGrid() ) {
        drawEdges( mmodel );
    }

    if ( attr->withNodes() ){
        //drawNodes( mmodel );
    }

    glEndList();
}

}

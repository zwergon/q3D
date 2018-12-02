/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Francois
 *
 * $Id: Mesh_renderer.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <QtOpenGL>
#include <QMenu>

#include "mesh_renderer.h"

#include <q3D/model/gl_data.h>
#include <q3D/drivers/mesh/mesh_renderer_attribute.h>

#include "mesh_model.h"


namespace Q3D {


MeshRenderer::MeshRenderer()
: ModelRenderer()
{
    setObjectName( "Mesh Renderer" );
}

void MeshRenderer::init(){

}

RendererAttribute* MeshRenderer::createAttribute() {
    return new MeshRendererAttribute(this);
}

void MeshRenderer::drawNodes( MeshModel* mesh_model ){

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());
    const QColor& node_color = attr->nodeColor();

    glColor3f( node_color.redF(), node_color.greenF(), node_color.blueF() );
    glPointSize(6.0);
    const Mesh& mesh = mesh_model->mesh();

    QList<corner_id> corners;
    mesh.get_all_corners( corners );

    glBegin(GL_POINTS);
    QListIterator<corner_id> it_co( corners );
    while( it_co.hasNext()) {
        const Point3d& pt = mesh.get_point( it_co.next() );
        glVertex3f(pt[0], pt[1], pt[2] );
    }
    glEnd();

}

void MeshRenderer::drawSurface( MeshModel* mesh_model ){
    glLineWidth(1.0);

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());
    const QColor& face_color = attr->faceColor();

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f( face_color.redF(), face_color.greenF(), face_color.blueF(), face_color.alphaF() );


    Mesh& mesh = mesh_model->mesh();

    QList<cell_id> cells;
    mesh.get_all_cells( cells );
    QListIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext()) {
        cell_id ce_id = it_ce.next();

        const ICell& cell = mesh.get_cell( ce_id );
        LocalCellTopology::Type type = cell.type();
        LocalCellTopology* cell_topo = LocalCellTopology::get_desc( type );

        for( int i = 0; i< cell_topo->get_n_faces(); i++ ){
            corner_id co_id1 = cell[ cell_topo->get_face_indice(i, 0) ];
            corner_id co_id2 = cell[ cell_topo->get_face_indice(i, 1) ];
            corner_id co_id3 = cell[ cell_topo->get_face_indice(i, 2) ];
            const Point3d& pt1 = mesh.get_point( co_id1 );
            const Point3d& pt2 = mesh.get_point( co_id2 );
            const Point3d& pt3 = mesh.get_point( co_id3 );

            Point3d u = { pt2[0] - pt1[0], pt2[1] - pt1[1], pt2[2] - pt1[2] };
            Point3d v = { pt3[0] - pt1[0], pt3[1] - pt1[1], pt3[2] - pt1[2] };
            Point3d uv = cross<double>(u, v);

            glNormal3d( uv[0], uv[1], uv[2] );

            int i4 = cell_topo->get_face_indice(i, 3);
            if ( i4 == -1 ){
                glBegin(GL_TRIANGLES);
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                glEnd();
            }
            else {
                corner_id co_id4 = cell[ cell_topo->get_face_indice(i, 3) ];
                const Point3d& pt4 = mesh.get_point( co_id4 );
                glBegin(GL_QUADS);
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                glVertex3d(pt4[0], pt4[1], pt4[2]);
                glEnd();
            }
        }
    }
    glDisable( GL_BLEND );

}

void MeshRenderer::setColor( const Property& prop, quint64 id, const QColor& default_color  ){

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());
    double opacity = attr->opacity();
    QVector<double> value;
    prop.get_value( id, value );
    if ( value[0] != std::numeric_limits<double>::infinity() ){
        GlColor3uv color;
        colormap()->getGlColor( value[0], color );
        glColor4ub( color[0], color[1], color[2], opacity*255 );
    }
    else {
        glColor4f(  default_color.redF(),
                    default_color.greenF(),
                    default_color.blueF(),
                    opacity );
    }

}

void MeshRenderer::drawPaintedCellSurface( MeshModel* mesh_model ){
    glLineWidth(1.0);

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());


    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    const QColor& face_color = attr->faceColor();
    glColor3f( face_color.redF(), face_color.greenF(), face_color.blueF() );


    Mesh& mesh = mesh_model->mesh();
    const Property& prop = attr->selectedProperty();

    QList<cell_id> cells;
    mesh.get_all_cells( cells );
    QListIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext()) {
        cell_id ce_id = it_ce.next();


        setColor( prop, ce_id, face_color );


        const ICell& cell = mesh.get_cell( ce_id );
        LocalCellTopology::Type type = cell.type();
        LocalCellTopology* cell_topo = LocalCellTopology::get_desc( type );

        for( int i = 0; i< cell_topo->get_n_faces(); i++ ){
            corner_id co_id1 = cell[ cell_topo->get_face_indice(i, 0) ];
            corner_id co_id2 = cell[ cell_topo->get_face_indice(i, 1) ];
            corner_id co_id3 = cell[ cell_topo->get_face_indice(i, 2) ];
            const Point3d& pt1 = mesh.get_point( co_id1 );
            const Point3d& pt2 = mesh.get_point( co_id2 );
            const Point3d& pt3 = mesh.get_point( co_id3 );

            Point3d u = { pt2[0] - pt1[0], pt2[1] - pt1[1], pt2[2] - pt1[2] };
            Point3d v = { pt3[0] - pt1[0], pt3[1] - pt1[1], pt3[2] - pt1[2] };
            Point3d uv = cross<double>(u, v);

            glNormal3d( uv[0], uv[1], uv[2] );

            int i4 = cell_topo->get_face_indice(i, 3);
            if ( i4 == -1 ){
                glBegin(GL_TRIANGLES);
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                glEnd();
            }
            else {
                corner_id co_id4 = cell[ cell_topo->get_face_indice(i, 3) ];
                const Point3d& pt4 = mesh.get_point( co_id4 );
                glBegin(GL_QUADS);
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                glVertex3d(pt4[0], pt4[1], pt4[2]);
                glEnd();
            }
        }
    }

    glDisable( GL_BLEND );

}

void MeshRenderer::drawPaintedNodeSurface( MeshModel* mesh_model ){
    glLineWidth(1.0);

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());

    const QColor& face_color = attr->faceColor();
    glColor3f( face_color.redF(), face_color.greenF(), face_color.blueF() );


    Mesh& mesh = mesh_model->mesh();
    const Property& prop = attr->selectedProperty();

    QList<cell_id> cells;
    mesh.get_all_cells( cells );
    QListIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext()) {
        cell_id ce_id = it_ce.next();

        const ICell& cell = mesh.get_cell( ce_id );
        LocalCellTopology::Type type = cell.type();
        LocalCellTopology* cell_topo = LocalCellTopology::get_desc( type );

        for( int i = 0; i< cell_topo->get_n_faces(); i++ ){
            corner_id co_id1 = cell[ cell_topo->get_face_indice(i, 0) ];
            corner_id co_id2 = cell[ cell_topo->get_face_indice(i, 1) ];
            corner_id co_id3 = cell[ cell_topo->get_face_indice(i, 2) ];
            const Point3d& pt1 = mesh.get_point( co_id1 );
            const Point3d& pt2 = mesh.get_point( co_id2 );
            const Point3d& pt3 = mesh.get_point( co_id3 );

            Point3d u = { pt2[0] - pt1[0], pt2[1] - pt1[1], pt2[2] - pt1[2] };
            Point3d v = { pt3[0] - pt1[0], pt3[1] - pt1[1], pt3[2] - pt1[2] };
            Point3d uv = cross<double>(u, v);

            glNormal3d( uv[0], uv[1], uv[2] );

            int i4 = cell_topo->get_face_indice(i, 3);
            if ( i4 == -1 ){
                glBegin(GL_TRIANGLES);
                setColor( prop, co_id1, face_color );
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                setColor( prop, co_id2, face_color );
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                setColor( prop, co_id3, face_color );
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                glEnd();
            }
            else {
                corner_id co_id4 = cell[ cell_topo->get_face_indice(i, 3) ];
                const Point3d& pt4 = mesh.get_point( co_id4 );
                glBegin(GL_QUADS);
                setColor( prop, co_id1, face_color );
                glVertex3d(pt1[0], pt1[1], pt1[2]);
                setColor( prop, co_id2, face_color );
                glVertex3d(pt2[0], pt2[1], pt2[2]);
                setColor( prop, co_id3, face_color );
                glVertex3d(pt3[0], pt3[1], pt3[2]);
                setColor( prop, co_id4, face_color );
                glVertex3d(pt4[0], pt4[1], pt4[2]);
                glEnd();
            }
        }
    }
}


void MeshRenderer::drawEdges( MeshModel* mesh_model ){

    glLineWidth(1.0);

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());
    const QColor& grid_color = attr->gridColor();
    glColor3f( grid_color.redF(), grid_color.greenF(), grid_color.blueF() );


    const Mesh& mesh = mesh_model->mesh();

    QList<cell_id> cells;
    mesh.get_all_cells( cells );
    QListIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext()) {
        cell_id ce_id = it_ce.next();
        const ICell& cell = mesh.get_cell( ce_id );
        LocalCellTopology::Type type = cell.type();
        LocalCellTopology* cell_topo = LocalCellTopology::get_desc( type );

        for( int i = 0; i< cell_topo->get_n_edges(); i++ ){
            corner_id co_id1 = cell[ cell_topo->get_edge_indice(i, 0) ];
            corner_id co_id2 = cell[ cell_topo->get_edge_indice(i, 1) ];
            const Point3d& pt1 = mesh.get_point( co_id1 );
            const Point3d& pt2 = mesh.get_point( co_id2 );

            glBegin(GL_LINES);
            glVertex3d(pt1[0], pt1[1], pt1[2]);
            glVertex3d(pt2[0], pt2[1], pt2[2]);
            glEnd();
        }
    }
}


void
MeshRenderer::buildGlList()
{
    MeshModel* mmodel = dynamic_cast<MeshModel*>( model() );
    if ( 0 == mmodel ){
        return;
    }

    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>(attribute());

    if (glIsList(gl_list_))
        glDeleteLists(gl_list_,1);

    gl_list_=glGenLists(1);

    glNewList( gl_list_, GL_COMPILE );


    if ( attr->withFaces() ){
        glPolygonOffset( 1.0, 1.0 );

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if ( attr->painted() && (attr->selectedProperty() != Property::null) ){
            if ( attr->selectedProperty().type() == Element::CELL ){
                drawPaintedCellSurface( mmodel );
            }
            else {
                drawPaintedNodeSurface( mmodel );
            }
        }
        else {
            drawSurface( mmodel );
        }
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if ( attr->withGrid() ) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawEdges( mmodel );
    }

    if ( attr->withNodes() ){
        drawNodes( mmodel );
    }

    glEndList();

}

}


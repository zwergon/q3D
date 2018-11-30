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
 * $Id: cpgmesh_renderer.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <QtOpenGL>
#include <QMenu>

#include <q3D/model/gl_data.h>
#include <q3D/model/model.h>
#include <q3D/drivers/drivers_renderer_attribute.h>

#include <wykobi/wykobi.hpp>

#include "cpgtopo.h"
#include "cpgmesh.h"
#include "cpgmesh_renderer.h"

namespace Q3D {

using namespace wykobi;

CpgMeshRenderer::CpgMeshRenderer()
: ModelRenderer() 
{
	setObjectName( "Cpg Mesh Renderer" );
}

RendererAttribute* CpgMeshRenderer::createAttribute(){
    return new DriversRendererAttribute(this);
}

void
CpgMeshRenderer::drawSurface( CpgMesh* cpgmesh )
{
	CpgTopo* topo = cpgmesh->topo();
    Q_ASSERT( colormap() != 0 );
    ColorMap& colormap_ = *(colormap());

	for ( int i=0; i< topo->nCells(); ++i )
	{
        Point3f& v1 = topo->getVertex( i, 0 );
        Point3f& v2 = topo->getVertex( i, 1 );
        Point3f& v3 = topo->getVertex( i, 2 );
        Point3f& v4 = topo->getVertex( i, 3 );
        Point3f& v5 = topo->getVertex( i, 4 );

        plane<float,3> pl  = make_plane<float>(v1, v2, v3);
        glNormal3f( pl.normal.x, pl.normal.y, pl.normal.z );

		glBegin(GL_TRIANGLE_FAN);

        GlColor3uv color;
        colormap_.getGlColor( v5[2], color );
        glColor3ubv( color );
        glVertex3f( v5[0], v5[1], v5[2] );

        colormap_.getGlColor( v1[2], color );
        glColor3ubv( color );
        glVertex3f( v1[0], v1[1], v1[2] );

        colormap_.getGlColor( v2[2], color );
        glColor3ubv( color );
        glVertex3f( v2[0], v2[1], v2[2] );

        colormap_.getGlColor( v3[2], color );
        glColor3ubv( color );
        glVertex3f( v3[0], v3[1], v3[2] );

        colormap_.getGlColor( v4[2], color );
        glColor3ubv( color );
        glVertex3f( v4[0], v4[1], v4[2] );

        colormap_.getGlColor( v1[2], color );
        glColor3ubv( color  );
        glVertex3f( v1[0], v1[1], v1[2] );


		glEnd();

	}

}

void
CpgMeshRenderer::drawLines( CpgMesh* cpgmesh )
{
    CpgTopo* topo = cpgmesh->topo();

    DriversRendererAttribute* attr = static_cast<DriversRendererAttribute*>(attribute());
    const QColor& grid_color = attr->gridColor();
    glColor3f( grid_color.redF(), grid_color.greenF(), grid_color.blueF() );

    for ( int i=0; i< topo->nCells(); ++i )
    {
        Point3f& v1 = topo->getVertex( i, 0 );
        Point3f& v2 = topo->getVertex( i, 1 );
        Point3f& v3 = topo->getVertex( i, 2 );
        Point3f& v4 = topo->getVertex( i, 3 );

        glBegin(GL_LINE_LOOP);
        glVertex3f( v1[0], v1[1], v1[2] );
        glVertex3f( v2[0], v2[1], v2[2] );
        glVertex3f( v3[0], v3[1], v3[2] );
        glVertex3f( v4[0], v4[1], v4[2] );
        glEnd();

    }

}

void CpgMeshRenderer::init(){
    CpgMesh* cpgmesh = dynamic_cast<CpgMesh*>( model() );
    if ( 0 == cpgmesh ){
        return;
    }

    colormap()->setMinMax( cpgmesh->mini()[2], cpgmesh->maxi()[2] );
}

void
CpgMeshRenderer::buildGlList()
{
	CpgMesh* cpgmesh = dynamic_cast<CpgMesh*>( model() );
	if ( 0 == cpgmesh ){
		return;
	}

    DriversRendererAttribute* attr = static_cast<DriversRendererAttribute*>(attribute());

	if (glIsList(gl_list_))
		glDeleteLists(gl_list_,1);

	gl_list_=glGenLists(1);

	glNewList( gl_list_, GL_COMPILE );

    glPolygonOffset( 1.0, 1.0 );

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawSurface( cpgmesh );
    glDisable(GL_POLYGON_OFFSET_FILL);

    if ( attr->withGrid() ) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawLines( cpgmesh );
    }

	glEndList();

}

}


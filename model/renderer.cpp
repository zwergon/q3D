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
 * $Id: model_renderer.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include "renderer.h"

#include <QtOpenGL>

#include "model.h"
#include "renderer_attribute.h"

namespace Q3D {

ModelRenderer::ModelRenderer()
    : QObject(0),
      gl_list_(-1)
{
    ColorMap* colormap = new ColorMap(this);
    connect( colormap, SIGNAL(colormapChanged(ColorMap*)),
             this, SLOT(colormapChangedSlot(ColorMap*)) );

}

ModelRenderer::~ModelRenderer()
{
  if ( glIsList(gl_list_))
    glDeleteLists(gl_list_,1);
}

 Model* ModelRenderer::model() const {
     return static_cast<Model*>( parent() );
 }

void ModelRenderer::setModel( Model* model ){
	setParent( model );
    RendererAttribute* attr = attribute();
    if ( 0 != attr ){
        delete attr;
    }
    attr = create_attribute();
    if ( 0 != attr ){
        connect( attr, SIGNAL(attributeChanged(RendererAttribute*)),
                 this, SLOT(attributeChangedSlot(RendererAttribute*)) );
    }
}

RendererAttribute* ModelRenderer::attribute() const {
    return static_cast<RendererAttribute*>( findChild<RendererAttribute*>() );
}

RendererAttribute* ModelRenderer::create_attribute(){
    return new RendererAttribute(this);
}

void ModelRenderer::colormapChangedSlot( ColorMap* ){
    update();
    notify();
}

void ModelRenderer::attributeChangedSlot( RendererAttribute* ){
    update();
    notify();
}

void ModelRenderer::update(){
   buildGlList();
}

void ModelRenderer::notify(){
    emit needRedraw( this );
}



}

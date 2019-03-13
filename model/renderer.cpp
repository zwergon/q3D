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
      gl_list_(0)
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
    if ( nullptr != attr ){
        delete attr;
    }
    attr = createAttribute();
    if ( nullptr != attr ){
        connect( attr, SIGNAL(attributeChanged(RendererAttribute*)),
                 this, SLOT(attributeChangedSlot(RendererAttribute*)) );
    }
}

RendererAttribute* ModelRenderer::attribute() const {
    return static_cast<RendererAttribute*>( findChild<RendererAttribute*>() );
}

RendererAttribute* ModelRenderer::createAttribute(){
    return nullptr; //no attribute by default.
}

void ModelRenderer::colormapChangedSlot( ColorMap* ){
    update();
    notify();
}

void ModelRenderer::attributeChangedSlot( RendererAttribute* ){
    update();
    notify();
}


void ModelRenderer::notify(){
    emit needRedraw( this );
}

void ModelRenderer::draw(){
    if (glIsList(gl_list_)){
        glCallList(gl_list_);
    }
}



}

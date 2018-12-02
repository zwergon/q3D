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
 * $Id: model_renderer.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_MODEL_MODEL_RENDERER_H_
#define Q3D_MODEL_MODEL_RENDERER_H_ 1

#include <q3D/model/model_scope.h>

#include <QObject>
#include <QtOpenGL>

#include <q3D/model/colormap.h>

class QMenu;

namespace Q3D {

class Model;
class RendererAttribute;

class MODELSHARED_EXPORT ModelRenderer : public QObject
{
    Q_OBJECT

public:
    ModelRenderer();
    virtual ~ModelRenderer();

    /* Accessor */
    void         setModel( Model* model );
    Model*       model() const;

    RendererAttribute* attribute() const;

    ColorMap* colormap();
   
    GLuint glList() const { return gl_list_; }


    void update();
    void notify();

    /*virtual functions*/
    virtual void init(){}
    virtual void buildGlList() = 0;
    virtual void draw(){}

signals:
    void needRedraw( ModelRenderer* );

protected slots:
    virtual void colormapChangedSlot( ColorMap* );
    virtual void attributeChangedSlot( RendererAttribute* );


protected:
    virtual RendererAttribute* createAttribute();

protected:
    GLuint       gl_list_;
};

inline ColorMap* ModelRenderer::colormap() {
    return findChild<ColorMap*>();
}

}

#endif /* Q3D_MODEL_MODEL_RENDERER_H_ */


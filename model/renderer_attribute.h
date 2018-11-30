#ifndef RENDERER_ATTRIBUTE_H
#define RENDERER_ATTRIBUTE_H

#include <q3D/model/model_scope.h>

#include <QObject>
#include <QColor>

namespace Q3D {

class ModelRenderer;
class RendererAttributeDialog;

class MODELSHARED_EXPORT RendererAttribute : public QObject
{
    Q_OBJECT

public:
    ModelRenderer* renderer() const;
    virtual RendererAttributeDialog* createDialog( QWidget* parent = nullptr );


signals:
    void attributeChanged( RendererAttribute* );


protected:
    explicit RendererAttribute( ModelRenderer *parent = 0);

};


}

#endif // RENDERER_ATTRIBUTE_H

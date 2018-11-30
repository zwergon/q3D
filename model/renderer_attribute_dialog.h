#ifndef RENDERER_ATTRIBUTE_DIALOG_H
#define RENDERER_ATTRIBUTE_DIALOG_H

#include <q3D/model/model_scope.h>

#include <QPointer>
#include <QDialog>

namespace Q3D {

class RendererAttribute;

class MODELSHARED_EXPORT RendererAttributeDialog
{
public:
    virtual QDialog* getDialog();
    RendererAttribute* getAttribute() const;

protected:
    RendererAttributeDialog( RendererAttribute* renderer_attribute );

protected:
    RendererAttribute* renderer_attribute_;
};

inline RendererAttribute* RendererAttributeDialog::getAttribute() const {
    return renderer_attribute_;
}

}
#endif // RENDERER_ATTRIBUTE_DIALOG_H

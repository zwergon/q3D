#ifndef MESH_RENDERER_ATTTRIBUTE_H
#define MESH_RENDERER_ATTTRIBUTE_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/drivers/drivers_renderer_attribute.h>
#include <q3D/mesh/property.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT MeshRendererAttribute : public DriversRendererAttribute
{
    Q_OBJECT
public:
    MeshRendererAttribute( ModelRenderer* parent = 0 );

    bool painted() const;
    void setPainted( bool painted );

    const Property& selectedProperty() const;
    void setSelectedProperty( const Property& property );

    virtual RendererAttributeDialog* createDialog(QWidget* parent = nullptr);

private:
    bool painted_;
    Property selected_property_;

};

inline bool MeshRendererAttribute::painted() const {
    return painted_;
}

inline const Property& MeshRendererAttribute::selectedProperty() const {
    return selected_property_;
}

}

#endif // MESH_RENDERER_ATTTRIBUTE_H

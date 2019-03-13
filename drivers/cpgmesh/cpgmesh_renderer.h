/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Franois
 *
 * $Id: cpgmesh_renderer.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_DRIVERS_CPGMESH_RENDERER_H_
#define Q3D_DRIVERS_CPGMESH_RENDERER_H_ 1

#include <q3D/drivers/drivers_global.h>
#include <q3D/model/renderer.h>

class CpgMesh;

namespace Q3D {


class DRIVERSSHARED_EXPORT CpgMeshRenderer : public ModelRenderer
{
    Q_OBJECT
public:
    CpgMeshRenderer();

    virtual void update();
    virtual void init();

protected:
    virtual RendererAttribute* createAttribute();
   
private:
    void drawSurface ( CpgMesh* cpgmesh );
    void drawLines ( CpgMesh* cpgmesh );
};

}

#endif /* Q3D_DRIVERS_CPGMESH_RENDERER_H_ */


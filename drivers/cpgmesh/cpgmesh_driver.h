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
 * $Id: cpgmesh_driver.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_DRIVERS_CPG_MESH_DRIVER_H_
#define Q3D_DRIVERS_CPG_MESH_DRIVER_H_ 1

#include <QString>

#include <q3D/drivers/drivers_global.h>
#include <q3D/model/model_driver.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT CpgMeshDriver : public ModelDriver
{
 public:
  CpgMeshDriver();

  virtual Model* open( const QString& );
  virtual void save( const Model& model, const QString& filename );
  
};

}

#endif /* Q3D_DRIVERS_CPG_MESH_DRIVER_H_*/

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
 * $Id: cpgmesh.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_DRIVERS_CPGMESH_CPGMESH_H_
#define Q3D_DRIVERS_CPGMESH_CPGMESH_H_ 1

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/model.h>
#include <q3D/drivers/cpgmesh/cpgtopo.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT CpgMesh : public Model
{
  Q_OBJECT

public:
  CpgMesh();

  CpgTopo* topo();
 
  virtual void update();

private:
  CpgTopo  mTopo;
};

inline CpgTopo* CpgMesh::topo() {
	return &mTopo; 
}

}



#endif /* Q3D_DRIVERS_CPGMESH_CPGMESH_H_ */


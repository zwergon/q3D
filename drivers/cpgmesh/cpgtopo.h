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
 * $Id: cpgtopo.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_DRIVERS_CPGMESH_CPG_TOPO_H_
#define Q3D_DRIVERS_CPGMESH_CPG_TOPO_H_ 1

#include <q3D/drivers/drivers_global.h>
#include <q3D/model/model.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT CpgPoints
{
public:
  int          idx;
  Point3f   vertex;
  Point3f   norm;

};

/*

0 ----- 1
|       |
|       |
|       |
3 ----- 2

0 ----- 1
| \   / |
|   4   |
| /   \ |
3 ----- 2

*/

class DRIVERSSHARED_EXPORT CpgTopo
{
public:
    CpgTopo();
    virtual ~CpgTopo();

    void setNCells( const int n_cells );
    int  nPoints() const { return n_points_; }
    int  nCells() const { return (n_points_ / 5); }
    void setCell( int i_cell, Point3f cell[4] );

    Point3f&  getVertex( int cell, int indice /* 0 ...5 */);

protected:
    int         n_points_;
    CpgPoints*  points_;

};

}

#endif /* Q3D_DRIVERS_CPGMESH_CPG_TOPO_H_ */


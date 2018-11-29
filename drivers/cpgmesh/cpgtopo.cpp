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
 * $Id: cpgtopo.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <iostream>

#include "cpgtopo.h"

namespace Q3D {

CpgTopo::CpgTopo()
   : n_points_( 0 ),
     points_( 0 )
{
}

CpgTopo::~CpgTopo()
{
   if ( points_ )
      delete [] points_;
}

void
CpgTopo::setNCells( int n_cells )
{
  n_points_ = 5*n_cells ;   // we have 4 triangles by strati cell

  if ( points_ )
     delete [] points_;
  
  points_ = new CpgPoints[n_points_];

}

void 
CpgTopo::setCell( int i_cell, Point3f cell[4] )
{
    int i;
    CpgPoints* ptr = points_ + 5*i_cell;
    Point3f center;
    
    for ( int node = 0; node < 4; ++node )
	for ( i =0; i<3; ++i )
	{
	    ptr[node].vertex[i] = cell[node][i];
	    if ( 0 == node )
		center[i] = cell[node][i];
	    else
		center[i] += cell[node][i];
	}
    
    for ( i =0; i<3; ++i )
    {
	ptr[4].vertex[i] = center[i] / 4;
    }

}

Point3f & 
CpgTopo::getVertex( int cell, int indice /* 0 .. 4 */)
{
   CpgPoints* ptr = points_ + 5*cell;

   return ptr[indice].vertex;
}

}


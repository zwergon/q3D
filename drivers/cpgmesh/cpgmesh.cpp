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
 * $Id: cpgmesh.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */

#include <QMenu>


#include "cpgtopo.h"
#include "cpgmesh.h"

namespace Q3D {

CpgMeshModel::CpgMeshModel(){
}

void 
CpgMeshModel::update(){

	bool first = true;
	for (int i=0; i< mTopo.nCells() ; i++)
	{
		for (int j=0; j<5; ++j)
		{
            Point3d& vertex = mTopo.getVertex( i, j );
            double x = vertex[0];
            double y = vertex[1];
            double z = vertex[2];

			if ( first )
			{
				min_[0] = x;
				min_[1] = y;
				min_[2] = z;

				max_[0] = x;
				max_[1] = y;
				max_[2] = z;

				first = false ;
			} 
			else {
				if ( x < min_[0] ) 
					min_[0] = x;
				else if ( x > max_[0] )
					max_[0] = x;

				if ( y < min_[1] ) 
					min_[1] = y;
				else if ( y > max_[1] )
					max_[1] = y;

				if ( z < min_[2] ) 
					min_[2] = z;
				else if ( z > max_[2] )
					max_[2] = z;
			}
		}
	}

    Model::update();

}

}



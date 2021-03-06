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
 * $Id: cpgmesh_driver.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "cpgmesh.h"
#include "cpgtopo.h"
#include "cpgmesh_driver.h"
#include "cpgmesh_renderer.h"

namespace Q3D {

CpgMeshDriver::CpgMeshDriver(){
	setDescription("CpgMeshDriver");
    renderer_factory_.registerFactory<CpgMeshRenderer>( "CpgMesh" );
}

bool CpgMeshDriver::canHandle(Model* model) const{
    return dynamic_cast<CpgMeshModel*>(model) != nullptr;
}

ModelOpenInfo* CpgMeshDriver::openInfo() const {
    FileModelOpenInfo* fmoi = new FileModelOpenInfo();
    fmoi->setExtension("stra");
    return fmoi;
}

Model *
CpgMeshDriver::open( const ModelOpenInfo& openInfo )
{
    QString fileName;
    try {
        const FileModelOpenInfo& fmoi = dynamic_cast<const FileModelOpenInfo&>(openInfo);
        fileName = fmoi.fileName();
    }
    catch(bad_cast){
        return nullptr;
    }

	float x, y, z;
	int i, j;
	int n_cells;
	char descr[512];
	
	QFile t( fileName );
	if (!t.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qCritical() << "unable to open " << fileName << endl ;
		return  0;
	}
	QTextStream in(&t);

	in >> descr ;

    if (( in.status() != QTextStream::Ok ) || ( descr != description() ))
    {
        qDebug() << description() << " is not the good driver\n";
        return 0;
    }

	qDebug() << description() << " is the good driver\n";

	//create Core data
    CpgMeshModel* cpgmesh  = new CpgMeshModel;
	cpgmesh->setObjectName( fileName );
	cpgmesh->setDriver( this );

	//fill topological datas
	CpgTopo* topo     = cpgmesh->topo();

	in >> n_cells;
	topo->setNCells( n_cells );

	for (i=0; i< n_cells ; i++)
	{
        Point3d cell[4];
		for (j=0; j<8; ++j)
		{
			in >> x >> y  >> z ;

			if ( j< 4 )
			{
				cell[j][0] = x;
				cell[j][1] = y;
				cell[j][2] = z;
			}
		}

		topo->setCell( i, cell );

	}
	t.close(); 

	cpgmesh->update();
	
	return cpgmesh;
}

void CpgMeshDriver::save( const Model& , const ModelOpenInfo& ){
    qDebug() << "Save for " << description() << " is not yet implemented";
}

}

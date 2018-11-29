/**
 * \file 
 * Copyright (C) 2004-2009 by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Devoitine Herve
 *
 * $Id: property_cube.h 245415 2012-11-28 16:53:07Z farmer $
 */

#ifndef Q3D_PROPERTY_CUBE_H_
#define Q3D_PROPERTY_CUBE_H_

#include <q3D/mesh/mesh_global.h>

#include <q3D/mesh/types.h>

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QVector>

namespace Q3D {


/***************************************************************/
class MESHSHARED_EXPORT PropertyCube
{
public:
	void reserve( int size );

    double value( qint64 id, int offset ) const ;
    QList<qint64> get_all() const;
    void set_value( qint64 id, int offset, double value );


private:
    QHash<qint64, QVector<double> > cube_;
};

} // namespace Q3D

#endif  //Q3D_PROPERTY_CUBE_H_

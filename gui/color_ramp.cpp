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
 * $Id: color_ramp.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <iostream>

#include <QPainter>

#include "color_ramp.h"
#include <q3D/model/colormap.h>

namespace Q3D {

ColorRamp:: ColorRamp( QWidget* parent )
: QFrame ( parent ), 
mpColorMap( 0 ) 
{
	setFrameStyle( QFrame::Panel | QFrame::Raised );
}

void
ColorRamp::paintEvent( QPaintEvent* pEvent )
{

	QFrame::paintEvent( pEvent );
	if ( 0 == mpColorMap )
		return;

	QRect cr = contentsRect();

	QPainter p( this );

	int w = cr.width();
	int h = cr.height();
	float step = (mpColorMap->maxi() - mpColorMap->mini()) / (float)(h-1.);

	float value = mpColorMap->mini();
	for (int i=0; i< h; i++)
	{
        Color3ub c;
		mpColorMap->getGlColor( value, c ) ;
		p.setPen( QColor(c[0], c[1], c[2]) );
		p.drawLine(0, h-i, w, h-i );

		value += step;
	}
}

}


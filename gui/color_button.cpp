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
 * $Id: color_button.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <QPainter>
#include <QtGui/QMouseEvent>
#include <QColorDialog>

#include "color_button.h"

namespace Q3D {

ColorButton::ColorButton( QWidget* parent )
  : QFrame( parent )
{
  setFrameStyle( QFrame::Panel | QFrame::Raised );
}

void
ColorButton::paintEvent( QPaintEvent* pEvent )
{
	
		QFrame::paintEvent( pEvent );
		QPainter painter( this );
		if ( isEnabled() ){
			painter.setBrush( mColor );
		}
		else {
			painter.setBrush( QColor( "gray" ) );
		}
		painter.drawRect( contentsRect() );
		painter.end();
	
}

void
ColorButton::mouseReleaseEvent( QMouseEvent*  )
{
    QColor color;

    color = QColorDialog::getColor (mColor);
    if ( color.isValid() )
    {
        mColor = color;
        repaint();
        emit colorChanged();
    }

}

}


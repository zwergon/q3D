/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Fran�ois
 *
 * $Id: level_dlg.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <QString>
#include <QLabel>
#include <QLineEdit>

#include "level_dlg.h"

namespace Q3D {

CLevelDlg::CLevelDlg(QWidget* parent) 
  : QDialog(parent)
{
	ui_.setupUi( this );
}

CLevelDlg::~CLevelDlg()
{
}

void
CLevelDlg::setMinMax( Point3f* min, Point3f* max )
{
  mMin = (*min)[2];
  mMax = (*max)[2];
  
  ui_.mpZMinLabel->setText( QString("Zmin : %1").arg(mMin) );
  ui_.mpZMaxLabel->setText( QString("Zmax : %1").arg(mMax) );
  
  mLevel = (mMin + mMax) /2.;
  
  ui_.mpIsoLevelLineEdit->setText( QString("%1").arg(mLevel) );
}

void
CLevelDlg::applySlot()
{
  emit levelChanged(  ui_.mpIsoLevelLineEdit->text().toFloat() );
}

}


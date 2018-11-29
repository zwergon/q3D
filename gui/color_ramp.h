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
 * $Id: color_ramp.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_UTILS_COLOR_RAMP_H_
#define Q3D_UTILS_COLOR_RAMP_H_ 1

#include <QFrame>

#include <q3D/gui/gui_scope.h>

namespace Q3D {

class ColorMap;

class GUISHARED_EXPORT ColorRamp : public QFrame
{

public:
  ColorRamp( QWidget* parent=0 );

  void setColorMap( ColorMap* colormap ) { mpColorMap = colormap; }
  
protected:
  ColorMap* mpColorMap;

  virtual void paintEvent( QPaintEvent* pEvent );

};

}

#endif /* Q3D_UTILS_COLOR_RAMP_H_ */


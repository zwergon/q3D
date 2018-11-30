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
 * $Id: color_button.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_UTILS_COLOR_BUTTON_H_
#define Q3D_UTILS_COLOR_BUTTON_H_ 1

#include <QFrame>
#include <QColor>

#include <q3D/gui/gui_global.h>

namespace Q3D {

class GUISHARED_EXPORT ColorButton : public QFrame
{
  Q_OBJECT

public:
  ColorButton( QWidget* parent=0 );

  void          setColor( const QColor color ) { mColor = color; }
  const QColor& color()  { return mColor; }

signals:
  void colorChanged();

protected:
  virtual void paintEvent( QPaintEvent* pEvent );
  virtual void mouseReleaseEvent( QMouseEvent * );

protected:
  QColor mColor;
};

}

#endif /* Q3D_UTILS_COLOR_BUTTON_H_ */


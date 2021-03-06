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
 * $Id: level_dlg.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef _LEVELDLG_IMPL_H
#define _LEVELDLG_IMPL_H 1

#include <q3D/gui/gui_global.h>

#include <QDialog>

#include <q3D/model/model.h>

namespace Ui {
 class CLevelDlgUI;
}

namespace Q3D {

class GUISHARED_EXPORT CLevelDlg :  public QDialog
{
    Q_OBJECT
    
public:
    CLevelDlg( QWidget* parent );

    void setMinMax( const Point3d& min, const Point3d& max );

signals:
    void levelChanged(float level);
    
protected slots:
virtual void applySlot();

protected:
    double mMin;
    double mMax;
    double mLevel;

    Ui::CLevelDlgUI* ui_;

};
}

#endif /* _LEVELDLG_IMPL_H */


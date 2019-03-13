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
 * $Id: colormap_dlg.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_UTILS_COLORMAP_DLG_H_
#define Q3D_UTILS_COLORMAP_DLG_H_ 1

#include <QDialog>

#include <q3D/gui/gui_global.h>
#include <q3D/model/colormap.h>

namespace Ui {
    class ColormapUI;
}

namespace Q3D {

class Model;

class GUISHARED_EXPORT ColorMapDlg : public QDialog
{
    Q_OBJECT

public:
    friend class ColorRamp;

    ColorMapDlg( QWidget* parent = 0 );
    void setModel( Model* model );
    void setOriginalColorMap( ColorMap* colormap );

signals:
    void colormapChanged( Model* );

private slots:
    void on_firstColorButtonW_colorChanged();
    void on_secondColorButtonW_colorChanged();
    void on_maxSliderW_valueChanged(int);
    void on_minSliderW_valueChanged(int);
    void on_applyButtonW_clicked();
	void on_browseButtonW_clicked();
	void on_interpolatedRadioButtonW_toggled( bool );
	void on_randomRadioButtonW_toggled( bool );
    void on_predefinedRadioButtonW_toggled( bool );
    void on_paletteCombo_currentIndexChanged (const QString&);
    void on_alpha_gb__toggled(bool);
    void on_alpha_spin__valueChanged(int);

    void initUI();

private:
    void initCombo( );
    QString load( const QString& fileName );

private:
    Model*    model_;
    ColorMap* original_cmap_;
    ColorMap  cmap_ ;


    Ui::ColormapUI* ui_;

};

inline void ColorMapDlg::setModel( Model* model ){
    model_ = model;
}

}

#endif /* Q3D_UTILS_COLORMAP_DLG_H_ */


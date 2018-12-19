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
 * $Id: colormap_dlg.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */
#include "colormap_dlg.h"
#include "ui_colormap_dlg.h"

#include <iostream>

#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QTextStream>
#include <QSlider>
#include <QColorDialog>
#include <QFileDialog>

#include "color_button.h"
#include "color_ramp.h"
#include <q3D/model/colormap.h>

namespace Q3D {

ColorMapDlg::ColorMapDlg( QWidget* parent )
:	QDialog( parent ),
    model_( nullptr ),
    original_cmap_( nullptr ),
    cmap_( nullptr ), // works on copy of original colormap
    ui_(new Ui::ColormapUI)
{
    ui_->setupUi( this );

    initCombo();

}

void ColorMapDlg::setOriginalColorMap( ColorMap* colormap ){
    if ( nullptr != original_cmap_ ){
        disconnect( original_cmap_, SIGNAL(colormapChanged(ColorMap*)),
                 this, SLOT(initUI()) );
    }

    if ( nullptr != colormap ){
        original_cmap_ = colormap;
        connect( original_cmap_, SIGNAL(colormapChanged(ColorMap*)),
                         this, SLOT(initUI()) );
    }

    initUI();

}

void ColorMapDlg::initUI(){

    if ( nullptr == original_cmap_ ){
        return;
    }


    cmap_.initFrom( *original_cmap_ );

    ui_->paletteCombo->blockSignals(true);
    if ( cmap_.name() != QString:: null ){
        ui_->paletteCombo->setCurrentIndex( ui_->paletteCombo->findText( cmap_.name() ) );
    }
    else {
        ui_->paletteCombo->setCurrentIndex( ui_->paletteCombo->findText( "Default" ) );
    }
    ui_->paletteCombo->blockSignals(false);

    ui_->colorFrameW->setColorMap( &cmap_ );

    ui_->firstColorButtonW->setEnabled(  cmap_.type() == ColorMap::INTERPOLATED  );
    ui_->secondColorButtonW->setEnabled(  cmap_.type() == ColorMap::INTERPOLATED  );
    ui_->secondColorButtonW->setColor( cmap_.getQColor(0) ) ;
    ui_->firstColorButtonW->setColor(  cmap_.getQColor( cmap_.getNColors() - 1 )  ) ;


    float max_slider_range = (float)ui_->maxSliderW->maximum() - (float)ui_->maxSliderW->minimum();
    float min_slider_range = (float)ui_->minSliderW->maximum() - (float)ui_->minSliderW->minimum();
    float data_range = cmap_.maxi() - cmap_.mini();

    int max = (float)ui_->maxSliderW->maximum() -(cmap_.currentMax() - cmap_.mini())*max_slider_range/data_range;
    ui_->maxSliderW->setValue(max);
    ui_->maxLineEditW->setText(QString::number( cmap_.currentMax() ));

    int min = (float)ui_->minSliderW->maximum() -(cmap_.currentMin() - cmap_.mini())*min_slider_range/data_range;
    ui_->minSliderW->setValue( min);
    ui_->minLineEditW->setText(QString::number(cmap_.currentMin() ));
}

void ColorMapDlg::initCombo()
{
    ui_->paletteCombo->clear();
    QList<QString> palettes = ColorMap::palettes().keys();
    qSort( palettes );
    QListIterator<QString> itp(palettes);
    while( itp.hasNext() ){
        ui_->paletteCombo->addItem( itp.next() );
    }



}

void 
ColorMapDlg::on_minSliderW_valueChanged(int value)
{

    if (value <= ui_->maxSliderW->value() )
        ui_->minSliderW->setValue(ui_->maxSliderW->value() + 1);
	else
	{
        float min = ( ui_->minSliderW->maximum() - value) * (cmap_.maxi() - cmap_.mini()) /
            (ui_->minSliderW->maximum() - ui_->minSliderW->minimum()) + cmap_.mini();
        cmap_.setCurrentMin( min );
        ui_->colorFrameW->repaint();

	}

    ui_->minLineEditW->setText(QString::number(cmap_.currentMin()));

}

void 
ColorMapDlg::on_secondColorButtonW_colorChanged()
{
    cmap_.createInterpolated( ui_->firstColorButtonW->color(), ui_->secondColorButtonW->color() );
    ui_->colorFrameW->repaint();

}

void 
ColorMapDlg::on_maxSliderW_valueChanged(int value)
{
    if (value >= ui_->minSliderW->value() )
        ui_->maxSliderW->setValue(ui_->minSliderW->value() - 1);
	else
	{
        float max = (ui_->maxSliderW->maximum() - value) * (cmap_.maxi() - cmap_.mini()) /
            (ui_->maxSliderW->maximum() - ui_->maxSliderW->minimum()) + cmap_.mini() ;
        cmap_.setCurrentMax( max );
        ui_->colorFrameW->repaint();
	}
    ui_->maxLineEditW->setText(QString::number(cmap_.currentMax()));
}

void 
ColorMapDlg::on_firstColorButtonW_colorChanged()
{
    cmap_.createInterpolated( ui_->firstColorButtonW->color(), ui_->secondColorButtonW->color() );
    ui_->colorFrameW->repaint();
}

void 
ColorMapDlg::on_applyButtonW_clicked()
{
    original_cmap_->initFrom( cmap_ );
    emit colormapChanged( model_ );
}

void
ColorMapDlg::on_browseButtonW_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if ( !fileName.isEmpty() ) {  
        QString palette_name = load(fileName);
        if ( !palette_name.isNull() ){
            initCombo();
            ui_->paletteCombo->setCurrentIndex( ui_->paletteCombo->findText( palette_name) );
        }

	}
}

void 
ColorMapDlg::on_interpolatedRadioButtonW_toggled( bool checked )
{
	
    ui_->firstColorButtonW->setEnabled( checked );
    ui_->secondColorButtonW->setEnabled( checked );
	if ( checked ){
        cmap_.createInterpolated( ui_->firstColorButtonW->color(), ui_->secondColorButtonW->color() );
        ui_->colorFrameW->repaint();
	}
}

void 
ColorMapDlg::on_randomRadioButtonW_toggled( bool )
{
    cmap_.createRandomColormap();
    ui_->colorFrameW->repaint();
}

void
ColorMapDlg::on_predefinedRadioButtonW_toggled( bool checked ){
    if ( checked ){
        cmap_.createFromName( ui_->paletteCombo->currentText() );
        ui_->firstColorButtonW->setEnabled( false );
        ui_->secondColorButtonW->setEnabled( false );
        ui_->colorFrameW->repaint();
    }
}

void
ColorMapDlg::on_paletteCombo_currentIndexChanged ( const QString & text ){
    cmap_.createFromName( text );
    ui_->firstColorButtonW->setEnabled( false );
    ui_->secondColorButtonW->setEnabled( false );
    ui_->colorFrameW->repaint();
}

QString
ColorMapDlg::load( const QString& fileName )
{

    QFileInfo finfo(fileName);
    QFile f(fileName);

    QList<unsigned char> values;
    if ( f.open( QIODevice::ReadOnly ) ) {    // file opened successfully
        QTextStream t( &f );        // use a text stream


        while ( !t.atEnd() ){
            float i, r, g, b ;
            t >> i >> r >> g >> b ;
            values.push_back( (unsigned char)i );
            values.push_back( (unsigned char)(255.*r) );
            values.push_back( (unsigned char)(255.*g) );
            values.push_back( (unsigned char)(255.*b) );
        }

        f.close();

        unsigned char* tab = new unsigned char[1+values.count()];
        int i = 0;
        tab[i++] = values.count()/4;
        QListIterator<unsigned char> itv( values );
        while( itv.hasNext() ){
            tab[i++] = itv.next();
        }

        ColorMap::registerPalette( finfo.baseName(), tab );


    }
    else
    {
        QString str = QString("unable to find Color Palette") ;
        QMessageBox::warning( 0, "Load failed", str );
        return QString::null;
    }

    return finfo.baseName();

}



}


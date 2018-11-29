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
 * $Id: colormap.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <iostream>

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

#include "colormap.h"

namespace Q3D {

static unsigned char default_palette[] = {
    5,
    0, 0, 0, 255,
    64, 0, 255, 255,
    128, 0, 255, 0,
    192, 255, 255, 0,
    255, 255, 0, 0
};

static unsigned char rainbow1_palette[] = {
    7,
    0, 255, 0, 0,
    42, 255, 255, 0,
    85, 0, 255, 0,
    128, 0, 255, 255,
    170, 0, 0, 255,
    212, 255, 0, 255,
    255, 255, 255, 255
};

static unsigned char africa_palette[] = {
    4,
    0, 204, 178, 115,
    85, 255, 255, 0,
    170, 0, 255, 0,
    255, 255, 0, 0
};

static unsigned char pastel_palette[] = {
    9,
    0, 251, 180, 174,
    32, 179, 205, 227,
    64, 204, 235, 197,
    96, 222, 203, 228,
    128, 254, 217, 166,
    159, 255, 255, 204,
    191, 229, 216, 189,
    223, 253, 218, 236,
    255, 242, 242, 242
};

StaticPalettes::StaticPalettes(){
    insert("Africa", africa_palette );
    insert("Default", default_palette );
    insert("Rainbow1", rainbow1_palette );
    insert("Pastel", pastel_palette );
}

StaticPalettes ColorMap::palettes_;

const StaticPalettes& ColorMap::palettes(){
    return palettes_;
}

void ColorMap::registerPalette( const QString& name, unsigned char* tab ){
    palettes_.insert( name, tab );
}

ColorMap::ColorMap( QObject* parent )
    : QObject( parent ),
      type_( NAMED ),
      name_("Default"),
      cmin_( 0 ),
      cmax_( 0 )
{
    createFromName( name_ );
    setMinMax( 0, getNColors() );
}

 void ColorMap::initFrom( const ColorMap& cmap ){
     type_ = cmap.type_;
     name_ = cmap.name_;
     min_ = cmap.min_;
     max_ = cmap.max_;
     cmin_ = cmap.cmin_;
     cmax_ = cmap.cmax_;
     colormap_ = cmap.colormap_;

     emit colormapChanged( this );

 }

void
ColorMap::getGlColor(float z, GlColor3uv& color) const
{
	int n_colors = getNColors();

    if (z > cmax_) z = cmax_ ;
    else if (z < cmin_ ) z = cmin_ ;
    int indice = (int)((float)( n_colors - 1. ) *(z - cmin_)/(cmax_-cmin_));
    color[0] = colormap_[3*indice];
    color[1] = colormap_[3*indice+1];
    color[2] = colormap_[3*indice+2];

}

void 
ColorMap::createDefaultColormap()
{
    blockSignals( true );
    createFromName( "Default" );
	setMinMax( 0, getNColors() );
    blockSignals(false);

    emit colormapChanged( this );
}

void ColorMap::createRandomColormap()
{
    type_ = RANDOM;
    colormap_.clear();

	int n_colors = 255;
	for (int i=0; i< n_colors; i++)
	{
        colormap_.push_back((unsigned char)( (double)n_colors*(double)rand()/(double)RAND_MAX ));
        colormap_.push_back((unsigned char)( (double)n_colors*(double)rand()/(double)RAND_MAX ));
        colormap_.push_back((unsigned char)( (double)n_colors*(double)rand()/(double)RAND_MAX ));
	}

    emit colormapChanged( this );
}

void ColorMap::createInterpolated( int i_start, int i_end, const QColor& c1, const QColor& c2 )
{
    int n_colors = i_end - i_start + 1;

    int r = c1.red() ;
    int g = c1.green();
    int b = c1.blue();

    float r_step = (c2.red()   - r) / (float)(n_colors -1.0) ;
    float g_step = (c2.green() - g) / (float)(n_colors -1.0) ;
    float b_step = (c2.blue()  - b) / (float)(n_colors -1.0) ;

    for (int i=0; i<n_colors; i++)
    {
        colormap_.push_back((unsigned char)(r + r_step*i));
        colormap_.push_back((unsigned char)(g + g_step*i));
        colormap_.push_back((unsigned char)(b + b_step*i));
    }

}

void 
ColorMap::createInterpolated( const QColor& c1, const QColor& c2 )
{
    type_ = INTERPOLATED;
    colormap_.clear();

    createInterpolated(0, 255, c1, c2);


     emit colormapChanged( this );
} 

void ColorMap::createFromName( const QString& name ){
    type_ = NAMED;

    if ( palettes_.contains(name) ){

        name_ = name;
        colormap_.clear();

        unsigned char* tab = palettes_[name];

        int n_step = tab[0];
        tab += 1;
        int i_step =0;
        while( i_step < n_step-1 ){
            int i_start = tab[4*i_step];
            int i_end = tab[4*i_step+4];
            QColor c1( tab[4*i_step+1], tab[4*i_step+2], tab[4*i_step+3] );
            QColor c2( tab[4*i_step+5], tab[4*i_step+6], tab[4*i_step+7] );

            createInterpolated( i_start, i_end, c1, c2 );
            i_step++;
        }


         emit colormapChanged( this );


    }
}

void 
ColorMap::setMinMax(float i_min, float i_max)
{
    min_ = cmin_ = i_min ;
    max_ = cmax_ = i_max ;
    emit colormapChanged( this );
}

}

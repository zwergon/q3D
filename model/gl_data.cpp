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
 * $Id: gl_machine.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include "gl_data.h"

#include <QWidget>
#include <QPalette>

namespace Q3D {

GLData* GLData::instance_ = 0;

GLData::GLData()
{

  QColor color = qApp->palette().color(QPalette::Background);
  color_bg_[0] = color.redF() ;
  color_bg_[1] = color.greenF() ;
  color_bg_[2] = color.blueF() ;
//color_bg_[3] = color.alphaF() ;
  color_bg_[3] = 1. ;

  ambient_light_[0] = 1.;
  ambient_light_[1] = 1.;
  ambient_light_[2] = 1.;
  ambient_light_[3] = 1.0;

  specular_light_[0] = 1.0;
  specular_light_[1] = 1.0;
  specular_light_[2] = 1.0;
  specular_light_[3] = 1.0;

  diffusion_light_[0] = 0.6;
  diffusion_light_[1] = 0.6;
  diffusion_light_[2] = 0.6;
  diffusion_light_[3] = 1.0;

  position_light_[0] = 0.0;
  position_light_[1] = 2.0;
  position_light_[2] = -1.0;
  position_light_[3] = 1.0;

  setWithAxis(true);
  setWithLight(false);
  setWithFill(false);
  setWithProf(true);
  setWithSmooth(true);

}

GLData*
GLData::instance(){

    if ( 0 == instance_ ){
        instance_ = new GLData();
    }
  
  return instance_;
}

void
GLData::setWithAxis  ( const bool option )
{
  with_axis_ = option;
}

void 
GLData::setWithFill  ( const bool option )
{
  with_fill_ = option;
}

void  
GLData::setWithLight ( const bool option )
{
  with_light_ = option;
}

void 
GLData::setWithProf  ( const bool option )
{
  with_prof_ = option;

  if (with_prof_)
    {
      clear_mask_ = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ;
    }
  else
    {
      clear_mask_ = GL_COLOR_BUFFER_BIT  ;
    }
}

void 
GLData::setWithSmooth( const bool option )
{
  with_smooth_ = option;
}
  
void
GLData::setPositionLight( GLfloat* light ){
    for( int i = 0; i<4; i++ ){
        position_light_[i] = light[i];
    }
}



}

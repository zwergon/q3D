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

namespace Q3D {

GLData* GLData::instance_ = 0;

GLData::GLData()
{
  mCouleurAP[0] = 0.8 ;
  mCouleurAP[1] = 0.8 ;
  mCouleurAP[2] = 0.8 ;
  mCouleurAP[3] = 1.0 ;

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
  mWithAxis = option;
}

void 
GLData::setWithFill  ( const bool option )
{
  mWithFill = option;
}

void  
GLData::setWithLight ( const bool option )
{
  mWithLight = option;
}

void 
GLData::setWithProf  ( const bool option )
{
  mWithProf = option;

  if (mWithProf)
    {
      mMasqueClear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ;
    }
  else
    {
      mMasqueClear = GL_COLOR_BUFFER_BIT  ;
    }
}

void 
GLData::setWithSmooth( const bool option )
{
  mWithSmooth = option;
}
  
void
GLData::setPositionLight( GLfloat* light ){
    for( int i = 0; i<4; i++ ){
        position_light_[i] = light[i];
    }
}


}

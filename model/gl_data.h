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
 * $Id: gl_machine.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_UTILS_GL_MACHINE_H_
#define Q3D_UTILS_GL_MACHINE_H_ 1

#include <q3D/model/model_scope.h>

#include <QtOpenGL>

namespace Q3D {

class MODELSHARED_EXPORT GLData
{
 public:
  static GLData* instance();

  const GLfloat* ambientLight() const { return ambient_light_; }
  const GLfloat* diffusionLight() const { return diffusion_light_; }
  const GLfloat* specularLight() const { return specular_light_; }
  const GLfloat* positionLight() const { return position_light_; }

  const GLbitfield&  masqueClear() const { return mMasqueClear; }
  const GLfloat* backgroundColor() const { return mCouleurAP; }

  void setPositionLight( GLfloat* light );




  bool    withAxis() const { return mWithAxis; }
  bool    withFill() const  { return mWithFill; }
  bool    withLight() const { return mWithLight; }
  bool    withProf() const { return mWithProf; }
  bool    withSmooth() const { return mWithSmooth; }

  void    setWithAxis  ( const bool option );
  void    setWithFill  ( const bool option );
  void    setWithLight ( const bool option );
  void    setWithProf  ( const bool option );
  void    setWithSmooth( const bool option );

  
 protected:

  /*Light parameters*/
  GLfloat      position_light_[4] ;
  GLfloat      ambient_light_[4];
  GLfloat      specular_light_[4];
  GLfloat      diffusion_light_[4];

  bool                mWithAxis;
  bool                mWithLight;
  bool                mWithFill;
  bool                mWithSmooth;
  bool                mWithProf;
  GLfloat             mCouleurAP[4]; /*background color*/
  GLbitfield          mMasqueClear ;

  static GLData* instance_;
  GLData();
};

}
#endif /* Q3D_UTILS_GL_MACHINE_H_ */


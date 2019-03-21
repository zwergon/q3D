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

#include <q3D/gui/gui_global.h>

#include <q3D/model/point3d.h>
#include <q3D/model/picking.h>

#include <QtOpenGL>
#include <QSet>

namespace Q3D {

class GUISHARED_EXPORT GLData
{
 public:
  static GLData* instance();

  const GLfloat* ambientLight() const { return ambient_light_; }
  const GLfloat* diffusionLight() const { return diffusion_light_; }
  const GLfloat* specularLight() const { return specular_light_; }
  const GLfloat* positionLight() const { return position_light_; }

  const GLbitfield&  clearMask() const { return clear_mask_; }
  const GLfloat* backgroundColor() const { return color_bg_; }

  void setPositionLight( GLfloat* light );

  bool    withAxis() const { return with_axis_; }
  bool    withFill() const  { return with_fill_; }
  bool    withLight() const { return with_light_; }
  bool    withProf() const { return with_prof_; }
  bool    withSmooth() const { return with_smooth_; }

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

  bool                with_axis_;
  bool                with_light_;
  bool                with_fill_;
  bool                with_smooth_;
  bool                with_prof_;
  GLfloat             color_bg_[4]; /*background color*/
  GLbitfield          clear_mask_ ;

  static GLData* instance_;
  GLData();
};

}
#endif /* Q3D_UTILS_GL_MACHINE_H_ */


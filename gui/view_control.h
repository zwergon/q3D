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
 * $Id: view_control_dlg.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_UTILS_VIEW_CONTROL_DLG_H_
#define Q3D_UTILS_VIEW_CONTROL_DLG_H_ 1

#include <q3D/gui/gui_global.h>
#include <q3D/model/model.h>
#include <q3D/gui/arc_ball.h>

namespace Q3D {

class CGlArea;

class GUISHARED_EXPORT ViewControl
{
public:
    ViewControl();


    ArcBall& arcBall() { return arc_ball_; }
    void setBounds( int width, int heigth );
    void getRotation4f( GLfloat* t) { return arc_ball_.getRotation4f(t); }

    double camX() { return cam_x_; }
    double camY() { return cam_y_; }
    double camZ() { return cam_z_; }

    double centreX() { return centre_x_; }
    double centreY() { return centre_y_; }
    double centreZ() { return centre_z_; }

    double zoomFactor() { return zoom_factor_; }
    double scaleZ() { return scale_z_ / zoom_factor_; }

    double dx(){ return dx_; }
    double dy(){ return dy_; }

    double pointSize(){ return point_size_; }

    void   scaleZIncr( bool increase  );
    void   scaleXYIncr( bool increase );
    void   camTranslate( double t_x, double t_y, double t_z );


    void setControls ( const Point3d& min, const Point3d& max );

protected:
    void camInit();

private:
    ArcBall             arc_ball_;
    double              cam_x_, cam_y_, cam_z_;
    double              zoom_factor_, zoom_inc_;
    double              scale_z_, scale_z_inc_;
    double              dist_min_, dist_max_;
    double              centre_x_, centre_y_, centre_z_;
    double              point_size_;
    double              dx_, dy_;


};

}

#endif /* Q3D_UTILS_VIEW_CONTROL_DLG_H_ */


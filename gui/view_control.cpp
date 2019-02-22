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
 * $Id: view_control_dlg.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include <math.h>
#include <iostream>

#include <q3D/gui/gl_area.h>

#include "view_control.h"

namespace Q3D {

using namespace std;

ViewControl::ViewControl(){
    camInit();
}


void ViewControl::setBounds(int width, int height){
    arc_ball_.setBounds(width, height);
}


void   ViewControl::scaleZIncr( bool increase  ){
    if ( increase ){
        scale_z_ *= (1 + scale_z_inc_ );
    }
    else {
        scale_z_ *= (1 - scale_z_inc_);
    }
}

void ViewControl::scaleXYIncr( bool increase ){
    if ( increase ){
        zoom_factor_ *= (1 + zoom_inc_ );
    }
    else {
        zoom_factor_ *= (1 - zoom_inc_ );
    }
}

void   ViewControl::camTranslate( double t_x, double t_y, double  ){
    dx_ += t_x;
    dy_ += t_y;
}

void ViewControl::camInit() {

    arc_ball_.reset();
    zoom_factor_    = 1.;
    zoom_inc_ = 0.1;

    scale_z_    = 1.;
    scale_z_inc_ = 0.1;

    dx_ = 0;
    dy_ = 0;

}

void ViewControl::setControls ( const Point3d& min, const Point3d& max ){

    Point3d diff;

    for ( quint16 i=0; i<3; ++i)
        diff[i] = max[i] - min[i];

    /* compute a dist whose diameter
        encloses the whole bounding box
        */
    double dist_max = diff[0];
    if ( diff[1] > dist_max )
        dist_max = diff[1];
    else if ( diff[2] > dist_max )
        dist_max = diff[2];

    /*compute center position*/
    centre_x_ = (max[0] + min[0]) / 2 ;
    centre_y_ = (max[1] + min[1]) / 2 ;
    centre_z_ = (max[2] + min[2]) / 2 ;


    zoom_factor_ = 2. / dist_max;
    scale_z_ = zoom_factor_;


    /*Camera position*/
    cam_x_ = centre_x_ ;
    cam_y_ = centre_y_ ;
    cam_z_ = centre_z_ + 3. ;

    dx_ = 0;
    dy_ = 0;

    point_size_ = dist_max / 50.;

}


}


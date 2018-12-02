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

void   ViewControl::angleTranslate( double angle_x, double angle_y ){
    int angle_y_max = 360;
    int angle_x_max = 360;

    mAngleX += angle_x/4.;
    mAngleY += angle_y/4.;

    while ( mAngleX  <  0 )
        mAngleX += angle_x_max;

    while ( mAngleX  >  angle_x_max )
        mAngleX -= angle_x_max;

    while ( mAngleY  <  0 )
        mAngleY += angle_y_max;

    while ( mAngleY  >  angle_y_max )
        mAngleY -= angle_y_max;

}


void   ViewControl::scaleZIncr( bool increase  ){
    if ( increase ){
        mScaleZ *= (1 + mScaleZInc );
    }
    else {
        mScaleZ *= (1 - mScaleZInc);
    }
}

void ViewControl::scaleXYIncr( bool increase ){
    if ( increase ){
        mZoomFactor *= (1 + mZoomInc );
    }
    else {
        mZoomFactor *= (1 - mZoomInc );
    }
}

void   ViewControl::camTranslate( double t_x, double t_y, double  ){
    mDx += t_x;
    mDy += t_y;
}

void ViewControl::camInit() {
    mAngleX = 0 ;
    mAngleY = 0 ;

    mZoomFactor    = 1.;
    mZoomInc = 0.1;

    mScaleZ    = 1.;
    mScaleZInc = 0.1;

    mDx = 0;
    mDy = 0;

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
    mCentreX = (max[0] + min[0]) / 2 ;
    mCentreY = (max[1] + min[1]) / 2 ;
    mCentreZ = (max[2] + min[2]) / 2 ;


    mZoomFactor = 2. / dist_max;
    mScaleZ = mZoomFactor;


    /*Camera position*/
    mCamX = mCentreX ;
    mCamY = mCentreY ;
    mCamZ = mCentreZ + 3. ;

    mDx = 0;
    mDy = 0;

    mPointSize = dist_max / 50.;

}


}


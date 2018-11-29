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

#include <q3D/gui/gui_scope.h>
#include <q3D/model/model.h>

namespace Q3D {

class CGlArea;

class GUISHARED_EXPORT ViewControl
{
public:
    ViewControl();

    float angleX() { return mAngleX; }
    float angleY() { return mAngleY; }

    double camX() { return mCamX; }
    double camY() { return mCamY; }
    double camZ() { return mCamZ; }

    double centreX() { return mCentreX; }
    double centreY() { return mCentreY; }
    double centreZ() { return mCentreZ; }

    double zoomFactor() { return mZoomFactor; }
    double scaleZ() { return mScaleZ / mZoomFactor; }

    double dx(){ return mDx; }
    double dy(){ return mDy; }

    double pointSize(){ return mPointSize; }

    void   angleTranslate( float angle_x, float angle_y );
    void   scaleZIncr( bool increase  );
    void   scaleXYIncr( bool increase );
    void   camTranslate( double t_x, double t_y, double t_z );


    void setControls ( const Point3f& min, const Point3f& max );

protected:
    void camInit();

private:
    float               mAngleX, mAngleY;
    double              mCamX, mCamY, mCamZ;
    double              mZoomFactor, mZoomInc;
    double              mScaleZ, mScaleZInc;
    double              mDistMin, mDistMax;
    double              mCentreX, mCentreY, mCentreZ;
    double              mPointSize;
    double              mDx, mDy;


};

}

#endif /* Q3D_UTILS_VIEW_CONTROL_DLG_H_ */


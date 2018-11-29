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
 * $Id: gl_area.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_GUI_GLAREA_H_
#define Q3D_GUI_GLAREA_H_ 1

#include <QSet>

#include <QtOpenGL>

#include <q3D/model/model.h>
#include <q3D/model/renderer.h>

#include <q3D/model/gl_data.h>
#include <q3D/gui/view_control.h>


namespace Q3D {

class CGlArea : public QGLWidget
{
	Q_OBJECT

public:
	CGlArea( QWidget* parent );

	void    addCoreRenderer   ( ModelRenderer* );
	void    removeCoreRenderer( ModelRenderer* );

protected:
	/*overload of QT virtual functions*/
	void  initializeGL();
	void  mousePressEvent  ( QMouseEvent * );
	void  mouseMoveEvent   ( QMouseEvent * );
	void  mouseReleaseEvent( QMouseEvent * );
    void  wheelEvent       ( QWheelEvent * );
	void  paintGL          ( );
	void  resizeGL         ( int w, int h );

private:
	void init();
	void computeBoundingBox();
	void setGeometry();
	void buildAll();
	void buildAxis();
    void popupMenuExec( QMouseEvent *);

    void cameraRotate( int angle_x, int angle_y );
    void cameraZoom( bool increase );
    void cameraTranslate( int translate_y, int translate_x );

private slots:
    void update( Model* );
	void smoothOptionSlot();
    void lightOptionSlot();
    void axisOptionSlot();


private:
    QSet<ModelRenderer*> model_renderers_;
	Point3f           min_, max_; /*bounding box*/
	
	/*View parameters*/
	int                 mGlLights, mGlAxis ; 
    ViewControl         view_control_;
    GLData*             gl_machine_;
	int                 mXPrec, mYPrec ;
	bool                mMoveActivated;


};

}

#endif /* Q3D_GUI_GLAREA_H_ */


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

class AbstractTool;

class CGlArea : public QGLWidget
{
	Q_OBJECT

public:
	CGlArea( QWidget* parent );
    virtual ~CGlArea();

    void getBoundingBox( Point3f& min, Point3f& max ) const;
    ViewControl& getViewControl();

    void addCoreRenderer   ( ModelRenderer* );
    void removeCoreRenderer( ModelRenderer* );

    void setActiveTool( AbstractTool* tool );

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

private slots:
    void update( Model* );
	void smoothOptionSlot();
    void lightOptionSlot();
    void axisOptionSlot();


private:
    QSet<ModelRenderer*> model_renderers_;
    AbstractTool* current_tool_;
    Point3f min_;
    Point3f max_; /*bounding box*/
	
	/*View parameters*/
    int gl_lights_;
    int gl_axis_ ;
    GLData* gl_machine_;
    ViewControl view_control_;

};

inline void CGlArea::getBoundingBox(Point3f &min, Point3f &max) const {
    min = min_;
    max = max_;
}

inline ViewControl& CGlArea::getViewControl(){
    return view_control_;
}

}

#endif /* Q3D_GUI_GLAREA_H_ */


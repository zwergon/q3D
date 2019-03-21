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

#include <q3D/gui/gui_global.h>

#include <QSet>

#include <QtOpenGL>

#include <q3D/model/model.h>
#include <q3D/model/renderer.h>
#include <q3D/model/renderer_area.h>

#include <q3D/gui/gl_data.h>
#include <q3D/gui/view_control.h>

#include <QMainWindow>

namespace Q3D {

class AbstractTool;

class GUISHARED_EXPORT CGlArea : public QGLWidget, public RendererArea
{
	Q_OBJECT

public:
	CGlArea( QWidget* parent );
    virtual ~CGlArea();

    QMainWindow* mainWindow() const {return qobject_cast<QMainWindow*>(parent());}

    void getBoundingBox( Point3d& min, Point3d& max ) const;
    ViewControl& getViewControl();

    void addCoreRenderer   ( ModelRenderer* );
    void removeCoreRenderer( ModelRenderer* );
    void doPickingAt(int x, int y, Pick& );

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void onToolSelected(AbstractTool*);


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
    void adjustBoundingBox();
	void buildAll();
	void buildAxis();
    void popupMenuExec( QMouseEvent *);

private slots:
    void update( Model* );
    void updateRenderer( ModelRenderer* );
	void smoothOptionSlot();
    void lightOptionSlot();
    void axisOptionSlot();


private:
    QSet<ModelRenderer*> model_renderers_;
    AbstractTool* current_tool_;
    Point3d min_;
    Point3d max_; /*bounding box*/
	
	/*View parameters*/
    int gl_lights_;
    GLuint  gl_axis_ ;
    GLData* gl_machine_;
    ViewControl view_control_;

};

inline void CGlArea::getBoundingBox(Point3d &min, Point3d &max) const {
    min = min_;
    max = max_;
}

inline ViewControl& CGlArea::getViewControl(){
    return view_control_;
}

}

#endif /* Q3D_GUI_GLAREA_H_ */


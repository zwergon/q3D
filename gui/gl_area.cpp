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
 * $Id: gl_area.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */
#include "gl_area.h"

#include "glu.h"

#include <QMenu>
#include <QMouseEvent>

#include <QGLWidget>
#include <q3D/model/model.h>
#include <q3D/model/renderer.h>
#include <q3D/model/renderer_area.h>

#include <q3D/gui/camera_tool.h>
#include <q3D/gui/tool_manager.h>



namespace Q3D {


QSize CGlArea::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize CGlArea::sizeHint() const
{
    return QSize(800, 800);
}

void
CGlArea::init()
{
    setAttribute( Qt::WA_NoSystemBackground );

    gl_machine_ = GLData::instance();

    gl_axis_ = 0;   /*dummy glList number*/
    gl_lights_ = 0;

    setMouseTracking( true );


}

/*!
Create a CGlArea widget
*/
CGlArea::CGlArea( QWidget* parent )
    : QGLWidget( parent ),
      current_tool_(nullptr)
{
    init();
}

CGlArea::~CGlArea(){
}

/*!
Set up the OpenGL rendering state, and define display list
*/

void 
CGlArea::initializeGL()
{

    const GLfloat* bg = gl_machine_->backgroundColor();
    glClearColor( bg[0], bg[1], bg[2], bg[3] );    // Background color
    glClearDepth(1.0f);                      // Depth Buffer Setup
    glEnable( GL_DEPTH_TEST );			    // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);	            // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

/*!
Set up the OpenGL view port, matrix mode, etc.
*/

void CGlArea::resizeGL( int width, int height )
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double fwidth = 0.5;
    double fheight = 0.5 * ((double)(height)/width);
    glFrustum(-fwidth,fwidth,-fheight,fheight,1.,500.);

    view_control_.setBounds(width, height);
}

/*!
Paint the box. The actual openGL commands for drawing the box are
performed here.
*/

void CGlArea::paintGL()
{
    glClear( gl_machine_->clearMask() );

    if (gl_machine_->withLight()){
        glEnable( GL_LIGHTING );
        glEnable( GL_LIGHT0);

        glLightfv( GL_LIGHT0, GL_AMBIENT,  gl_machine_->ambientLight() );
        glLightfv( GL_LIGHT0, GL_DIFFUSE,  gl_machine_->diffusionLight() );
        glLightfv( GL_LIGHT0, GL_SPECULAR, gl_machine_->specularLight() );

        // set material properties which will be assigned by glColor
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
    }
    else {
        glDisable( GL_LIGHTING );
        glDisable( GL_LIGHT0 );
        glDisable( GL_COLOR_MATERIAL );
    }

    if ( gl_machine_->withSmooth() ){
        glShadeModel( GL_SMOOTH );
    }
    else {
        glShadeModel( GL_FLAT );
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    GLu::lookAt( view_control_.camX(), view_control_.camY(), view_control_.camZ(),
               view_control_.centreX(), view_control_.centreY(), view_control_.centreZ(),
               0.0, 1.0 ,0.0 );

    glTranslated( view_control_.centreX(), view_control_.centreY(), view_control_.centreZ() );
    glScaled( view_control_.zoomFactor(), view_control_.zoomFactor(), view_control_.zoomFactor());
    glTranslated( view_control_.dx(), view_control_.dy(), 0 );

    GLfloat transform[16];
    view_control_.getRotation4f(transform);
    glMultMatrixf(transform);

    glScaled( 1., 1., view_control_.scaleZ() );

    if (gl_machine_->withLight()){
        glLightfv(GL_LIGHT0, GL_POSITION, gl_machine_->positionLight() );
    }

    glTranslated( -view_control_.centreX(), -view_control_.centreY(), -view_control_.centreZ() );


    if ( gl_machine_->withAxis() && glIsList(gl_axis_))
        glCallList(gl_axis_);


    for ( QSet<ModelRenderer*>::iterator itr = model_renderers_.begin();
          itr != model_renderers_.end();
          itr++ ){
        ModelRenderer *model_renderer = *itr;
        model_renderer->draw(this);
    }

    //qDebug() << "draw " << uv_quads_.size() << " patches";

    //second pass to draw textured patches.
    if ( !uv_quads_.empty() ){
        glGetFloatv( GL_MODELVIEW_MATRIX, UVQuad::modelview );
        UVQuad::cam2center[0] = view_control_.camX() - view_control_.centreX();
        UVQuad::cam2center[1] = view_control_.camY() - view_control_.centreY();
        UVQuad::cam2center[2] = view_control_.camZ() - view_control_.centreZ();

        qSort(uv_quads_.begin(), uv_quads_.end(), UVQuad::lessThan);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for( auto uv_quad : uv_quads_ ){
            uv_quad.draw();
        }
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

}

void
CGlArea::computeBoundingBox()
{

    min_ = max_positif_point3<double>();
    max_ = min_negatif_point3<double>();
    QSetIterator<ModelRenderer*> it_r( model_renderers_ );
    while( it_r.hasNext() ){
        Model* model = it_r.next()->model();

        const Point3d& min = model->mini();
        const Point3d& max = model->maxi();

        for (quint16 j=0; j<3; ++j){
            if (min[j] < min_[j]) min_[j] = min[j];
            if (max[j] > max_[j]) max_[j] = max[j];
        }

    }

    //ensure bounding box is not zero size in one direction.
    for ( quint16 i=0; i<3; i++){
        if ( AreSame(min_[i], max_[i]) ){
            max_[i] += 1;
        }
    }
}

void
CGlArea::onToolSelected(AbstractTool *tool){

    qDebug() << "setActiveTool";

    if ( current_tool_ != nullptr ){
        current_tool_->deactivate();
        mainWindow()->statusBar()->clearMessage();
    }

    current_tool_ = tool;
    if ( nullptr != current_tool_ ){
        current_tool_->activate();
        setCursor(current_tool_->cursor());
    }

}

void 
CGlArea::adjustBoundingBox()
{
    computeBoundingBox();

    view_control_.setControls(min_, max_);

    GLfloat light[4];

    /*specify position of all lights*/
    light[0] = (GLfloat)max_[0] ;
    light[1] = (GLfloat)min_[1] ;
    light[2] = (GLfloat)max_[2] ;
    light[3] = 1.0 ;

    gl_machine_->setPositionLight( light );

}

void 
CGlArea::buildAll()
{

    buildAxis();
    for ( QSet<ModelRenderer*>::iterator itr = model_renderers_.begin();
          itr != model_renderers_.end();
          itr++ ){
        ModelRenderer *model_renderer = *itr;
        model_renderer->update(this);
    }
}

void  
CGlArea::mousePressEvent  ( QMouseEvent* mouseEvent )
{
    if ( (mouseEvent->buttons() & Qt::RightButton) &&
         !(mouseEvent->modifiers() & Qt::CTRL)) {
        popupMenuExec( mouseEvent );
        return;
    }

    if ( ( nullptr != current_tool_ ) && current_tool_->isActive() ){
        current_tool_->handleMousePressEvent(mouseEvent);
    }

}

void  
CGlArea::mouseMoveEvent(QMouseEvent* mouseEvent){
    if ( ( nullptr != current_tool_ ) && current_tool_->isActive() ){
        current_tool_->handleMouseMoveEvent(mouseEvent);
    }
}

void
CGlArea::wheelEvent(QWheelEvent* wevent){
    if ( ( nullptr != current_tool_ ) && current_tool_->isActive() ){
        current_tool_->handleWheelEvent(wevent);
    }
}

void 
CGlArea::mouseReleaseEvent(QMouseEvent* mouseEvent){
    if ( ( nullptr != current_tool_ ) && current_tool_->isActive() ){
        current_tool_->handleMouseReleaseEvent(mouseEvent);
    }
}

void
CGlArea::buildAxis()
{

    if ( !gl_machine_->withAxis() ){
        return;
    }

    /* Liste pour l'objet terrain */
    if (glIsList(gl_axis_))
        glDeleteLists(gl_axis_,1);

    gl_axis_=glGenLists(1);
    glNewList(gl_axis_, GL_COMPILE);

    if (gl_machine_->withLight())
        glDisable( GL_LIGHTING );


    glColor3f(1., 0., 0.);

    glBegin(GL_LINE_STRIP);
    glVertex3d(min_[0], min_[1], min_[2]);
    glVertex3d(min_[0], min_[1], max_[2]);
    glVertex3d(min_[0], max_[1], max_[2]);
    glVertex3d(min_[0], max_[1], min_[2]);
    glVertex3d(min_[0], min_[1], min_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3d(max_[0], min_[1], min_[2]);
    glVertex3d(max_[0], min_[1], max_[2]);
    glVertex3d(max_[0], max_[1], max_[2]);
    glVertex3d(max_[0], max_[1], min_[2]);
    glVertex3d(max_[0], min_[1], min_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3d(min_[0], min_[1], max_[2]);
    glVertex3d(max_[0], min_[1], max_[2]);
    glVertex3d(max_[0], max_[1], max_[2]);
    glVertex3d(min_[0], max_[1], max_[2]);
    glVertex3d(min_[0], min_[1], max_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3d(min_[0], min_[1], min_[2]);
    glVertex3d(max_[0], min_[1], min_[2]);
    glVertex3d(max_[0], max_[1], min_[2]);
    glVertex3d(min_[0], max_[1], min_[2]);
    glVertex3d(min_[0], min_[1], min_[2]);
    glEnd();

    if (gl_machine_->withLight())
        glEnable(GL_LIGHTING);

    glEndList();

}

void
CGlArea::addCoreRenderer(ModelRenderer *model_renderer)
{
    if ( nullptr == model_renderer ){
        return;
    }

    model_renderers_.insert(model_renderer);
    connect( model_renderer, &ModelRenderer::needRedraw,
             this, &CGlArea::updateRenderer);
    adjustBoundingBox();
    buildAll();
    updateGL();
}

void 
CGlArea::removeCoreRenderer(ModelRenderer *model_renderer){

    QSet<ModelRenderer*>::iterator itr = model_renderers_.find( model_renderer );
    if ( itr != model_renderers_.end() ){
        ModelRenderer* model_renderer = *itr;
        model_renderer->clean(this);
        model_renderer->disconnect(this);
        model_renderers_.erase( itr );

        updateGL();
    }
}

void
CGlArea::axisOptionSlot()
{
    gl_machine_->setWithAxis( !gl_machine_->withAxis() );
    buildAll();
    updateGL();
}


void
CGlArea::lightOptionSlot()
{
    gl_machine_->setWithLight( !gl_machine_->withLight() );
    buildAll();
    repaint();

}

void
CGlArea::smoothOptionSlot()
{
    gl_machine_->setWithSmooth( !gl_machine_->withSmooth() );
    buildAll();
    repaint();
}

void 
CGlArea::popupMenuExec( QMouseEvent *event )
{

    QMenu* areaMenu = new QMenu( this );

    QAction* action = areaMenu->addAction( "Axis", this,  SLOT(axisOptionSlot()) );
    action->setCheckable( true );
    action->setChecked( gl_machine_->withAxis());

    action = areaMenu->addAction( "Lights On/Off",    this,  SLOT(lightOptionSlot()) );
    action->setCheckable( true );
    action->setChecked( gl_machine_->withLight() );

    action = areaMenu->addAction( "Smooth On/Off"   , this,  SLOT(smoothOptionSlot()) );
    action->setCheckable( true );
    action->setChecked( gl_machine_->withSmooth());

    areaMenu->exec( mapToGlobal(event->pos()) );

    delete areaMenu;

}


void
CGlArea::updateRenderer(ModelRenderer *renderer ){
    renderer->update(this);
    updateGL();
}

/**
  * When Model is changed, look at ModelRenderer to know whose need to be
  * updated.
  */
void
CGlArea::update( Model* model ){

    bool need_update = false;
    QSetIterator<ModelRenderer*> it_m( model_renderers_ );
    while( it_m.hasNext() ){
        ModelRenderer *model_renderer = it_m.next();
        if ( model == model_renderer->model() ){
            model_renderer->update(this);
            need_update = true;
        }
    }
    if ( need_update ){
        /*bounding box may have changed*/
        computeBoundingBox();
        buildAxis();
        updateGL();
    }
}

void
CGlArea::doPickingAt(int x, int y, Pick& pick){

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble matModelView[16], matProjection[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView );
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection );

    GLfloat winX = (float)x;
    GLfloat winY = (float)viewport[3] - (float)y;

    GLdouble posX, posY, posZ;
    GLu::unProject(winX, winY, 0.,
                   matModelView, matProjection, viewport,
                   &posX, &posY, &posZ);

    Point3d origin = { posX, posY, posZ };

    GLu::unProject(winX, winY, 1.,
                   matModelView, matProjection, viewport,
                   &posX, &posY, &posZ);


    Point3d ray = { posX - origin[0], posY - origin[1], posZ - origin[2]};

    pick.setPickRay(origin, ray);
    foreach(ModelRenderer* renderer, model_renderers_){
        renderer->pick(pick);
    }

    pick.sort();

}



}


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

#include <QMenu>
#include <QMouseEvent>

#include <wykobi/wykobi.hpp>

#include <QGLWidget>
#include <q3D/model/model.h>
#include <q3D/model/renderer.h>


namespace Q3D {

/*
** Make m an identity matrix
*/
static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

static void normalize(float v[3])
{
    float r;

    r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

/**
 * @brief gluLookAt
 * The implementation of gluLookAt from http://www.mesa3d.org to avoid link with GLU lib.
 */
static void
gluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
           GLdouble centerx, GLdouble centery, GLdouble centerz,
           GLdouble upx, GLdouble upy, GLdouble upz )
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}


void
CGlArea::init()
{
    setAttribute( Qt::WA_NoSystemBackground );

    gl_machine_ = GLData::instance();

    mGlAxis = -1;   /*dummy glList number*/
    mGlLights = -1;

    mMoveActivated = false;

    setMouseTracking( true );
}

/*!
Create a CGlArea widget
*/
CGlArea::CGlArea( QWidget* parent )
    : QGLWidget( parent )
{
    init();
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
    float fwidth = 0.5f;
    float fheight = 0.5f * ((float)(height)/width);
    glFrustum(-fwidth,fwidth,-fheight,fheight,1.f,500.0f);
}

/*!
Paint the box. The actual openGL commands for drawing the box are
performed here.
*/

void CGlArea::paintGL()
{
    glClear( gl_machine_->masqueClear() );

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
    gluLookAt( view_control_.camX(), view_control_.camY(), view_control_.camZ(),
               view_control_.centreX(), view_control_.centreY(), view_control_.centreZ(),
               0.0, 1.0 ,0.0 );

    glTranslatef( view_control_.centreX(), view_control_.centreY(), view_control_.centreZ() );

    glScalef( view_control_.zoomFactor(), view_control_.zoomFactor(), view_control_.zoomFactor());

    glTranslatef( view_control_.dx(), view_control_.dy
                  (), 0 );
    glRotatef( view_control_.angleX(), 1.0, 0.0, 0.0);
    glRotatef( view_control_.angleY(), 0.0, 0.0, 1.0 );

    glScalef( 1., 1., view_control_.scaleZ() );

    if (gl_machine_->withLight()){
        glLightfv(GL_LIGHT0, GL_POSITION, gl_machine_->positionLight() );
    }

    glTranslatef( -view_control_.centreX(), -view_control_.centreY(), -view_control_.centreZ() );

    if ( gl_machine_->withAxis() && glIsList(mGlAxis))
        glCallList(mGlAxis);

    for ( QSet<ModelRenderer*>::iterator itr = model_renderers_.begin();
          itr != model_renderers_.end();
          itr++ ){
        ModelRenderer *model_renderer = *itr;
        if (glIsList(model_renderer->glList())){
            glCallList(model_renderer->glList());
        }

    }
}

void
CGlArea::computeBoundingBox()
{

    min_ = wykobi::positive_infinite_point3d<float>();
    max_ = wykobi::negative_infinite_point3d<float>();
    QSetIterator<ModelRenderer*> it_r( model_renderers_ );
    while( it_r.hasNext() ){
        Model* model = it_r.next()->model();

        const Point3f& min = model->mini();
        const Point3f& max = model->maxi();

        for (int j=0; j<3; ++j){
            if (min[j] < min_[j]) min_[j] = min[j];
            if (max[j] > max_[j]) max_[j] = max[j];
        }

    }

    //ensure bounding box is not zero size in one direction.
    for ( int i=0; i<3; i++){
        if ( wykobi::is_equal(min_[i], max_[i]) ){
            max_[i] += 1;
        }
    }
}

void 
CGlArea::setGeometry()
{
    computeBoundingBox();

    view_control_.setControls( min_, max_ );

    GLfloat light[4];

    /*specify position of all lights*/
    light[0] = max_[0] ;
    light[1] = min_[1] ;
    light[2] = max_[2] ;
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
        model_renderer->update();
    }
}

void  
CGlArea::mousePressEvent  ( QMouseEvent* mouseEvent )
{
    QPoint p = mouseEvent->globalPos();
    mXPrec = p.x();
    mYPrec = p.y();

    mMoveActivated = true;

    if ( (mouseEvent->buttons() & Qt::RightButton) &&
         !(mouseEvent->modifiers() & Qt::CTRL)) {
        popupMenuExec( mouseEvent );
        mMoveActivated = false;
    }

}

void  
CGlArea::mouseMoveEvent  ( QMouseEvent* mouseEvent )
{

    if ( mMoveActivated )
    {
        QPoint p = mouseEvent->globalPos();

        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            if ( mouseEvent->modifiers() == Qt::CTRL ){
                cameraTranslate( p.y()-mYPrec,	p.x()-mXPrec  ) ;
            }
            else {
                cameraRotate( p.y()-mYPrec,	p.x()-mXPrec ) ;
            }
        }

        if (mouseEvent->buttons() & Qt::MidButton)
        {
            view_control_.scaleZIncr( (p.y()-mYPrec) < 0 ) ;
            updateGL();
        }

        mXPrec = p.x();
        mYPrec = p.y();

    }

}

void
CGlArea::wheelEvent(QWheelEvent* wevent ){
    if (!mMoveActivated)
        cameraZoom( wevent->delta() < 0 ) ;
}

void
CGlArea::cameraRotate( int angle_x, int angle_y ){
    view_control_.angleTranslate(  angle_x,	angle_y ) ;
    updateGL();
}


void
CGlArea::cameraZoom( bool increase ){
    view_control_.scaleXYIncr( increase );
    view_control_.scaleZIncr( increase ) ;
    updateGL();
}

void
CGlArea::cameraTranslate( int translate_y, int translate_x ){
    double dx =  (double)translate_x / (double)width()  * (max_[0]- min_[0]);
    double dy = -(double)translate_y / (double)height() * (max_[1]- min_[1]);
    view_control_.camTranslate(dx, dy, 0);
    updateGL();
}

void 
CGlArea::mouseReleaseEvent( QMouseEvent*  )
{
    mMoveActivated = false;
}


void
CGlArea::buildAxis()
{

    if ( !gl_machine_->withAxis() ){
        return;
    }

    /* Liste pour l'objet terrain */
    if (glIsList(mGlAxis))
        glDeleteLists(mGlAxis,1);

    mGlAxis=glGenLists(1);
    glNewList(mGlAxis, GL_COMPILE);

    if (gl_machine_->withLight())
        glDisable( GL_LIGHTING );


    glColor3f(1., 0., 0.);

    glBegin(GL_LINE_STRIP);
    glVertex3f(min_[0], min_[1], min_[2]);
    glVertex3f(min_[0], min_[1], max_[2]);
    glVertex3f(min_[0], max_[1], max_[2]);
    glVertex3f(min_[0], max_[1], min_[2]);
    glVertex3f(min_[0], min_[1], min_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(max_[0], min_[1], min_[2]);
    glVertex3f(max_[0], min_[1], max_[2]);
    glVertex3f(max_[0], max_[1], max_[2]);
    glVertex3f(max_[0], max_[1], min_[2]);
    glVertex3f(max_[0], min_[1], min_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(min_[0], min_[1], max_[2]);
    glVertex3f(max_[0], min_[1], max_[2]);
    glVertex3f(max_[0], max_[1], max_[2]);
    glVertex3f(min_[0], max_[1], max_[2]);
    glVertex3f(min_[0], min_[1], max_[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(min_[0], min_[1], min_[2]);
    glVertex3f(max_[0], min_[1], min_[2]);
    glVertex3f(max_[0], max_[1], min_[2]);
    glVertex3f(min_[0], max_[1], min_[2]);
    glVertex3f(min_[0], min_[1], min_[2]);
    glEnd();

    if (gl_machine_->withLight())
        glEnable(GL_LIGHTING);

    glEndList();

}

void
CGlArea::addCoreRenderer(ModelRenderer *model_renderer)
{
    if ( 0 == model_renderer ){
        return;
    }

    model_renderers_.insert(model_renderer);
    connect( model_renderer, SIGNAL(needRedraw(ModelRenderer*)),
             this, SLOT(updateGL()));
    setGeometry();
    buildAll();
    updateGL();
}

void 
CGlArea::removeCoreRenderer(ModelRenderer *model_renderer){

    QSet<ModelRenderer*>::iterator itr = model_renderers_.find( model_renderer );
    if ( itr != model_renderers_.end() ){
        ModelRenderer* model_renderer = *itr;
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
            model_renderer->update();
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

}


#include "cube_renderer.h"

#include <q3D/model/geometry.h>
#include <q3D/model/picking.h>
#include <q3D/model/renderer_area.h>

#include <q3D/gui/glu.h>

#include <q3D/cube/slicer.h>

#include <q3D/drivers/cube/cube_model.h>
#include <q3D/drivers/cube/cube_renderer_attribute.h>


namespace Q3D {



/*----------------------------------------------*/



CubeRenderer::CubeRenderer() : texture_(nullptr)
{
}

CubeRenderer::~CubeRenderer(){
}

RendererAttribute* CubeRenderer::createAttribute(){
    return new CubeRendererAttribute(this);
}

void CubeRenderer::init(){
    CubeModel* cube_model = dynamic_cast<CubeModel*>( model() );
    if ( nullptr == cube_model ){
        return;
    }

    Cube& cube = cube_model->cube();

    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());
    cube_attribute->blockSignals(true); //avoid drawing before total init
    cube_attribute->setCursorX(cube.getNx()/4);
    cube_attribute->setCursorY(cube.getNy()/3);
    cube_attribute->setCursorZ(cube.getNz()/2);
    cube_attribute->blockSignals(false);

    double min = cube.getValue(0);
    double max = min;
    for( int i=1; i<cube.size(); i++ ){
        double val = cube.getValue(i);
        if (val < min) min = val;
        if (val > max) max = val;
    }

    colormap()->setMinMax( min, max );

}

void CubeRenderer::clean(RendererArea *area){
    if ( nullptr != texture_ ){
        for(int i=0; i<3; i++){
            area->removeUVQuads(texture_[i]);
        }
        glDeleteTextures(3, texture_);
        delete [] texture_; texture_ = nullptr;
    }
}

bool CubeRenderer::hasTransparency() const{

    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());
    if ( nullptr == cube_attribute ){
        return false;
    }

    return (texture_ != nullptr )&& ( (cube_attribute->opacity()<1.) || colormap()->hasAlpha() );
}

void CubeRenderer::update(RendererArea* area){

    CubeModel* cube_model = dynamic_cast<CubeModel*>( model() );
    if ( nullptr == cube_model ){
        return;
    }

    clean(area);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);

    texture_ = new GLuint[Slice::LAST];
    glGenTextures(3, texture_);
    for( int slice = 0; slice < Slice::LAST;  slice++ ){
        createTexture(cube_model->cube(), texture_, slice);
    }

    if ( hasTransparency() ){
        createUVQuad(cube_model->cube(), texture_, area);
    }

    glDisable(GL_TEXTURE_2D);

}

void CubeRenderer::draw( RendererArea* ){
    if ( !hasTransparency() ){
        CubeRendererAttribute* cube_attribute =
                static_cast<CubeRendererAttribute*>(attribute());

        CubeModel* cube_model = dynamic_cast<CubeModel*>( model() );
        if ( nullptr == cube_model ){
            return;
        }

        Cube& cube = cube_model->cube();
        int nx = cube.getNx();
        int ny = cube.getNy();
        int nz = cube.getNz();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_[Slice::XY]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(0, 0, cube_attribute->getCursorZ());
        glTexCoord2i(0, 1); glVertex3i(0, ny, cube_attribute->getCursorZ());
        glTexCoord2i(1, 1); glVertex3i(nx, ny, cube_attribute->getCursorZ());
        glTexCoord2i(1, 0); glVertex3i(nx, 0, cube_attribute->getCursorZ());
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture_[Slice::XZ]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(0, cube_attribute->getCursorY(), 0);
        glTexCoord2i(0, 1); glVertex3i(0, cube_attribute->getCursorY(), nz);
        glTexCoord2i(1, 1); glVertex3i(nx, cube_attribute->getCursorY(), nz);
        glTexCoord2i(1, 0); glVertex3i(nx, cube_attribute->getCursorY(), 0);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture_[Slice::YZ]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(cube_attribute->getCursorX(), 0, 0);
        glTexCoord2i(0, 1); glVertex3i(cube_attribute->getCursorX(), 0, nz);
        glTexCoord2i(1, 1); glVertex3i(cube_attribute->getCursorX(), ny, nz);
        glTexCoord2i(1, 0); glVertex3i(cube_attribute->getCursorX(), ny, 0);
        glEnd();

        glDisable(GL_TEXTURE_2D);


    }
}

void CubeRenderer::createUVQuad(const Cube& cube, GLuint* tex, RendererArea* area) {


    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());

    int nx = cube.getNx();
    int ny = cube.getNy();
    int nz = cube.getNz();

    int ix = cube_attribute->getCursorX();
    int iy = cube_attribute->getCursorY();
    int iz = cube_attribute->getCursorZ();

    float ax = (float)ix /(float)nx;
    float ay = (float)iy /(float)ny;
    float az = (float)iz /(float)nz;

    UVQuad q1;

    //-----------
    q1.tex = tex[Slice::XY];
    q1.id = (int)this;
    q1.pts[0].init(0, 0, iz, 0, 0);
    q1.pts[1].init(ix, 0, iz, ax, 0);
    q1.pts[2].init(ix, iy, iz, ax, ay);
    q1.pts[3].init(0, iy, iz, 0, ay);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, 0, iz, ax, 0);
    q1.pts[1].init(nx, 0, iz, 1, 0);
    q1.pts[2].init(nx, iy, iz, 1., ay);
    q1.pts[3].init(ix, iy, iz, ax, ay);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, iy, iz, ax, ay);
    q1.pts[1].init(nx, iy, iz, 1., ay);
    q1.pts[2].init(nx, ny, iz, 1., 1.);
    q1.pts[3].init(ix, ny, iz, ax, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(0, iy, iz, 0, ay);
    q1.pts[1].init(ix, iy, iz, ax, ay);
    q1.pts[2].init(ix, ny, iz, ax, 1.);
    q1.pts[3].init(0, ny, iz, 0, 1.);
    area->addUVQuad(q1);

    //-----------
    q1.tex = tex[Slice::YZ];
    q1.pts[0].init(ix, 0, 0, 0, 0);
    q1.pts[1].init(ix, iy, 0, ay, 0);
    q1.pts[2].init(ix, iy, iz, ay, az);
    q1.pts[3].init(ix, 0, iz, 0, az);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, iy, 0, ay, 0);
    q1.pts[1].init(ix, ny, 0, 1, 0);
    q1.pts[2].init(ix, ny, iz, 1., az);
    q1.pts[3].init(ix, iy, iz, ay, az);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, iy, iz, ay, az);
    q1.pts[1].init(ix, ny, iz, 1., az);
    q1.pts[2].init(ix, ny, nz, 1., 1.);
    q1.pts[3].init(ix, iy, nz, ay, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, 0, iz, 0, az);
    q1.pts[1].init(ix, iy, iz, ay, az);
    q1.pts[2].init(ix, iy, nz, ay, 1.);
    q1.pts[3].init(ix, 0, nz, 0, 1.);
    area->addUVQuad(q1);

    //-----------
    q1.tex = tex[Slice::XZ];

    q1.pts[0].init(0, iy, 0, 0, 0);
    q1.pts[1].init(ix, iy, 0, ax, 0);
    q1.pts[2].init(ix, iy, iz, ax, az);
    q1.pts[3].init(0, iy, iz, 0, az);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, iy, 0, ax, 0);
    q1.pts[1].init(nx, iy, 0, 1, 0);
    q1.pts[2].init(nx, iy, iz, 1., az);
    q1.pts[3].init(ix, iy, iz, ax, az);
    area->addUVQuad(q1);

    q1.pts[0].init(ix, iy, iz, ax, az);
    q1.pts[1].init(nx, iy, iz, 1., az);
    q1.pts[2].init(nx, iy, nz, 1., 1.);
    q1.pts[3].init(ix, iy, nz, ax, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(0, iy, iz, 0, az);
    q1.pts[1].init(ix, iy, iz, ax, az);
    q1.pts[2].init(ix, iy, nz, ax, 1.);
    q1.pts[3].init(0, iy, nz, 0, 1.);
    area->addUVQuad(q1);


}


void CubeRenderer::createTexture(const Cube& cube, GLuint* tex, int slice){

    Slicer2D* slicer = nullptr;

    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());

    switch(slice){
    default:
    case Slice::XY:
        slicer = new ZSlicer2D(cube, cube_attribute->getCursorZ());
        break;
    case Slice::YZ:
        slicer = new XSlicer2D(cube, cube_attribute->getCursorX());
        break;
    case Slice::XZ:
        slicer = new YSlicer2D(cube, cube_attribute->getCursorY());
    }

    ColorMap* cmap = colormap();

    int nx = slicer->getNx();
    int ny = slicer->getNy();

    GLuint opacity = cube_attribute->opacity() * 255.;

    GLuint*  image = new GLuint[nx*ny];
    for( int y=0; y<ny; y++ ){
        for( int x=0; x<nx; x++ ){
            double val = slicer->getValue(x, y);
            Color4ub color;
            cmap->getGlColor(val, color);
            color[3] = qMin<GLuint>(opacity, color[3]);
            image[x+y*nx]   = ((color[0] << 24) | (color[1] << 16) | (color[2] << 8) | (color[3] << 0));
        }
    }


    glBindTexture(GL_TEXTURE_2D, tex[slice]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);  //Always set the base and max mipmap levels of a texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nx, ny, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, (GLvoid*)image);

    delete [] image;
}


void
CubeRenderer::pick(Pick& pick){
    CubeModel* cube_model = dynamic_cast<CubeModel*>( model() );
    if ( nullptr == cube_model ){
        return;
    }

    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());
    if ( nullptr == cube_attribute ){

        return;
    }

    Cube& cube = cube_model->cube();
    int nx = cube.getNx();
    int ny = cube.getNy();
    int nz = cube.getNz();

    QList<double> t_list;
    double t;
    {
        Point3d v0 = {0, 0, cube_attribute->getCursorZ()};
        Point3d v1 = {0, ny, cube_attribute->getCursorZ()};
        Point3d v2 = {nx, ny, cube_attribute->getCursorZ()};
        Point3d v3 = {nx, 0, cube_attribute->getCursorZ()};
        if ( pick.is_quad_picked(v0, v1, v2, v3, t) ){
            t_list.append(t);
        }
    }
    {
        Point3d v0 = {0, cube_attribute->getCursorY(), 0};
        Point3d v1 = {0, cube_attribute->getCursorY(), nz};
        Point3d v2 = {nx, cube_attribute->getCursorY(), nz};
        Point3d v3 = {nx, cube_attribute->getCursorY(), 0};
        if ( pick.is_quad_picked(v0, v1, v2, v3, t) ){
            t_list.append(t);
        }
    }
    {
        Point3d v0 = {cube_attribute->getCursorX(), 0, 0};
        Point3d v1 = {cube_attribute->getCursorX(), 0, nz};
        Point3d v2 = {cube_attribute->getCursorX(), ny, nz};
        Point3d v3 = {cube_attribute->getCursorX(), ny, 0};
        if ( pick.is_quad_picked(v0, v1, v2, v3, t) ){
            t_list.append(t);
        }
    }

    if (!t_list.isEmpty()){
        qSort(t_list);
        PickInfo* pinfo = new CubePickInfo();
        pinfo->setCurvi(t_list.at(0));
        pinfo->setModel(cube_model);
        pick.addPickInfo(pinfo);
    }

}


QString
CubePickInfo::toString(const Pick& pick) const {
    CubeModel* cube_model = dynamic_cast<CubeModel*>( getModel() );
    if ( nullptr == cube_model ){
        return QString();
    }
    Cube& cube = cube_model->cube();

    Point3d impact = pick.impact(this);
    int i = (int)impact[0];
    int j = (int)impact[1];
    int k = (int)impact[2];

    return QString("(%1, %2, %3) : %4").arg(i).arg(j).arg(k).arg(cube.getValue(i, j, k));
}

}

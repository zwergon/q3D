#include "cube_renderer.h"

#include <q3D/model/geometry.h>
#include <q3D/model/picking.h>
#include <q3D/model/renderer_area.h>

#include <q3D/gui/glu.h>

#include <q3D/cube/slicer.h>
#include <q3D/cube/cube_api.h>

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
    cube_attribute->setCursorX(cube.nx()/4);
    cube_attribute->setCursorY(cube.ny()/3);
    cube_attribute->setCursorZ(cube.nz()/2);
    cube_attribute->blockSignals(false);

    double min, max;
    CubeAPI::extrema(&cube, min, max);

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

        const double* ord = cube.origin();

        double idd[3];
        idd[0] = cube.ijk2xyz(cube_attribute->getCursorX(), 0);
        idd[1] = cube.ijk2xyz(cube_attribute->getCursorY(), 1);
        idd[2] = cube.ijk2xyz(cube_attribute->getCursorZ(), 2);

        double mad[3];
        mad[0] = cube.ijk2xyz( cube.nx(), 0 );
        mad[1] = cube.ijk2xyz( cube.ny(), 1 );
        mad[2] = cube.ijk2xyz( cube.nz(), 2 );

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_[Slice::XY]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3d(ord[0], ord[1], idd[2]);
        glTexCoord2i(0, 1); glVertex3d(ord[0], mad[1], idd[2]);
        glTexCoord2i(1, 1); glVertex3d(mad[0], mad[1], idd[2]);
        glTexCoord2i(1, 0); glVertex3d(mad[0], ord[1], idd[2]);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture_[Slice::XZ]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3d(ord[0], idd[1], ord[2]);
        glTexCoord2i(0, 1); glVertex3d(ord[0], idd[1], mad[2]);
        glTexCoord2i(1, 1); glVertex3d(mad[0], idd[1], mad[2]);
        glTexCoord2i(1, 0); glVertex3d(mad[0], idd[1], ord[2]);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture_[Slice::YZ]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3d(idd[0], ord[1], ord[2]);
        glTexCoord2i(0, 1); glVertex3d(idd[0], ord[1], mad[2]);
        glTexCoord2i(1, 1); glVertex3d(idd[0], mad[1], mad[2]);
        glTexCoord2i(1, 0); glVertex3d(idd[0], mad[1], ord[2]);
        glEnd();

        glDisable(GL_TEXTURE_2D);


    }
}

void CubeRenderer::createUVQuad(const Cube& cube, GLuint* tex, RendererArea* area) {

    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(attribute());

    int nx = cube.nx();
    int ny = cube.ny();
    int nz = cube.nz();

    int ix = cube_attribute->getCursorX();
    int iy = cube_attribute->getCursorY();
    int iz = cube_attribute->getCursorZ();

    const double* ord = cube.origin();

    double idd[3];
    idd[0] = cube.ijk2xyz(ix, 0);
    idd[1] = cube.ijk2xyz(iy, 1);
    idd[2] = cube.ijk2xyz(iz, 2);

    double mad[3];
    mad[0] = cube.ijk2xyz( nx, 0 );
    mad[1] = cube.ijk2xyz( ny, 1 );
    mad[2] = cube.ijk2xyz( nz, 2 );

    float ax = (float)ix /(float)nx;
    float ay = (float)iy /(float)ny;
    float az = (float)iz /(float)nz;

    UVQuad q1;

    //-----------
    q1.tex = tex[Slice::XY];
    q1.id = reinterpret_cast<int>(this);
    q1.pts[0].init(ord[0], ord[1], idd[2], 0, 0);
    q1.pts[1].init(idd[0], ord[1], idd[2], ax, 0);
    q1.pts[2].init(idd[0], idd[1], idd[2], ax, ay);
    q1.pts[3].init(ord[0], idd[1], idd[2], 0, ay);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], ord[1], idd[2], ax, 0);
    q1.pts[1].init(mad[0], ord[1], idd[2], 1, 0);
    q1.pts[2].init(mad[0], idd[1], idd[2], 1., ay);
    q1.pts[3].init(idd[0], idd[1], idd[2], ax, ay);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], idd[1], idd[2], ax, ay);
    q1.pts[1].init(mad[0], idd[1], idd[2], 1., ay);
    q1.pts[2].init(mad[0], mad[1], idd[2], 1., 1.);
    q1.pts[3].init(idd[0], mad[1], idd[2], ax, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(ord[0], idd[1], idd[2], 0, ay);
    q1.pts[1].init(idd[0], idd[1], idd[2], ax, ay);
    q1.pts[2].init(idd[0], mad[1], idd[2], ax, 1.);
    q1.pts[3].init(ord[0], mad[1], idd[2], 0, 1.);
    area->addUVQuad(q1);

    //-----------
    q1.tex = tex[Slice::YZ];
    q1.pts[0].init(idd[0], ord[1], ord[2], 0, 0);
    q1.pts[1].init(idd[0], idd[1], ord[2], ay, 0);
    q1.pts[2].init(idd[0], idd[1], idd[2], ay, az);
    q1.pts[3].init(idd[0], ord[1], idd[2], 0, az);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], idd[1], ord[2], ay, 0);
    q1.pts[1].init(idd[0], mad[1], ord[2], 1, 0);
    q1.pts[2].init(idd[0], mad[1], idd[2], 1., az);
    q1.pts[3].init(idd[0], idd[1], idd[2], ay, az);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], idd[1], idd[2], ay, az);
    q1.pts[1].init(idd[0], mad[1], idd[2], 1., az);
    q1.pts[2].init(idd[0], mad[1], mad[2], 1., 1.);
    q1.pts[3].init(idd[0], idd[1], mad[2], ay, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], ord[1], idd[2], 0, az);
    q1.pts[1].init(idd[0], idd[1], idd[2], ay, az);
    q1.pts[2].init(idd[0], idd[1], mad[2], ay, 1.);
    q1.pts[3].init(idd[0], ord[1], mad[2], 0, 1.);
    area->addUVQuad(q1);

    //-----------
    q1.tex = tex[Slice::XZ];

    q1.pts[0].init(ord[0], idd[1], ord[2], 0, 0);
    q1.pts[1].init(idd[0], idd[1], ord[2], ax, 0);
    q1.pts[2].init(idd[0], idd[1], idd[2], ax, az);
    q1.pts[3].init(ord[0], idd[1], idd[2], 0, az);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], idd[1], ord[2], ax, 0);
    q1.pts[1].init(mad[0], idd[1], ord[2], 1, 0);
    q1.pts[2].init(mad[0], idd[1], idd[2], 1., az);
    q1.pts[3].init(idd[0], idd[1], idd[2], ax, az);
    area->addUVQuad(q1);

    q1.pts[0].init(idd[0], idd[1], idd[2], ax, az);
    q1.pts[1].init(mad[0], idd[1], idd[2], 1., az);
    q1.pts[2].init(mad[0], idd[1], mad[2], 1., 1.);
    q1.pts[3].init(idd[0], idd[1], mad[2], ax, 1);
    area->addUVQuad(q1);

    q1.pts[0].init(ord[0], idd[1], idd[2], 0, az);
    q1.pts[1].init(idd[0], idd[1], idd[2], ax, az);
    q1.pts[2].init(idd[0], idd[1], mad[2], ax, 1.);
    q1.pts[3].init(ord[0], idd[1], mad[2], 0, 1.);
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

    const double* ord = cube.origin();

    double idd[3];
    idd[0] = cube.ijk2xyz(cube_attribute->getCursorX(), 0);
    idd[1] = cube.ijk2xyz(cube_attribute->getCursorY(), 1);
    idd[2] = cube.ijk2xyz(cube_attribute->getCursorZ(), 2);

    double mad[3];
    mad[0] = cube.ijk2xyz( cube.nx(), 0 );
    mad[1] = cube.ijk2xyz( cube.ny(), 1 );
    mad[2] = cube.ijk2xyz( cube.nz(), 2 );

    QList<double> t_list;
    double t;
    {
        Point3d v0 = {ord[0], ord[1], idd[2]};
        Point3d v1 = {ord[0], mad[1], idd[2]};
        Point3d v2 = {mad[0], mad[1], idd[2]};
        Point3d v3 = {mad[0], ord[1], idd[2]};
        if ( pick.is_quad_picked(v0, v1, v2, v3, t) ){
            t_list.append(t);
        }
    }
    {
        Point3d v0 = {ord[0], idd[1], ord[2]};
        Point3d v1 = {ord[0], idd[1], mad[2]};
        Point3d v2 = {mad[0], idd[1], mad[2]};
        Point3d v3 = {mad[0], idd[1], ord[2]};
        if ( pick.is_quad_picked(v0, v1, v2, v3, t) ){
            t_list.append(t);
        }
    }
    {
        Point3d v0 = {idd[0], ord[1], ord[2]};
        Point3d v1 = {idd[0], ord[1], mad[2]};
        Point3d v2 = {idd[0], mad[1], mad[2]};
        Point3d v3 = {idd[0], mad[1], ord[2]};
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
    int i = cube.xyz2ijk(impact[0], 0);
    int j = cube.xyz2ijk(impact[1], 1);
    int k = cube.xyz2ijk(impact[2], 2);

    return QString("(%1, %2, %3) : %4").arg(i).arg(j).arg(k).arg(cube.value(i, j, k));
}

}

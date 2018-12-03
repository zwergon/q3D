#include "cube_renderer.h"

#include <q3D/cube/cube_model.h>

#include <q3D/cube/slicer.h>
#include <q3D/cube/cube_renderer_attribute.h>


namespace Q3D {
CubeRenderer::CubeRenderer()
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

    colormap()->setMinMax( cube_model->mini()[2], cube_model->maxi()[2] );

}

void CubeRenderer::buildGlList(){

    if (glIsList(gl_list_))
        glDeleteLists(gl_list_,1);

    gl_list_=glGenLists(1);

    glNewList( gl_list_, GL_COMPILE );

    draw();

    glEndList();

}

void CubeRenderer::draw(){
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

    glEnable(GL_TEXTURE_2D);

    GLuint* tex = new GLuint[Slice::LAST];
    glGenTextures(Slice::LAST, tex);

    createTexture(cube, Slice::YZ, tex[Slice::YZ]);
    createTexture(cube, Slice::XZ, tex[Slice::XZ]);
    createTexture(cube, Slice::XY, tex[Slice::XY]);

    glColor3f(1., 1., 1.);
    glPolygonMode(GL_FRONT, GL_FILL);

    glBindTexture(GL_TEXTURE_2D, tex[Slice::XY]);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex3i(0, 0, cube_attribute->getCursorZ());
    glTexCoord2i(0, 1); glVertex3i(0, ny, cube_attribute->getCursorZ());
    glTexCoord2i(1, 1); glVertex3i(nx, ny, cube_attribute->getCursorZ());
    glTexCoord2i(1, 0); glVertex3i(nx, 0, cube_attribute->getCursorZ());
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[Slice::XZ]);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex3i(0, cube_attribute->getCursorY(), 0);
    glTexCoord2i(0, 1); glVertex3i(0, cube_attribute->getCursorY(), nz);
    glTexCoord2i(1, 1); glVertex3i(nx, cube_attribute->getCursorY(), nz);
    glTexCoord2i(1, 0); glVertex3i(nx, cube_attribute->getCursorY(), 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[Slice::YZ]);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex3i(cube_attribute->getCursorX(), 0, 0);
    glTexCoord2i(0, 1); glVertex3i(cube_attribute->getCursorX(), 0, nz);
    glTexCoord2i(1, 1); glVertex3i(cube_attribute->getCursorX(), ny, nz);
    glTexCoord2i(1, 0); glVertex3i(cube_attribute->getCursorX(), ny, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(Slice::LAST, tex);
    glDisable(GL_TEXTURE_2D);

}

void CubeRenderer::createTexture(const Cube& cube, Slice slice, GLuint tId){

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

    GLuint*  image = new GLuint[nx*ny];
    for( int y=0; y<ny; y++ ){
        for( int x=0; x<nx; x++ ){
            quint8 val = slicer->getValue(x, y);
            GlColor3uv color;
            cmap->getGlColor(val, color);
            image[x+y*nx]   = ((color[0] << 24) | (color[1] << 16) | (color[2] << 8) | (255 << 0));
        }
    }


    glBindTexture(GL_TEXTURE_2D, tId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);  //Always set the base and max mipmap levels of a texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nx, ny, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, (GLvoid*)image);

    delete [] image;
}

}

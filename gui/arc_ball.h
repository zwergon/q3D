#ifndef ARC_BALL_H
#define ARC_BALL_H

#include <q3D/gui/gui_global.h>

#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QtOpenGL>

namespace Q3D {

class GUISHARED_EXPORT ArcBall
{
public:
    ArcBall();

    void getRotation4f(GLfloat* rotation);

    void reset();
    void setBounds(float width, float height);
    void click(int x, int y);
    void drag(int x, int y);

private:
    QVector3D mouseToSphere(float x, float y);

private:
    QVector3D   down_pt_;
    QQuaternion q_now_;

    float adjust_width_;
    float adjust_height_;
};

}
#endif // ARC_BALL_H

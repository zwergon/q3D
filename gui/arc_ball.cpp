#include "arc_ball.h"


namespace Q3D {

ArcBall::ArcBall()
    : adjust_height_(1.),
      adjust_width_(1.),
      q_now_() //identity
{
}

void ArcBall::setBounds(float width, float height){
    // Set adjustment factor for width/height
    adjust_width_ = 2. / ((float)width - 1.) ;
    adjust_height_ = 2. /((float)height - 1.);
}

QVector3D ArcBall::mouseToSphere(float x, float y){

    QVector2D v2(x*adjust_width_ - 1., 1. - y*adjust_height_);

    QVector3D v3(v2.x(), v2.y(), 0);

    float mag = v2.lengthSquared();
    if (mag>1.0){
        v3.normalize();
    }
    else {
        v3.setZ(sqrt(1. - mag));
    }

    return v3;
}

void ArcBall::reset(){
    q_now_ = QQuaternion();
}

void ArcBall::click(int x, int y){
    down_pt_ = mouseToSphere((float)x, (float)y);
}


void ArcBall::drag(int x, int y){
    QVector3D new_pt = mouseToSphere((float)x, (float)y);

    QQuaternion q_drag = QQuaternion::rotationTo(new_pt, down_pt_);

    q_now_ = q_now_*q_drag;
    down_pt_ = new_pt;
}

void ArcBall::getRotation4f(GLfloat* rotation)
{
    // Quaternion (x, y, z, w)
  q_now_.normalize();

  double xx = q_now_.x() * q_now_.x(), xy = q_now_.x() * q_now_.y(),
         xz = q_now_.x() * q_now_.z(), xw = q_now_.x() * q_now_.scalar();
  double yy = q_now_.y() * q_now_.y(), yz = q_now_.y() * q_now_.z(),
         yw = q_now_.y() * q_now_.scalar();
  double zz = q_now_.z() * q_now_.z(), zw = q_now_.z() * q_now_.scalar();

  // Column major order
  rotation[0] = 1.0f - 2.0f * (yy + zz);
  rotation[1] = 2.0f * (xy - zw);
  rotation[2] = 2.0f * (xz + yw);
  rotation[3] = 0;

  rotation[4] = 2.0f * (xy + zw);
  rotation[5] = 1.0f - 2.0f * (xx + zz);
  rotation[6] = 2.0f * (yz - xw);
  rotation[7] = 0;

  rotation[8] = 2.0f * (xz - yw);
  rotation[9] = 2.0f * (yz + xw);
  rotation[10] = 1.0f - 2.0f * (xx + yy);
  rotation[11] = 0;

  rotation[12] = 0;
  rotation[13] = 0;
  rotation[14] = 0;
  rotation[15] = 1;
}

}



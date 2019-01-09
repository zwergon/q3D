#include "picking.h"

#include <q3D/model/geometry.h>

namespace Q3D {

Pick::~Pick(){
    foreach(PickInfo* pi, pinfos_){
        delete pi;
    }
    pinfos_.clear();
}

void Pick::setPickRay(const Point3d& origin, const Point3d& ray ){
    origin_ = origin;
    ray_ = ray;
}

bool Pick::is_trgl_picked(
        const Point3d& v0,
        const Point3d& v1,
        const Point3d& v2,
        double &t) const {
    return Geometry::rayIntersectsTriangle( origin_, ray_, v0, v1, v2, t);
}

bool Pick::is_quad_picked(
        const Point3d& v0,
        const Point3d& v1,
        const Point3d& v2,
        const Point3d& v3,
        double &t) const {

    if ( is_trgl_picked(v0, v1, v2, t) ){
        return true;
    }
    else {
        if ( is_trgl_picked(v0, v2, v3, t) ){
           return true;
        }
    }

    return false;
}

void Pick::addPickInfo( PickInfo* pinfo ){
    pinfos_.append(pinfo);
}

void Pick::sort(){
    qSort(pinfos_.begin(), pinfos_.end(), &PickInfo::lessThan);
}

Point3d Pick::impact(double t) const {
    Point3d impact;
    for(int i=0; i<3; i++){
        impact[i] = origin_[i] + t*ray_[i];
    }

    return impact;
}

Point3d Pick::impact(const PickInfo* pi) const {
    return impact(pi->curvi());
}

Point3d Pick::impactAt(int i) const {
    return impact(pinfos_.at(i));
}

QString Pick::stringAt(int i) const{
    return pinfos_.at(0)->toString(*this);
}



/********************************************/
PickInfo::PickInfo()
{
}

PickInfo::~PickInfo(){}

QString PickInfo::toString( const Pick& pick ) const{
    Point3d impact = pick.impact(this);

    return QString("%1 %2 %3").arg(impact[0]).arg(impact[1]).arg(impact[2]);
}

}


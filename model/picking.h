#ifndef PICKING_INFO_H
#define PICKING_INFO_H

#include <q3D/model/model_scope.h>

#include <q3D/model/point3d.h>

#include <QHash>

namespace Q3D {


/**************************************************/
class Model;
class Pick;

class MODELSHARED_EXPORT PickInfo
{
public:
    PickInfo();
    virtual ~PickInfo();

    void setCurvi(double curvi);
    double curvi() const { return curvi_;}

    Model* getModel() const {return model_;}
    void setModel( Model* model);

    virtual QString toString( const Pick& ) const;

    static bool lessThan(PickInfo* e1, PickInfo* e2);

private:
    double curvi_;
    Model* model_;
};

inline bool operator==(const PickInfo &e1, const PickInfo &e2)
{
    return AreSame(e1.curvi(), e2.curvi());
}

inline void PickInfo::setCurvi(double t){
    curvi_ = t;
}

inline void PickInfo::setModel(Model *model){
    model_ = model;
}

inline bool PickInfo::lessThan(PickInfo* e1, PickInfo* e2){
    return e1->curvi() < e2->curvi();
}

/**************************************************/

class MODELSHARED_EXPORT Pick {
public:
    ~Pick();

    void setPickRay(const Point3d& origin, const Point3d& ray );
    const Point3d& origin() const;
    const Point3d& ray() const;

    bool is_trgl_picked(
            const Point3d& v0,
            const Point3d& v1,
            const Point3d& v2,
            double &t) const;

    bool is_quad_picked(
            const Point3d& v0,
            const Point3d& v1,
            const Point3d& v2,
            const Point3d& v3,
            double &t) const;

    bool isSucceeded() const { return !pinfos_.isEmpty();}

    void addPickInfo( PickInfo* pinfo );
    void sort();

    int pickCount() const {return pinfos_.size();}
    PickInfo* pickAt(int i) const {return pinfos_.at(i);}
    QString stringAt(int i) const;

    Point3d impact(double t) const;
    Point3d impact(const PickInfo* pi) const;
    Point3d impactAt(int i) const;



private:
    QList<PickInfo*> pinfos_;
    Point3d origin_;
    Point3d ray_;
};

inline const Point3d& Pick::origin() const{
    return origin_;
}

inline const Point3d& Pick::ray() const{
    return ray_;
}

}

inline uint qHash(const Q3D::PickInfo &key, uint seed)
{
    return qHash(key.curvi(), seed);
}



#endif // PICKING_INFO_H

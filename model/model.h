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
 * $Id: model.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_MODEL_MODEL_H_
#define Q3D_MODEL_MODEL_H_ 1

#include <QtCore/QObject>
#include <QtCore/QString>

#include <q3D/model/model_scope.h>

#include <q3D/model/point3d.h>

class QMenu;

namespace Q3D {

class ModelDriver;

/********************************************************/

class MODELSHARED_EXPORT ModelData {
public:
    virtual ~ModelData(){}
};


/********************************************************/

class MODELSHARED_EXPORT Model : public QObject
{
	Q_OBJECT

public:
    Model(){}
    virtual ~Model(){}

    void setDriver( ModelDriver* driver );
    ModelDriver* driver() const { return driver_; }

    const Point3d& mini() const;
    const Point3d& maxi() const;
	
    virtual void update();
    virtual void popupMenu( QMenu* );

    virtual QString tooltip() const;

signals:
    void modelUpdated( Model* );

protected:
    ModelDriver*  driver_;
    Point3d    min_;
    Point3d    max_;
};

inline const Point3d& Model::mini() const {
	return min_; 
}

inline const Point3d& Model::maxi() const {
	return max_; 
}

inline QString Model::tooltip() const {
    return objectName();
}


}


#endif /* Q3D_MODEL_MODEL_H_ */


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

#include <q3D/model/types.h>

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

    const Point3f& mini() const;
    const Point3f& maxi() const;
	
    virtual void update();
    virtual void popupMenu( QMenu* );

signals:
    void modelUpdated( Model* );

protected:
    ModelDriver*  driver_;
    Point3f    min_;
    Point3f    max_;
};

inline const Point3f& Model::mini() const {
	return min_; 
}

inline const Point3f& Model::maxi() const {
	return max_; 
}


}


#endif /* Q3D_MODEL_MODEL_H_ */


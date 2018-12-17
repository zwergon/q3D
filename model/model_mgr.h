#ifndef Q3D_MODEL_MANAGER_H
#define Q3D_MODEL_MANAGER_H

#include <q3D/model/model_scope.h>

#include <q3D/model/model.h>
#include <q3D/model/model_open_info.h>

#include <QObject>
#include <QList>

namespace Q3D {

class MODELSHARED_EXPORT ModelManager : public QObject
{
     Q_OBJECT
public:
    static ModelManager*  instance();

public:
    const QList<Model*>& models() const;

    Model* loadModel( const ModelOpenInfo&  );
    void removeModel( Model* model );

signals:
    void modelAdded( Model* );
    void modelRemoved( Model* );

private:
    QList<Model*> models_;
    static ModelManager* instance_;
};

inline const QList<Model*>& ModelManager::models() const {
    return models_;
}

}

#endif // Q3D_MODEL_MANAGER_H

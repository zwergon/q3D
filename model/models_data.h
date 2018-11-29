#ifndef MODELS_DATA_H
#define MODELS_DATA_H

#include <q3D/model/model_scope.h>

#include <q3D/model/model.h>

#include <QObject>
#include <QList>

namespace Q3D {

class MODELSHARED_EXPORT ModelsData : public QObject
{
     Q_OBJECT
public:
    const QList<Model*>& models() const;

    Model* loadModel( const QString& filename );
    void removeModel( Model* model );

signals:
    void modelAdded( Model* );
    void modelRemoved( Model* );

private:
    QList<Model*> models_;
};

inline const QList<Model*>& ModelsData::models() const {
    return models_;
}

}

#endif // MODELS_DATA_H

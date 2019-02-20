#ifndef PROCESS_LOG_H
#define PROCESS_LOG_H

#include <q3D/model/model_scope.h>

#include <QObject>

namespace Q3D {

class MODELSHARED_EXPORT ProcessLog : public QObject
{
    Q_OBJECT
public:
    explicit ProcessLog(QObject *parent = nullptr);

signals:

public slots:
    virtual void log( const QString& ) const;
    virtual void error( const QString& ) const;
};

}

#endif // PROCESS_LOG_H

#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <q3D/model/model_scope.h>

#include <QtXml/QDomDocument>
#include <QVariant>
#include <QFileInfo>
#include <QDir>
#include <QPointer>

#include <q3D/model/model.h>

namespace Q3D {

/**********************************************/

class MODELSHARED_EXPORT ProcessParam {
public:
        ProcessParam(const QString& name = QString::null, const QString& value = QString::null );

        const QString& name() const { return name_; }
        const QString& value() const { return value_; }

private:
        QString name_;
        QString value_;
};

/************************************************/

class MODELSHARED_EXPORT ProcessInfo
{
public:
    ProcessInfo( const QString& name  );

    const QString& name() const { return name_; }

    void setModel( Model* model );
    Model* model() const;

    void setProcessExe( const QString& dir, const QString& exe );
    const QFileInfo& processExe() const { return process_exe_; }

    void addParam( const QString& name, const QVariant& value );
    const QHash<QString, ProcessParam>& getParams() const { return params_; }

    ProcessParam getParam(const QString& name ) const;

protected:
    QString name_;
    QPointer<Model> model_;
    QFileInfo process_exe_;
    QHash<QString, ProcessParam> params_;
};

inline void ProcessInfo::addParam(const QString &name, const QVariant &value){
    ProcessParam param(name, value.toString());
    params_.insert(name, param);
}

inline void ProcessInfo::setProcessExe(const QString &dir, const QString &exe){
    process_exe_ = QFileInfo(QDir(dir), exe);
}

inline ProcessParam ProcessInfo::getParam(const QString& name ) const {
    return params_[name];
}

inline void ProcessInfo::setModel( Model* model ){
    model_ = model;
}

inline Model* ProcessInfo::model() const {
    return model_.data();
}

}

#endif // PROCESS_INFO_H
#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <q3D/model/model_scope.h>

#include <QtXml/QDomDocument>
#include <QVariant>
#include <QFileInfo>
#include <QDir>
#include <QPointer>

#include <q3D/model/model.h>
#include <q3D/model/params_info.h>

namespace Q3D {


/************************************************/

class MODELSHARED_EXPORT ProcessInfo
{
public:
    ProcessInfo( const QString& name  );

    const QString& name() const { return name_; }

    void setModel( Model* model );
    Model* model() const;

    void setProcessExe( const QFileInfo& );
    const QFileInfo& processExe() const { return process_exe_; }

    void addParam( const QString& name, const QVariant& value );
    const QHash<QString, ParamsElement>& getParams() const { return params_; }

    bool getParam(const QString& name, ParamsElement&) const;

protected:
    QString name_;
    QPointer<Model> model_;
    QFileInfo process_exe_;
    QHash<QString, ParamsElement> params_;
};

inline void ProcessInfo::addParam(const QString &name, const QVariant &value){
    ParamsElement param;
    param.setName(name);
    param.setValue(value.toString());
    params_.insert(name, param);
}

inline void ProcessInfo::setProcessExe(const QFileInfo& file_info){
    process_exe_ = file_info;
}

inline bool ProcessInfo::getParam(const QString& name, ParamsElement& process_param) const {
    if (!params_.contains(name)){
        return false;
    }
    process_param = params_[name];
    return true;
}

inline void ProcessInfo::setModel( Model* model ){
    model_ = model;
}

inline Model* ProcessInfo::model() const {
    return model_.data();
}

}

#endif // PROCESS_INFO_H

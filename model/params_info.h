#ifndef PARAMS_INFO_H
#define PARAMS_INFO_H

#include <q3D/model/model_scope.h>

#include <QDomDocument>
#include <QDomElement>
#include <QHash>

namespace Q3D {

class MODELSHARED_EXPORT ParamsElement {
public:
    ParamsElement();
    ParamsElement(
            const QString& type,
            const QString& id,
            const QString& name,
            const QString& value,
            const QString& desc );

    QDomElement getDomElement(QDomDocument& doc);

    void setId( const QString& id );
    QString id() const;

    void setName( const QString& name );
    QString name() const;

    void setValue( const QString& value );
    QString value() const;

private:
    QHash<QString, QString> keyValue_;
};

inline void ParamsElement::setId(const QString& id ){
    keyValue_["Id"] = id;
}

inline QString ParamsElement::id() const {
    return keyValue_["Id"];
}

inline void ParamsElement::setName(const QString& name ){
    keyValue_["Name"] = name;
}

inline QString ParamsElement::name() const {
    return keyValue_["Name"];
}

inline void ParamsElement::setValue(const QString& value){
    keyValue_["Value"] = value;
}

inline QString ParamsElement::value() const{
    return keyValue_["Value"];
}

class MODELSHARED_EXPORT ParamsInfo {
public:
    QDomDocument getDomDocument();
protected:
    QList<ParamsElement> elements_;
};

}

#endif // PARAMS_INFO_H

#include "params_info.h"

namespace Q3D {

ParamsElement::ParamsElement(){
}

ParamsElement::ParamsElement(
        const QString &type,
        const QString &id,
        const QString &name,
        const QString &value,
        const QString &desc ){
    keyValue_["Type"] = type;
    keyValue_["Id"]  = id;
    keyValue_["Name"] = name;
    keyValue_["Value"] = value;
    keyValue_["Desc"] = desc;
}

QDomElement ParamsElement::getDomElement(QDomDocument &doc){
    QDomElement elt = doc.createElement("Param");
    foreach (auto key, keyValue_.keys()) {
        elt.setAttribute(key, keyValue_[key]);
    }
    return elt;
}

QDomDocument ParamsInfo::getDomDocument(){
    QDomDocument doc;
    QDomElement root = doc.createElement("Root");
    foreach( auto p_elt, elements_){
        root.appendChild(p_elt.getDomElement(doc));
    }
    doc.appendChild(root);
    return doc;
}

}

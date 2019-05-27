#include "widget_creator.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

namespace Q3D {

/*****************************************************/

QWidget* BoolBuilder::create(const QDomElement &element, QWidget* parent){
    QCheckBox* cb = new QCheckBox(parent);
    cb->setText(element.attribute("Name", "CheckBox"));
    cb->setToolTip(element.attribute("Desc"));
    cb->setChecked( element.attribute("Value") == "true" );
    cb->setObjectName(element.attribute("Id"));

    return cb;
}

QString BoolBuilder::getValue(QWidget* widget) const{
    QCheckBox* cb = qobject_cast<QCheckBox*>(widget);
    if ( cb != nullptr ){
        return ( cb->isChecked() ) ? "true" : "false";
    }
    return QString::null;
}
/*****************************************************/

QWidget* IntegerBuilder::create(const QDomElement &element, QWidget* parent){

    QGroupBox* group = new QGroupBox(parent);
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label = new QLabel(group);
    label->setText(element.attribute("Name"));
    layout->addWidget(label);

    QSpinBox* sb = new QSpinBox(group);
    sb->setValue(element.attribute("Value", "0").toInt());
    sb->setMaximum(element.attribute("Max", "100").toInt());
    sb->setMinimum(element.attribute("Min", "0").toInt());
    sb->setSingleStep(element.attribute("Incr", "1").toInt());
    sb->setObjectName(element.attribute("Id"));
    layout->addWidget(sb);

    group->setLayout(layout);

    return group;
}

QString IntegerBuilder::getValue(QWidget* widget) const{
    QSpinBox* sb = qobject_cast<QSpinBox*>(widget);
    if ( sb != nullptr ){
        return QString::number(sb->value());
    }
    return QString::null;
}


/*****************************************************/

QWidget* DoubleBuilder::create(const QDomElement &element, QWidget* parent){

    QGroupBox* group = new QGroupBox(parent);
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label = new QLabel(group);
    label->setText(element.attribute("Name"));
    layout->addWidget(label);

    QDoubleSpinBox* sb = new QDoubleSpinBox(group);
    sb->setValue(element.attribute("Value", "0").toDouble());
    sb->setMaximum(element.attribute("Max", "100").toDouble());
    sb->setMinimum(element.attribute("Min", "0").toDouble());
    sb->setSingleStep(element.attribute("Incr", "1").toDouble());
    sb->setObjectName(element.attribute("Id"));
    layout->addWidget(sb);

    group->setLayout(layout);

    return group;
}

QString DoubleBuilder::getValue(QWidget* widget) const{
    QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(widget);
    if ( sb != nullptr ){
        return QString::number(sb->value());
    }
    return QString::null;
}


/*****************************************************/

QWidget* WidgetCreator::create(const QDomElement &element, QWidget* parent){

    QString type = element.attribute("Type");
    if (!builders_.contains(type)){
        return nullptr;
    }
    return builders_[type]->create(element, parent);
}

QString WidgetCreator::getValue(const QDomElement &element, QWidget* w) const {
    QString type = element.attribute("Type");
    if (!builders_.contains(type)){
        return QString::null;
    }
    return builders_[type]->getValue(w);
}


WidgetCreator::WidgetCreator(){
    builders_.insert("Bool", new BoolBuilder);
    builders_.insert("Double", new DoubleBuilder);
    builders_.insert("Integer", new IntegerBuilder);
}

WidgetCreator::~WidgetCreator(){
    for( auto value : builders_ ){
        delete value;
    }
    builders_.clear();
}


}

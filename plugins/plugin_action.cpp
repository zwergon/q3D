#include "plugin_action.h"

#include <QDebug>

namespace Q3D {

PluginAction::PluginAction(int type, QObject *parent) :
    QObject(parent),
    type_(type),
    action_(new QAction(this))
{
    connect(action_, &QAction::triggered, this, &PluginAction::on_action_triggered);
}

PluginAction::~PluginAction(){
    //do nothing.
    qDebug() << "~PluginAction";
}

bool PluginAction::canWorkOn(Model*) const {
    return true; //by default can work on every kind of object
}

bool PluginAction::execute(Model* model){
    //do nothing by default
    return true;
}

void PluginAction::on_action_triggered(){
    emit activated(this);
}

}

#include "cube_model.h"

#include <q3D/cube/plugim_process.h>
#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_driver.h>

#include <q3D/model/process_mgr.h>
#include <QFileInfo>
#include <QDir>

namespace Q3D {

CubeModel::CubeModel(){
}

CubeModel::~CubeModel(){
    delete cube_;
}

void CubeModel::update(){

    for( int i=0; i<3; i++ ){
        min_[i] = 0;
    }

    max_[0] = cube_->getNx();
    max_[1] = cube_->getNy();
    max_[2] = cube_->getNz();

    Model::update();

}


void CubeModel::popupMenu(QMenu* parent){
    QMenu* menu = new QMenu("Cube", parent);
    if ( nullptr != menu ){
        menu->addAction( "Launch", this, &CubeModel::onCubeLaunchAction);
    }
    parent->addMenu(menu);
}

void CubeModel::onCubeLaunchAction(){

    qDebug() << "onCubeLauchAction";

    ProcessInfo pi("volumeBilateral3DSEP");
    pi.setModel(this);
    pi.setProcessExe(
                "C:\\Users\\lecomtje\\Desktop\\Plugim\\PlugIns\\volumeBilateral3DSEP",
                "volumeBilateral3DSEP.exe");
    pi.addParam("IN", "C:\\TEMP\\In.fda");
    pi.addParam("OUT", "C:\\TEMP\\Out.fda" );
    pi.addParam("flow", "true");
    pi.addParam("sigmaR", 49);
    pi.addParam("sigmaS", 20);

    PlugimProcessBuilder builder;
    ProcessManager::instance()->submitProcess(pi, builder);

}

}

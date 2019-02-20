#include "plugim_process.h"

#include <QDebug>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_driver.h>

#include <q3D/model/model_mgr.h>


namespace Q3D {

PlugImProcess::PlugImProcess(const ProcessInfo& pi ): Process(pi)
{
}

bool PlugImProcess::prepare() {

    Model* model = process_info_.model();
    if ( nullptr == model ){
        qDebug() << "unable to prepare, model is null";
        return false;
    }

    // 1. Create a copy of the data in IN directory
    ProcessParam inParam;
    process_info_.getParam("IN", inParam);
    ModelDriver* driver =
            ModelDriverManager::instance()->getDriverByName("FdaCubeDriver");
    driver->save(*model, inParam.value());

    // 2. Create the PAR xml file
    QDomDocument xml("Root");
    QDomElement root = xml.createElement("Root");
    xml.appendChild(root);

    for( ProcessParam param : process_info_.getParams().values() ){
        QDomElement tag = xml.createElement(param.name());
        root.appendChild(tag);
        tag.setAttribute("Value", param.value());
    }

    // 3. save it
    QFileInfo process_exe = process_info_.processExe();
    QFileInfo xml_param(
                process_exe.absoluteDir(),
                QString("%1PAR.xml").arg(process_info_.name()));
    QFile file( xml_param.absoluteFilePath() );
    if ( !file.open(QFile::WriteOnly | QFile::Text)){
        qCritical() << "unable to save xml parameter file";
        file.close();
        return false;
    }
    QTextStream output(&file);
    output << xml.toString();

    file.close();

    return true;
}

bool PlugImProcess::validate(){

    ProcessParam outParam;
    if ( process_info_.getParam("OUT", outParam) ){
        FileModelOpenInfo fmoi(outParam.value());
        ModelManager::instance()->loadModel(fmoi);

        QFile fiOut(outParam.value());
        fiOut.remove();
    }

    ProcessParam inParam;
    process_info_.getParam("IN", inParam);
    QFile fiIn(inParam.value());
    fiIn.remove();

    return true;
}

Process* PlugImProcessBuilder::createProcess(const ProcessInfo &pi) const {
    return new PlugImProcess(pi);
}

}

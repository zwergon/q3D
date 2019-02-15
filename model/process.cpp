#include "process.h"

#include <QDebug>

namespace Q3D {

Process::Process(const ProcessInfo& pi) :
    state_(IDLE),
    process_info_(pi)
{
    QObject::connect(&process_, (void (QProcess::*)(int))&QProcess::finished,
                     this, &Process::on_process_finished);
    QObject::connect(&process_, &QProcess::errorOccurred,
                     this, &Process::on_error_occured);
}

bool Process::launch(){
    qDebug() << "launch Process";

    QFileInfo process_exe = process_info_.processExe();
    process_.start(process_exe.absoluteFilePath());

    if (!process_.waitForStarted()){
        setState(Process::ERROR);
        return false;
    }

    setState(Process::RUNNING);

    return true;
}

void Process::on_process_finished(int code){

    if ( code != 0 ){
        qDebug() << "process exited with error code " << code;
        setState(Process::ERROR);
        return;
    }

    qDebug() << "process is finished";
    setState(Process::FINISHED);

    emit processEnded(this);

}

void Process::on_error_occured(QProcess::ProcessError error){
    qDebug() << "an error occured";
    setState(Process::ERROR);
}


}

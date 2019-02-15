#include "process_mgr.h"

#include <QDebug>

namespace Q3D {

ProcessManager* ProcessManager::instance_ = nullptr;


ProcessManager* ProcessManager::instance(){
    if ( instance_ == nullptr ){
        instance_ = new ProcessManager;
    }
    return instance_;
}

ProcessManager::ProcessManager() {
}

bool ProcessManager::submitProcess(
        const ProcessInfo& pi,
        const ProcessBuilder& builder
        ){

    Process* process = builder.createProcess(pi);
    if ( nullptr == process ){
        return false;
    }

    if ( !process->prepare() ){
        return false;
    }

    process->setState(Process::PREPARED);

    if (!process->launch()){
        return false;
    }

    QObject::connect( process, &Process::processEnded, this, &ProcessManager::on_process_ended);

    return true;
}

void ProcessManager::on_process_ended(Process* process){

    if ( process->state() == Process::FINISHED ){
        const ProcessInfo& process_info = process->processInfo();
        qDebug() << "process " << process_info.name() << " is ended";
        process->validate();
    }

    process->deleteLater();
}



}

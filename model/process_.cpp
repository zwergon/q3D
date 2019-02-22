#include "process_.h"

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
    QObject::connect(&process_, &QProcess::readyReadStandardOutput,
                     this, &Process::on_standard_output);
    QObject::connect(&process_, &QProcess::readyReadStandardError,
                     this, &Process::on_standard_error);
}

bool Process::launch(){
    qDebug() << "launch Process";

    QFileInfo process_exe = process_info_.processExe();
    if ( process_exe.suffix() == "py"){
        QStringList arguments { "-u" /*for direct flush of stdout*/, process_exe.absoluteFilePath()  };
        process_.start("D:\\lecomtje\\Utils\\Anaconda\\python.exe", arguments);
    }
    else {
        process_.start(process_exe.absoluteFilePath());
    }

    if (!process_.waitForStarted()){
        emit error(getMessage("unable to start"));
        setState(Process::ERROR);
        return false;
    }
    emit log(getMessage("started"));

    setState(Process::RUNNING);

    return true;
}

void Process::on_process_finished(int code){

    if ( code != 0 ){
        emit error(getMessage(QString("exited with %1 code").arg(code)));
        setState(Process::ERROR);
        return;
    }

    emit log(getMessage("finished"));
    setState(Process::FINISHED);

    emit processEnded(this);

}

void Process::on_error_occured(QProcess::ProcessError error){
    qDebug() << "an error occured";
    setState(Process::ERROR);
}

void Process::on_standard_output(){
    QProcess *p = qobject_cast<QProcess*>(sender());
    p->setReadChannel(QProcess::StandardOutput);
    QTextStream in(p);
    while(!in.atEnd())
    {
         emit log(getMessage(in.readLine()));
    }
}

void Process::on_standard_error(){
    QProcess *p = qobject_cast<QProcess*>(sender());
    p->setReadChannel(QProcess::StandardError);
    QTextStream in(p);
    while(!in.atEnd())
    {
         emit error(getMessage(in.readLine()));
    }
}

QString Process::getMessage( const QString& text ) const {
    return QString("[%1] : %2").arg(process_info_.name()).arg(text);
}


}

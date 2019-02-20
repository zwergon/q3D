#include "process_log.h"

#include <QDebug>

namespace Q3D {

ProcessLog::ProcessLog(QObject *parent) : QObject(parent)
{

}


void ProcessLog::log(const QString& text ) const{
    qDebug() << text;
}

void ProcessLog::error(const QString& text ) const{
    qDebug() << text;
}

}

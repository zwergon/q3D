#include "terminal_log.h"

namespace Q3D {

TerminalLog::TerminalLog(QTextEdit* te, QObject* parent) : ProcessLog(parent), text_edit_(te)
{
}


void TerminalLog::log(const QString &text) const {
    if ( text_edit_.data() != nullptr ){
        text_edit_->append(QString("<font color=\"blue\">%1</font>").arg(text));
    }
}

void TerminalLog::error(const QString &text) const {
    if ( text_edit_.data() != nullptr ){
        text_edit_->append( QString("<font color=\"red\">%1</font>").arg(text));
    }
}



}

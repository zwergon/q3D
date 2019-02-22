#ifndef TERMINAL_LOG_H
#define TERMINAL_LOG_H

#include <q3D/gui/gui_global.h>

#include <q3D/model/process_log.h>

#include <QTextEdit>
#include <QPointer>

namespace Q3D {

class GUISHARED_EXPORT TerminalLog : public ProcessLog
{
public:
    TerminalLog( QTextEdit* text_edit, QObject* parent = nullptr );

    virtual void log(const QString &) const override;
    virtual void error(const QString& ) const override;

private:
    QPointer<QTextEdit> text_edit_;
};

}

#endif // TERMINAL_LOG_H

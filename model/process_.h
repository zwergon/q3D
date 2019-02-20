#ifndef PROCESS_H
#define PROCESS_H

#include <q3D/model/model_scope.h>

#include <QObject>
#include <QProcess>
#include <q3D/model/process_info.h>

namespace Q3D {



class MODELSHARED_EXPORT Process : public QObject {
      Q_OBJECT
public:
    enum {
        IDLE,
        PREPARED,
        STARTING,
        RUNNING,
        FINISHED,
        ERROR
    };

public:
    void setState(int state);
    int state() const;

    const ProcessInfo& processInfo() const;

    virtual bool prepare() = 0;
    virtual bool validate() = 0;
    bool launch();

signals:
    void processEnded( Process* );
    void log(const QString&);
    void error(const QString&);

protected slots:
    void on_process_finished(int);
    void on_error_occured(QProcess::ProcessError);
    void on_standard_output();
    void on_standard_error();

protected:
    Process(const ProcessInfo& pi);

protected:
    int state_;
    ProcessInfo process_info_;
    QProcess process_;
};

inline int Process::state() const {
    return state_;
}

inline const ProcessInfo& Process::processInfo() const {
    return process_info_;
}

inline void Process::setState(int state){
    state_ = state;
}


class MODELSHARED_EXPORT ProcessBuilder {
public:
  virtual Process* createProcess( const ProcessInfo& pi ) const = 0;
};

}
#endif // PROCESS_H

#ifndef PROCESS_MGR_H
#define PROCESS_MGR_H

#include <q3D/model/model_scope.h>

#include <q3D/model/process_info.h>
#include <q3D/model/process.h>


namespace Q3D {


class MODELSHARED_EXPORT ProcessManager : public QObject
{
    Q_OBJECT
public:
    static ProcessManager* instance();

    bool submitProcess( const ProcessInfo& pi, const ProcessBuilder& builder );

private slots:
    void on_process_ended(Process*);

private:
    ProcessManager();
    static ProcessManager* instance_;
};

}

#endif // PROCESS_MGR_H

#ifndef BILATERAL_FILTER_H
#define BILATERAL_FILTER_H

#include <q3D/cube/cube_global.h>

#include <q3D/model/process.h>

namespace Q3D {

class CUBESHARED_EXPORT PlugimProcess : public Process
{
    Q_OBJECT
public:
    PlugimProcess(const ProcessInfo& pi);

    virtual bool prepare() override;
    virtual bool validate() override;
};


class CUBESHARED_EXPORT PlugimProcessBuilder : public ProcessBuilder {
public:
    virtual Process* createProcess(const ProcessInfo &pi) const;
};

}

#endif // BILATERAL_FILTER_H

#ifndef BILATERAL_FILTER_H
#define BILATERAL_FILTER_H

#include <q3D/plugim/plugim_global.h>

#include <q3D/model/process.h>

namespace Q3D {

class PLUGIMSHARED_EXPORT PlugImProcess : public Process
{
    Q_OBJECT
public:
    PlugImProcess(const ProcessInfo& pi);

    virtual bool prepare() override;
    virtual bool validate() override;
};


class PLUGIMSHARED_EXPORT PlugImProcessBuilder : public ProcessBuilder {
public:
    virtual Process* createProcess(const ProcessInfo &pi) const;
};

}

#endif // BILATERAL_FILTER_H

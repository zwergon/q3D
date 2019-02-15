#include "process_info.h"


namespace Q3D {

ProcessParam::ProcessParam(const QString &name, const QString &value)
    : name_(name),
      value_(value)
{

}

ProcessInfo::ProcessInfo( const QString& name ) : name_(name)
{
}

}

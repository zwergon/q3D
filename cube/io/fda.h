#ifndef FDA_H
#define FDA_H

#include <cube/cube_global.h>

#include <stdint.h>
#include <string>
#include <cube/cube.h>

class CUBESHARED_EXPORT FDA
{
public:
    static Cube* read( const std::string& filename );
    static void write( const Cube& cube, const std::string& filename );
};


#endif // FDA_H

#include "fda.h"

#include <iostream>
#include <fstream>
#include <string>


Cube* FDA::read(const std::string& fileName )
{

    if(fileName.substr(fileName.find_last_of(".") + 1) != "fda") {
        return nullptr;
    }

    std::ifstream is(fileName, std::ifstream::binary);
    if (!is)
    {
        std::cerr << "unable to open " << fileName << std::endl ;
        return  nullptr;
    }

    int32_t header[4];
    is.read((char*)header, 4*sizeof(int32_t));


    Cube* cube = Cube::create(header[0]);
    if ( cube == nullptr ){
        std::cerr << "type " << header[0] << " is not readable !" << std::endl;
        return nullptr;
    }

    cube->setSize(header[1], header[2], header[3]);

    char* buffer = new char[cube->byteSize()];

    is.read(buffer, cube->byteSize());
    if (!is){
        std::cerr << "error: only " << is.gcount() << " could be read";
        delete [] buffer;
        delete cube;
        return nullptr;
    }

    cube->setData(buffer);
    is.close();


    return cube;
}

void FDA::write( const Cube& cube, const std::string& filename ){

    std::ofstream os(filename, std::ofstream::binary);
    if (!os)
    {
        std::cerr << "unable to open " << filename << std::endl ;
        return;
    }

    int32_t header[4];
    header[0] = cube.type();
    header[1] = cube.nx();
    header[2] = cube.ny();
    header[3] = cube.nz();

    os.write((char*)header, 4*sizeof(int32_t));
    os.write((char*)cube.data(), cube.byteSize());

    os.close();

}

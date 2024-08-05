#include <iostream>
#include "IO.hpp"
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// create file stream object
IO::IO(const std::string &file_path)
{
    // create file stream
    file_stream.open(file_path, std::ios::in | std::ios::out | std::ios::binary);

    // handle errors
    if (!file_stream.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        exit(1);
    }
}

// return file stream
fstream IO::get_fileStream()
{
    return std::move(file_stream);
}

// stream distructor
IO::~IO()
{
    if (file_stream.is_open())
    {
        file_stream.close();
    }
}
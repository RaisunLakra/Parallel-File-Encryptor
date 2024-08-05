/*

* This fileHandling just create file stream and return it. At last it destroys itself before closing

Alternate :
fstream f_stream;
return f_stream.open(filename, std::ios::out, std::ios::in);

*/

#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>

class IO
{
private:
    std::fstream file_stream;

public:
    IO(const std::string &filename);
    ~IO();
    std::fstream get_fileStream();
};

#endif
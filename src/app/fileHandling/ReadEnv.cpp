#include <iostream>
#include <string>
#include <fstream>
#include "IO.hpp"
#include <sstream>

class ReadEnv
{
public:
    std::string getenv()
    {
        std::string env_path = ".env";
        IO io(env_path);
        std::fstream f_stream = io.get_fileStream();
        std::stringstream buffer;
        buffer << f_stream.rdbuf();
        std::string content = buffer.str();
        return content;
    }
};
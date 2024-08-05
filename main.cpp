#include <iostream>
#include <filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path : ";
    std::getline(std::cin, directory);

    std::cout << "Enter the action (Encrypt/Decrypt): ";
    std::getline(std::cin, action);

    try
    {
        if (fs::exists(directory) && fs::is_directory(directory))
        {
            ProcessManagement processManagement;
            for (const auto &entry : fs::recursive_directory_iterator(directory))
            {
                if (entry.is_regular_file())
                {
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = std::move(io.get_fileStream());
                    if (f_stream.is_open())
                    {
                        Action taskAction = (action == "ENCRYPT" || action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        processManagement.submitToQueue(std::move(task));
                    }
                    else
                    {
                        std::cerr << "Failed to open file: " << filePath << '\n';
                    }
                }
            }
            processManagement.executeTasks();
        }
        else
        {
            std::cerr << "Invalid directory path: " << directory << '\n';
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "FileSystem Error: " << e.what() << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
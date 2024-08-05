#include "ProcessManagement.hpp"
#include <iostream>
#include <string>
// #include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <mutex>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    taskQueue.push(std::move(task));
}

void ProcessManagement::executeTasks()
{
    while (!taskQueue.empty())
    {
        std::unique_ptr tasktoExecute = std::move(taskQueue.front());
        taskQueue.pop();
        std::cout << "Executing task: " << tasktoExecute->toString();

        // perform threading
        pid_t f = fork();
        if (f == 0)
        {
            executeCryption(tasktoExecute->toString());
        }
    }
}

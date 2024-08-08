#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    taskQueue.push(std::move(task));
    int pid = fork();
    if (pid < 0)
        return false;
    else if (pid > 0)
    {
        std::cout << "parent process" << std::endl;
    }
    else
    {
        std::cout << "Entering child process" << std::endl;
        executeTasks();
        std::cout << "Exiting child process" << std::endl;
    }
    return true;
}

void ProcessManagement::executeTasks()
{
    while (!taskQueue.empty())
    {
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();
        std::cout << "Executing task: " << taskToExecute->toString() << std::endl;
        executeCryption(taskToExecute->toString());
    }
}
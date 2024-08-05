/*

In process Management we are performing the creating task and push it to queue after which we will ppp it and create child processes using multithreading we perform encryption to various file parallelly.

*/

#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "Task.hpp"
#include <queue>
#include <memory>

class ProcessManagement
{
public:
    ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTasks();

private:
    std::queue<std::unique_ptr<Task>> taskQueue;
};

#endif
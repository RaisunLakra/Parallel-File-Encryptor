
/*
#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <memory>
#include <atomic>
#include <iostream>
#include <sys/mman.h> // For shared memory
#include <fcntl.h>    // For O_* constants
#include <unistd.h>   // For ftruncate
#include <semaphore.h>
#include <mutex> // Include this header for std::mutex

class ProcessManagement
{
    sem_t *itemsSemaphore;      // item present in the semaphore
    sem_t *emptySlotsSemaphore; // empty space in the semaphore
public:
    ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTasks();
    ~ProcessManagement();

private:
    struct SharedMemory
    {
        std::atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;

        void printSharedMemory()
        {
            std::cout << size.load() << std::endl;
        }
    };

    SharedMemory *sharedMem;
    int shmFd;
    const char *SHM_NAME = "/my_queue";
    std::mutex queueLock; // Ensure <mutex> is included to use std::mutex
};

#endif
*/

#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <memory>
#include <atomic>
#include <iostream>
#include <sys/mman.h> // For shared memory
#include <fcntl.h>    // For O_* constants
#include <unistd.h>   // For ftruncate
#include <semaphore.h>
#include <mutex> // Include this header for std::mutex

class ProcessManagement
{
    sem_t *itemsSemaphore;      // item present in the semaphore
    sem_t *emptySlotsSemaphore; // empty space in the semaphore
public:
    ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTasks();
    ~ProcessManagement();

private:
    struct SharedMemory
    {
        std::atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;

        void printSharedMemory()
        {
            std::cout << size.load() << std::endl;
        }
    };

    SharedMemory *sharedMem;
    int shmFd;
    const char *SHM_NAME = "/my_queue";
    std::mutex queueLock; // Ensure <mutex> is included to use std::mutex
};

#endif

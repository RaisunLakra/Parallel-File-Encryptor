#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/mman.h>
#include <atomic>
#include <fcntl.h>
#include <semaphore.h>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement()
{
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);               // item present in the semaphore
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000); // empty space in the semaphore

    if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmFd, sizeof(SharedMemory)) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *mmap_result = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (mmap_result == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sharedMem = static_cast<SharedMemory *>(mmap_result);
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    if (sharedMem->size.load() >= 1000)
    {
        return false;
    }

    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    sem_post(itemsSemaphore);

    int pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if (pid > 0)
    {
        std::cout << "Parent process" << std::endl;
    }
    else
    {
        std::cout << "Entering child process" << std::endl;
        executeTasks();
        std::cout << "Exiting child process" << std::endl;
        _exit(0);
    }
    return true;
}

void ProcessManagement::executeTasks()
{
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    if (sharedMem->size.load() == 0)
    {
        sem_post(itemsSemaphore);
        return;
    }

    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    sem_post(emptySlotsSemaphore);

    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement()
{
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);

    sem_close(itemsSemaphore);
    sem_unlink("/items_semaphore");

    sem_close(emptySlotsSemaphore);
    sem_unlink("/empty_slots_semaphore");
}

/*
#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/mman.h>
#include <atomic>
#include <fcntl.h>
#include <semaphore.h>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement()
{
    sem_t *itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);               // item present in the semaphore
    sem_t *emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000); // empty space in the semaphore
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmFd, sizeof(SharedMemory)) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *mmap_result = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (mmap_result == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sharedMem = static_cast<SharedMemory *>(mmap_result);
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    if (sharedMem->size.load() >= 1000)
    {
        return false;
    }

    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    sem_post(itemsSemaphore);

    int pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if (pid > 0)
    {
        std::cout << "Parent process" << std::endl;
    }
    else
    {
        std::cout << "Entering child process" << std::endl;
        executeTasks();
        std::cout << "Exiting child process" << std::endl;
        _exit(0);
    }
    return true;
}

void ProcessManagement::executeTasks()
{
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);
    if (sharedMem->size.load() == 0)
    {
        return;
    }

    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    sem_post(emptySlotsSemaphore);

    std::cout << "Executing tasks" << std::endl;
    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement()
{
    munmap(sharedMem, sizeof(SharedMemory));
    sem_unlink(SHM_NAME);
}

*/

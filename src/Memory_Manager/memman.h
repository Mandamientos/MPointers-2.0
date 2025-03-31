//This a template for the memory manager class. The memory
//manager class is responsible for managing the memory pool
//that is used to store the data that is sent to the service.

#ifndef MEMMAN_H
#define MEMMAN_H

#include <cstdint>
#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


class memoryManager {
public:

    uint32_t idCount_ = 0;

    memoryManager(size_t size_mb, std::string dumpFolder_);
    ~memoryManager();
    
    uint32_t createBlock(size_t size, const std::string& type);
    bool setValue(uint32_t id, const std::vector<uint8_t>& value);
    std::vector<uint8_t> getValue(uint32_t id);
    void increaseRefCount(uint32_t id);
    void decreaseRefCount(uint32_t id);
    void dumpMemory();
    void garbageCollector();
    void defrag();
    
private:
    struct memoryBlock
    {
        uint32_t id;
        size_t size;
        uint32_t ref_count;
        size_t offset;
        std::string type;
    };

    uint8_t* heap_;
    size_t heapSize_;
    std::vector<memoryBlock> blocks_;
    std::mutex mtx_;
    std::string dumpFolder_;

    // Garbage collector thread configuration
    std::thread gcThread_;
    std::atomic<bool> gcRunning_;
    void gcThreadLoop();
};

#endif
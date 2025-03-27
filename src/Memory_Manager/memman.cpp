#include "memman.h"
#include <stdexcept>
#include <cstring>
#include <fstream> //dump

memoryManager::memoryManager(size_t size_mb, std::string dumpFolder) {
    heapSize_ = size_mb * 1024 * 1024;
    heap_ = static_cast<uint8_t*>(malloc(heapSize_));
    if (!heap_) {
        throw std::runtime_error("Could not allocate memory");
    }
    dumpFolder_ = dumpFolder;
}

memoryManager::~memoryManager() {
    free(heap_);
}

uint32_t memoryManager::createBlock(size_t size, const std::string& type) {
    std::lock_guard<std::mutex> lock(mtx_);
    memoryBlock block;
    block.id = ++idCount_;
    ++idCount_;
    block.size = size;
    block.ref_count = 0;
    block.type = type;
    return block.id;
}


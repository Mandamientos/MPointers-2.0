#include "memman.h"
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>

memoryManager::memoryManager(size_t size_mb, std::string dumpFolder) {
    heapSize_ = size_mb * 1024 * 1024;
    heap_ = static_cast<uint8_t*>(malloc(heapSize_));
    gcRunning_ = true;
    if (!heap_) {
        throw std::runtime_error("Could not allocate memory");
    }
    dumpFolder_ = dumpFolder;
    std::cout << "[MM] Memory manager initialized with size: " << heapSize_ << " bytes." << std::endl;
    gcThread_ = std::thread(&memoryManager::gcThreadLoop, this);
}

memoryManager::~memoryManager() {
    free(heap_);
}

void memoryManager::dumpMemory() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream filename;
    filename << dumpFolder_ << "/dump_" << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d_%H-%M-%S") << "_" << now_ms.count() << ".txt";

    std::ofstream dumpFile(filename.str());
    if (!dumpFile.is_open()) {
        std::cerr << "[MM] Error opening dump file: " << filename.str() << std::endl;
        return;
    }

    dumpFile << "Memory Dump at " << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "\n";
    dumpFile << "----------------------------------------\n";
    dumpFile << "Heap Size: " << heapSize_ << " bytes\n";
    dumpFile << "----------------------------------------\n";
    dumpFile << "ID\tSize\tRef Count\tOffset\tType\n";
    for (const auto& block : blocks_) {
        dumpFile << block.id << "\t" << block.size << "\t" << block.ref_count << "\t" << block.offset << "\t" << block.type << "\n";
    }
    dumpFile << "----------------------------------------\n";
    dumpFile << "End of Memory Dump\n";
    dumpFile.close();
    std::cout << "[MM] Memory dump saved to: " << filename.str() << std::endl;
}

uint32_t memoryManager::createBlock(size_t size, const std::string& type) {
    std::lock_guard<std::mutex> lock(mtx_);

    size_t offset = 0;
    for (const auto& block : blocks_) {
        offset = block.offset + block.size;
        std::cout << "[MM] Calculating offset, current total offset: " << offset << std::endl;
    }

    if (offset + size > heapSize_) {
        throw std::runtime_error("Not enough memory");
    }

    memoryBlock newBlock{
        idCount_++,
        size,
        0,
        offset,
        type
    };

    blocks_.push_back(newBlock);
    dumpMemory();
    return newBlock.id;
}

void memoryManager::increaseRefCount(uint32_t id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& block : blocks_) {
        if (block.id == id) {
            block.ref_count++;
            dumpMemory();
            return;
        }
    }
}

void memoryManager::decreaseRefCount(uint32_t id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& block : blocks_) {
        if (block.id == id) {
            if (block.ref_count > 0) {
                block.ref_count--;
                dumpMemory();
            }
            return;
        }
    }
}

void memoryManager::gcThreadLoop() {
    std::cout << "[GC] Garbage collector initialized." << std::endl;
    while (gcRunning_) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        garbageCollector();
        std::cout << "[GC] Cleaning memory..." << std::endl;
        //dumpMemory();
    }
}

void memoryManager::garbageCollector() {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto it = blocks_.begin(); it != blocks_.end();) {
        if (it->ref_count == 0) {
            std::memset(heap_ + it->offset, 0, it->size);
            it = blocks_.erase(it);
        } else {
            ++it;
        }
    }
}


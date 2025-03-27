#ifndef MPOINTERS_H
#define MPOINTERS_H

#include <cstdint>
#include <vector>
#include <mutex>
#include <iostream>
#include <grpcpp/grpcpp.h>

#include "memman.grpc.pb.h"

template <typename T>
class MPointer {
private:
    static std::unique_ptr<MemoryManager::Stub> stub_;
    static std::shared_ptr<grpc::Channel> channel_;

    void setValue(const T& value);
    T getValue() const;
    void increaseRefCount();
    void decreaseRefCount();

public:

    uint32_t id_;

    MPointer() : id_(-1) {} //pointer without initialization

    ~MPointer() { //destructor for the pointer class 
        if (id_ != -1) {
            //decreaseRefCount();
        }
    }

    static void Init(const std::string serverAddress) { //initialize the pointer class with the server address
        channel_ = grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials());
        stub_ = MemoryManager::NewStub(channel_);
    }

    static MPointer<T> New() {
        MPointer<T> pointer;

        CreateRequest request;
        request.set_size(sizeof(T));
        request.set_type(typeid(T).name());
    
        CreateResponse response;
        grpc::ClientContext context;
    
        grpc::Status status = stub_->Create(&context, request, &response);
    
        if (!status.ok()) {
            std::cerr << "Error creating pointer: " << status.error_message() << std::endl;
        }
    
        pointer.id_ = response.id();
        return pointer;
    }

    T operator*() const {
        return getValue();
    }

    void operator=(const T& value) {
        setValue(value);
    }

    MPointer<T>& operator=(const MPointer<T>& other);

    int operator&() const {
        return id_;
    }
};

template <typename T>
std::shared_ptr<grpc::Channel> MPointer<T>::channel_ = nullptr;

template <typename T>
std::unique_ptr<MemoryManager::Stub> MPointer<T>::stub_ = nullptr;

#endif
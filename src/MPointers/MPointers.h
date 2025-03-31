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
    uint32_t id_;
    static std::unique_ptr<MemoryManager::Stub> stub_;
    static std::shared_ptr<grpc::Channel> channel_;

public:

    MPointer() : id_(-1) {} //pointer without initialization

    ~MPointer() { //destructor for the pointer class 
        if (id_ != -1) {
            decreaseRefCount();
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

    void setValue(const T& value) {
        SetRequest request;
        request.set_id(id_);
        request.set_value(reinterpret_cast<const char*>(&value), sizeof(T));

        SetResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->Set(&context, request, &response);

        if (!status.ok()) {
            std::cerr << "Error setting value: " << status.error_message() << std::endl;
        }
    };

    T getValue() const {
        GetRequest request;
        request.set_id(id_);

        GetResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->Get(&context, request, &response);

        if (!status.ok()) {
            std::cerr << "Error getting value: " << status.error_message() << std::endl;
            return T(); // Return default value
        }

        T value;
        std::memcpy(&value, response.value().data(), sizeof(T));
        return value;
    }

    void increaseRefCount() {
        RefCountRequest request;
        request.set_id(id_);

        RefCountResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);

        if (!status.ok()) {
            std::cerr << "Error increasing reference count: " << status.error_message() << std::endl;
        }
    }

    void decreaseRefCount() {
        RefCountRequest request;
        request.set_id(id_);

        RefCountResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);

        if (!status.ok()) {
            std::cerr << "Error decreasing reference count: " << status.error_message() << std::endl;
        }
    }

    class Proxy {
        private:
            MPointer<T>& pointer_;
        public:
            Proxy(MPointer<T>& pointer) : pointer_(pointer) {}

            void operator=(const T& value) {
                pointer_.setValue(value);
            }

            operator T() const {
                return pointer_.getValue();
            }
    };

    Proxy operator*() {
        std::cout << "Dereferencing pointer" << std::endl;
        return Proxy(*this);
    }

    MPointer<T>& operator=(const MPointer<T>& other) {
        if (std::addressof(*this) != std::addressof(other)) {
            if (id_ != -1) {
                decreaseRefCount();
            }
            id_ = other.id_;
            std::cout << "Copying pointer" << std::endl;
            increaseRefCount();
        }
        return *this;
    }

    int operator&() const {
        return id_;
    }
};

template <typename T>
std::shared_ptr<grpc::Channel> MPointer<T>::channel_ = nullptr;

template <typename T>
std::unique_ptr<MemoryManager::Stub> MPointer<T>::stub_ = nullptr;

#endif
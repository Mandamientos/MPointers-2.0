#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <grpcpp/grpcpp.h>
#include <vector>

#include "memman.grpc.pb.h"
#include "memman.h"

class memmanServiceImpl final : public MemoryManager::Service {
public:
    memmanServiceImpl(size_t memsize, std::string& dumpFolder);

    grpc::Status Create(grpc::ServerContext* context, const CreateRequest* request, CreateResponse* response) override;
    grpc::Status Set(grpc::ServerContext* context, const SetRequest* request, SetResponse* response) override;
    grpc::Status Get(grpc::ServerContext* context, const GetRequest* request, GetResponse* response) override;
    grpc::Status IncreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) override;
    grpc::Status DecreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) override;

private:
    memoryManager memman_;
};

class starter {
public:
    starter(std::string& address, size_t memsize, std::string dumpFolder); // std::string dumpFolder
    void startServer();
private:
    std::string address_;
    std::unique_ptr<grpc::Server> server_;
    memmanServiceImpl service_;
};

#endif

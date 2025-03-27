#include "server.h"

starter::starter(std::string& address, size_t memsize, std::string dumpFolder) : address_(address), service_(memsize, dumpFolder) {}

void starter::startServer() {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << address_ << std::endl;
    server_->Wait();
}

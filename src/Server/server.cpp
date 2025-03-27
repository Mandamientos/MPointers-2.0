#include "server.h"

memmanServiceImpl::memmanServiceImpl(size_t memsize, std::string& dumpFolder) : memman_(memsize, dumpFolder) {}

grpc::Status memmanServiceImpl::Create(grpc::ServerContext* context, const CreateRequest* request, CreateResponse* response) {
    uint32_t id = memman_.createBlock(request->size(), request->type());
    response->set_id(id);
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::Set(grpc::ServerContext* context, const SetRequest* request, SetResponse* response) {
    // TODO: Implement this method
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::Get(grpc::ServerContext* context, const GetRequest* request, GetResponse* response) {
    //TODO: Implement this method
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::DecreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) {
    //TODO: Implement this method
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::IncreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) {
    //TODO: Implement this method
    return grpc::Status::OK;
}
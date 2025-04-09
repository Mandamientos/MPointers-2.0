#include "server.h"

memmanServiceImpl::memmanServiceImpl(size_t memsize, std::string& dumpFolder) : memman_(memsize, dumpFolder) {}

grpc::Status memmanServiceImpl::Create(grpc::ServerContext* context, const CreateRequest* request, CreateResponse* response) {
    uint32_t id = memman_.createBlock(request->size(), request->type());
    response->set_id(id);
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::Set(grpc::ServerContext* context, const SetRequest* request, SetResponse* response) {
    std::vector<uint8_t> value(request->value().begin(), request->value().end());
    memman_.setValue(request->id(), value);
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::Get(grpc::ServerContext* context, const GetRequest* request, GetResponse* response) {
    std::vector<uint8_t> value = memman_.getValue(request->id());
    response->set_value(std::string(value.begin(), value.end()));
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::DecreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) {
    memman_.decreaseRefCount(request->id());
    return grpc::Status::OK;
}

grpc::Status memmanServiceImpl::IncreaseRefCount(grpc::ServerContext* context, const RefCountRequest* request, RefCountResponse* response) {
    memman_.increaseRefCount(request->id());
    return grpc::Status::OK;
}
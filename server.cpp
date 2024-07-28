#include <grpcpp/grpcpp.h>

#include <iostream>

#include "example.grpc.pb.h"

// 实现 ExampleService 服务
class ExampleServiceImpl final : public example::ExampleService::Service {
public:
    grpc::Status ExampleMethod(grpc::ServerContext* context,
                               const example::Request* request,
                               example::Response* response) override {
        // 处理请求
        std::string message = "Hello, " + request->message();
        response->set_message(message);
        return grpc::Status::OK;
    }
};

int main(int argc, char** argv) {
    // 设置监听地址
    std::string server_address("0.0.0.0:50051");

    // 创建 gRPC 服务
    ExampleServiceImpl service;

    // 创建 gRPC 服务器
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // 启动服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // 等待服务器关闭
    server->Wait();
    return 0;
}

#include <grpcpp/grpcpp.h>

#include <iostream>

#include "example.grpc.pb.h"

int main() {
    // 创建 gRPC 客户端
    auto channel = grpc::CreateChannel("localhost:50051",
                                       grpc::InsecureChannelCredentials());
    auto stub = example::ExampleService::NewStub(channel);

    // 创建请求和响应对象
    example::Request request;
    example::Response response;
    grpc::ClientContext context;

    // 设置请求数据
    request.set_message("Hello, gRPC!");

    // 发起 RPC 调用
    grpc::Status status = stub->ExampleMethod(&context, request, &response);

    if (status.ok()) {
        std::cout << "Response: " << response.message() << std::endl;
    } else {
        std::cerr << "RPC failed" << std::endl;
    }

    return 0;
}

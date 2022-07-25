#include "server.h"
#include "functions.pb.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

server::server() : port(8080) {}

server::server(int port) : port(port) {}

int server::start() {
  std::string server_address("0.0.0.0:" + std::to_string(port));
  grpc::ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  if (server == NULL) {
    std::cout << "listen " << port << " failed!!!!!" << std::endl;
    exit(-1);
  }

  std::cout << "running" << std::endl;
  server->Wait();
  return 0;
}

server::~server() {}

void server::stop() {}

grpc::Status server::QueryData(grpc::ServerContext *context,
                               const functions::GeneratorRequest *request,
                               functions::Drop *response) {
  response->set_timestamp(100);

  DeviceDrop device_drop = DeviceDrop{};
  DeviceDropItem device_drop_item = DeviceDropItem{
      .deviceId = "test-deviceid",
      .properties = std::unordered_map<std::string, Properties>()};

  std::vector<std::string> propertys = std::vector<std::string>();
  propertys.push_back("1");
  propertys.push_back("2");
  propertys.push_back("3");
  propertys.push_back("4");
  propertys.push_back("5");
  propertys.push_back("6");
  propertys.push_back("7");

  device_drop_item.properties.insert(
      std::map<std::basic_string<char>, Properties>::value_type(
          "p1",
          Properties{.dataType = functions::Int, .properties = propertys}));

  device_drop.items = std::vector<DeviceDropItem>();
  device_drop.items.push_back(device_drop_item);

  functions::DeviceDrop *drop = device_drop.ToFunctionDeviceDrop();
  response->set_allocated_devicedrop(drop);

  return grpc::Status::OK;
}

grpc::Status
server::SubscribeData(grpc::ServerContext *context,
                      const functions::GeneratorRequest *request,
                      grpc::ServerWriter<::functions::Drop> *writer) {

  DeviceDrop device_drop = DeviceDrop{};
  DeviceDropItem device_drop_item = DeviceDropItem{
      .deviceId = "test-deviceid",
      .properties = std::unordered_map<std::string, Properties>()};

  std::vector<std::string> propertys = std::vector<std::string>();
  propertys.push_back("1");
  propertys.push_back("2");
  propertys.push_back("3");
  propertys.push_back("4");
  propertys.push_back("5");
  propertys.push_back("6");
  propertys.push_back("7");

  device_drop_item.properties.insert(
      std::map<std::basic_string<char>, Properties>::value_type(
          "p1",
          Properties{.dataType = functions::Int, .properties = propertys}));

  device_drop.items = std::vector<DeviceDropItem>();
  device_drop.items.push_back(device_drop_item);

  functions::DeviceDrop *d_drop = device_drop.ToFunctionDeviceDrop();

  functions::Drop drop;
  drop.set_allocated_devicedrop(d_drop);
  int n = 1000;
  while (true) {
    drop.set_timestamp(n);
    writer->Write(drop);
    n++;
    sleep(1);
  }

  return grpc::Status::OK;
}

// generator function
grpc::Status server::Probe(grpc::ServerContext *context,
                           const functions::ProbeRequest *request,
                           functions::ProbeResponse *response) {
  std::cout << "probe" << std::endl;
  return grpc::Status::OK;
}
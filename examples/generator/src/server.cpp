#include "server.h"
#include "drop.h"
#include "functions.pb.h"
#include "util.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

Server::Server() : port(8080) {}

Server::Server(int port) : port(port) {}

int Server::start() {
  std::string server_address("0.0.0.0:" + std::to_string(port));
  grpc::ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  if (server == NULL) {
    std::cout << "listen " << port << " failed!!!!!" << std::endl;
    exit(-1);
  }

  std::cout << "running on " << this->port << std::endl;
  server->Wait();
  return 0;
}

Server::~Server() {}

void Server::stop() {}

static functions::DeviceDrop *generatro_device_drop() {
  DeviceDrop device_drop = DeviceDrop{};
  DeviceDropItem device_drop_item = DeviceDropItem{
      .deviceId = "test-deviceid",
      .properties = std::unordered_map<std::string, Properties>()};

  std::vector<std::string> propertys = std::vector<std::string>();

  for (int i = 0; i < 10; i++) {
    propertys.push_back(std::to_string(get_random(0, 100)));
  }

  device_drop_item.properties.insert(
      std::map<std::basic_string<char>, Properties>::value_type(
          "p1",
          Properties{.dataType = functions::Int, .properties = propertys}));

  device_drop.items = std::vector<DeviceDropItem>();
  device_drop.items.push_back(device_drop_item);

  functions::DeviceDrop *drop = device_drop.ToFunctionDeviceDrop();
  return drop;
}

grpc::Status Server::QueryData(grpc::ServerContext *context,
                               const functions::GeneratorRequest *request,
                               functions::Drop *response) {
  long ts = get_ms_timestamp();
  response->set_timestamp(ts);

  functions::DeviceDrop *drop = generatro_device_drop();
  response->set_allocated_devicedrop(drop);

  return grpc::Status::OK;
}

grpc::Status
Server::SubscribeData(grpc::ServerContext *context,
                      const functions::GeneratorRequest *request,
                      grpc::ServerWriter<functions::Drop> *writer) {
  functions::Drop drop;

  while (true) {
    long ts = get_ms_timestamp();
    drop.set_timestamp(ts);
    functions::DeviceDrop *device_drop = generatro_device_drop();

    drop.set_allocated_devicedrop(device_drop);

    writer->Write(drop);
    sleep(1);
  }

  return grpc::Status::OK;
}

// generator function
grpc::Status Server::Probe(grpc::ServerContext *context,
                           const functions::ProbeRequest *request,
                           functions::ProbeResponse *response) {
  std::cout << "probe" << std::endl;
  return grpc::Status::OK;
}
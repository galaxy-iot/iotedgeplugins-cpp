#include "server.h"
#include "dataset.h"
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

static functions::DeviceDataSet *generatro_device_drop() {
  DeviceDataSet device_ds = DeviceDataSet{};
  DeviceDataSetItem device_ds_item = DeviceDataSetItem{
      .deviceId = "test-deviceid",
      .properties = std::unordered_map<std::string, Properties>()};

  std::vector<std::string> propertys = std::vector<std::string>();

  for (int i = 0; i < 10; i++) {
    propertys.push_back(std::to_string(get_random(0, 100)));
  }

  device_ds_item.properties.insert(
      std::map<std::basic_string<char>, Properties>::value_type(
          "p1",
          Properties{.dataType = functions::Int, .properties = propertys}));

  device_ds.items = std::vector<DeviceDataSetItem>();
  device_ds.items.push_back(device_ds_item);

  functions::DeviceDataSet *drop = device_ds.ToFunctionDeviceDrop();
  return drop;
}

grpc::Status Server::QueryData(grpc::ServerContext *context,
                               const functions::GeneratorRequest *request,
                               functions::DataSet *response) {
  long ts = get_ms_timestamp();
  response->set_timestamp(ts);

  functions::DeviceDataSet *ds = generatro_device_drop();
  response->set_allocated_devicedataset(ds);
  return grpc::Status::OK;
}

grpc::Status
Server::SubscribeData(grpc::ServerContext *context,
                      const functions::GeneratorRequest *request,
                      grpc::ServerWriter<functions::DataSet> *writer) {
  functions::DataSet ds;

  while (true) {
    long ts = get_ms_timestamp();
    ds.set_timestamp(ts);
    functions::DeviceDataSet *device_data_set = generatro_device_drop();

    ds.set_allocated_devicedataset(device_data_set);

    writer->Write(ds);
    sleep(1);
  }

  return grpc::Status::OK;
}
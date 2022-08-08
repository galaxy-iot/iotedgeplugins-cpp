#include "server.h"
#include "drop.h"
#include "functions.pb.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <util.h>

DeviceDropProcessor::DeviceDropProcessor() : port(8080) {}

DeviceDropProcessor::DeviceDropProcessor(int port) : port(port) {}

int DeviceDropProcessor::Start() {
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

DeviceDropProcessor::~DeviceDropProcessor() {}

void DeviceDropProcessor::Stop() {}

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

grpc::Status DeviceDropProcessor::ProcessDataOnce(grpc::ServerContext *context,
                                                  const functions::Drop *drop,
                                                  functions::Drop *response) {
  // if this drop is not a device drop, just return it
  functions::Drop *mutable_drop = const_cast<functions::Drop *>(drop);
  if (!mutable_drop->has_devicedrop()) {
    response = mutable_drop;
    return grpc::Status::OK;
  }

  functions::DeviceDrop *device_drop =
      new functions::DeviceDrop(mutable_drop->devicedrop());

  auto items = device_drop->mutable_items();
  for (auto it = items->begin(); it != items->end(); it++) {
    auto property_map = it->mutable_properties();

    for (auto p_it = property_map->begin(); p_it != property_map->end();
         p_it++) {
      auto properties = p_it->second.mutable_properties();
      auto data_type = p_it->second.datatype();

      if (data_type == functions::Int) {
        for (auto pp_it = properties->begin(); pp_it != properties->end();
             pp_it++) {

          int v = atoi(pp_it->c_str());
          *pp_it = std::to_string(v * 10);
        }
      }
    }
  }

  response->set_allocated_devicedrop(device_drop);
  return grpc::Status::OK;
}

grpc::Status DeviceDropProcessor::ProcessDataStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<functions::Drop, functions::Drop> *stream) {

  functions::Drop drop;
  while (stream->Read(&drop)) {
    stream->Write(drop);
  }

  return grpc::Status::OK;
}
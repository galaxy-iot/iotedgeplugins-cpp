#include "server.h"
#include "dataset.h"
#include "functions.pb.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string.h>
#include <string>
#include <unordered_map>
#include <util.h>
#include <vector>

DeviceDataSetProcessor::DeviceDataSetProcessor() : port(8080) {}

DeviceDataSetProcessor::DeviceDataSetProcessor(int port) : port(port) {}

int DeviceDataSetProcessor::Start() {
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

DeviceDataSetProcessor::~DeviceDataSetProcessor() {}

void DeviceDataSetProcessor::Stop() {}

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

grpc::Status
DeviceDataSetProcessor::ProcessDataOnce(grpc::ServerContext *context,
                                        const functions::DataSet *ds,
                                        functions::DataSet *response) {
  // if this drop is not a device drop, just return it
  functions::DataSet *mutable_ds = const_cast<functions::DataSet *>(ds);
  if (!mutable_ds->has_devicedataset()) {
    response = mutable_ds;
    return grpc::Status::OK;
  }

  functions::DeviceDataSet *device_ds =
      new functions::DeviceDataSet(mutable_ds->devicedataset());

  auto items = device_ds->mutable_items();
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

  response->set_allocated_devicedataset(device_ds);
  return grpc::Status::OK;
}

grpc::Status DeviceDataSetProcessor::ProcessDataStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<functions::DataSet, functions::DataSet> *stream) {

  functions::DataSet ds;
  while (stream->Read(&ds)) {
    stream->Write(ds);
  }

  return grpc::Status::OK;
}
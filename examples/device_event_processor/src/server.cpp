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

DeviceEventProcessor::DeviceEventProcessor() : port(8080) {}

DeviceEventProcessor::DeviceEventProcessor(int port) : port(port) {}

int DeviceEventProcessor::Start() {
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

DeviceEventProcessor::~DeviceEventProcessor() {}

void DeviceEventProcessor::Stop() {}

grpc::Status
DeviceEventProcessor::ProcessDataOnce(grpc::ServerContext *context,
                                      const functions::DataSet *ds,
                                      functions::DataSet *response) {
  if (!ds->has_eventdataset()) {
    response->set_ok(false);
    response->set_allocated_errormessage(
        new std::string("this dataset is not a event dataset"));
    return grpc::Status::OK;
  }

  auto event_ds = ds->eventdataset();
  functions::EventDataSet *new_ds = new functions::EventDataSet;
  new_ds->CopyFrom(event_ds);

  response->set_allocated_eventdataset(new_ds);
  return grpc::Status::OK;
}

grpc::Status DeviceEventProcessor::ProcessDataStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<functions::DataSet, functions::DataSet> *stream) {

  functions::DataSet ds;
  while (stream->Read(&ds)) {
    stream->Write(ds);
  }

  return grpc::Status::OK;
}
#ifndef SERVER_H
#define SERVER_H

#include "dataset.h"
#include "functions.grpc.pb.h"
#include "functions.pb.h"
#include <grpcpp/support/status.h>
#include <iterator>
#include <unordered_map>
#include <vector>

class DeviceEventProcessor : public functions::Processor::Service {
private:
  int port;

public:
  // create a grpc server with port 8080
  DeviceEventProcessor();
  // create a grpc server with given port
  DeviceEventProcessor(int port);
  // deconstruct function
  ~DeviceEventProcessor();
  // make this server running
  int Start();
  // stop this server
  void Stop();

  grpc::Status ProcessDataOnce(grpc::ServerContext *context,
                               const functions::DataSet *drop,
                               functions::DataSet *response);

  grpc::Status ProcessDataStream(
      grpc::ServerContext *context,
      grpc::ServerReaderWriter<functions::DataSet, functions::DataSet> *stream);
};

#endif
#ifndef SERVER_H
#define SERVER_H

#include "drop.h"
#include "functions.grpc.pb.h"
#include "functions.pb.h"
#include <grpcpp/support/status.h>
#include <iterator>
#include <unordered_map>
#include <vector>

class DeviceDropProcessor : public functions::Processor::Service {
private:
  int port;

public:
  // create a grpc server with port 8080
  DeviceDropProcessor();
  // create a grpc server with given port
  DeviceDropProcessor(int port);
  // deconstruct function
  ~DeviceDropProcessor();
  // make this server running
  int Start();
  // stop this server
  void Stop();

  grpc::Status ProcessDataOnce(grpc::ServerContext *context,
                               const functions::Drop *drop,
                               functions::Drop *response);

  grpc::Status ProcessDataStream(
      grpc::ServerContext *context,
      grpc::ServerReaderWriter<functions::Drop, functions::Drop> *stream);
};

#endif
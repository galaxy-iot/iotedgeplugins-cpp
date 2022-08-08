#ifndef SERVER_H
#define SERVER_H

#include "functions.grpc.pb.h"
#include "functions.pb.h"
#include <iterator>
#include <unordered_map>
#include <vector>

class rpc : public functions::Generator::Service {};

class Server : public functions::Generator::Service {
private:
  int port;

public:
  // create a grpc server with port 8080
  Server();
  // create a grpc server with given port
  Server(int port);
  // deconstruct function
  ~Server();
  // make this server running
  int start();
  // stop this server
  void stop();

  grpc::Status QueryData(grpc::ServerContext *context,
                         const functions::GeneratorRequest *request,
                         functions::Drop *response);

  grpc::Status SubscribeData(grpc::ServerContext *context,
                             const functions::GeneratorRequest *request,
                             grpc::ServerWriter<::functions::Drop> *writer);
  // generator function
  grpc::Status Probe(grpc::ServerContext *context,
                     const functions::ProbeRequest *request,
                     functions::ProbeResponse *response);
};

#endif
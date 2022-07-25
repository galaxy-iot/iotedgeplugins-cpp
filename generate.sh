#! /bin/bash

mkdir -p ./protobuf/function
# generate grpc protobuf
protoc -I=./protos/function --cpp_out=./protobuf/function --grpc_out=./protobuf/function --plugin=protoc-gen-grpc=/bin/grpc_cpp_plugin  functions.proto
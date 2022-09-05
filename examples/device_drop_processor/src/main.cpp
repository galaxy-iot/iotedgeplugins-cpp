#include "server.h"
#include <iostream>
#include <string>

int main() {
  std::string s1;
  DeviceDataSetProcessor s(8082);
  s.Start();
}
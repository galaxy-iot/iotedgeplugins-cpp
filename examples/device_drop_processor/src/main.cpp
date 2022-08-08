#include "server.h"
#include <iostream>

int main() {
  DeviceDropProcessor s(8082);
  s.Start();
}
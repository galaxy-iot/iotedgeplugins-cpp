#include "server.h"
#include <iostream>

int main() {
  DeviceEventProcessor s(8082);
  s.Start();
}
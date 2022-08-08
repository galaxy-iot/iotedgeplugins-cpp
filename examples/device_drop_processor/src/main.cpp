#include "server.h"
#include <iostream>

int main() {
  DeviceDropProcessor s(8081);
  s.Start();
}
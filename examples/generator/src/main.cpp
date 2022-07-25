#include "server.h"
#include <iostream>

int main() {
  server s(8081);
  s.start();
}
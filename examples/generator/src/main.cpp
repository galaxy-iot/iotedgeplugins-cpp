#include "server.h"
#include <iostream>

int main() {
  Server s(8081);
  s.start();
}
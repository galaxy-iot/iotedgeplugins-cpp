#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <iostream>
#include <sys/time.h>

long get_ms_timestamp() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long ms = static_cast<long>(tv.tv_sec) * 1000 +
            static_cast<long>(tv.tv_usec) / 1000;
  return ms;
}

int get_random(int min, int max) { return (rand() % (max - min + 1)) + min; }

#endif
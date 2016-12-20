#include <Arduino.h>


extern "C" {
  #include "extra.h"
  int close(int fd) {
    return 0;
  }

  int open(const char *pathname, int flags, ...) {
    Serial.print("open(");
    Serial.print(pathname),
    Serial.println("");
    return 0;
  }

  int fstat(int fd, struct stat *buf) {
    return 0;
  }

  ssize_t read(int fd, void *buf, size_t count) {
    return 0;
  }
}

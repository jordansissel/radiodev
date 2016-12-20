#include <Arduino.h>


extern "C" {
  #include "extra.h"
  int close(int fd) {
    return 0;
  }

  int open(const char *pathname, int flags, ...) {
    Serial.print("open(");
    Serial.print(pathname),
    Serial.println(")");
    return 0;
  }

  int fstat(int fd, struct stat *buf) {
    Serial.print("fstat(");
    Serial.print(fd);
    Serial.println(")");
    return 0;
  }

  ssize_t read(int fd, void *buf, size_t count) {
    Serial.print("read(");
    Serial.print(fd);
    Serial.print(", ");
    Serial.print(count);
    Serial.println(")");
    return 0;
  }
}

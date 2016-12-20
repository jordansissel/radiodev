#ifndef _EXTRA_H_
#define _EXTRA_H_
typedef int ssize_t;

#define O_RDONLY 0

// Maybe this hack is sufficient.
#define S_ISCHR(x) 0

struct stat { 
  int st_mode;
};

struct timeval { 
  int tv_sec;
  int tv_usec;
};

extern int close(int fd);
extern int open(const char *pathname, int flags, ...);
#endif /* _EXTRA_H_ */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <sys/types.h>
#define ssize_t long
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifndef _S_IREAD
#define _S_IREAD 0400
#endif
#ifndef _S_IWRITE
#define _S_IWRITE 0200
#endif

#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define open _open
#define close _close
#define read _read
#define write _write
#define lseek _lseek
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
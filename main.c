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

//wrote my own custom getline
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }

    char *buffer = *lineptr;
    size_t size = *n;
    int c;
    size_t i = 0;

    if (buffer == NULL || size == 0) {
        size = 128;  // Initial buffer size
        buffer = malloc(size);
        if (buffer == NULL) {
            return -1;
        }
    }

    while ((c = fgetc(stream)) != EOF) {
        if (i >= size - 1) {
            size *= 2;  // Double the buffer size
            char *new_buffer = realloc(buffer, size);
            if (new_buffer == NULL) {
                free(buffer);
                return -1;
            }
            buffer = new_buffer;
        }
        buffer[i++] = (char)c;
        if (c == '\n') {
            break;
        }
    }

    if (i == 0 && c == EOF) {
        return -1;  // End of file and no data
    }

    buffer[i] = '\0';
    *lineptr = buffer;
    *n = size;

    return i;
}


typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_DUPLICATE_KEY,
} ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_NEGATIVE_ID,
  PREPARE_STRING_TOO_LONG,
  PREPARE_SYNTAX_ERROR,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert;  // only used by insert statement
} Statement;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
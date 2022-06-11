#ifndef __MT_LOG_H__
#define __MT_LOG_H__

#include <mutiny/mutiny.h>

typedef struct _mt_log {
  FILE* file;
  const char* start_txt;
  char buf[512];
} mt_log_t;

mt_log_t mt_log_init(FILE* file, const char* start_txt);

void mt_log_add(mt_log_t* log, const char* fmt, ...);

void mt_log_dump(mt_log_t* log);

#define MT_VERBOSE "\033[35m\033[1mVerbose: \033[0m"
#define MT_WARNING "\033[33m\033[1mWarning: \033[0m"
#define MT_ERROR   "\033[31m\033[1mError: \033[0m"

#endif // __MT_LOG_H__

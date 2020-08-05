#ifndef VARCEL_LOG_H_
#define VARCEL_LOG_H
#include <stdio.h>

#ifdef VARCEL_LOG_SHOW_DEBUG_
#define logI(...) {fputs("[INFO] ", stdout); printf(__VA_ARGS__); fputc('\n', stdout);}
#define logW(...) {fputs("[WARNING] ", stdout); printf(__VA_ARGS__); fputc('\n', stdout);}
#else
#define logI(...)
#define logW(...)
#endif//LOG_DEBUG_
#define logE(...) {fputs("[ERR] ", stderr); fprintf(stderr, __VA_ARGS__); fputc('\n', stderr);}

#endif
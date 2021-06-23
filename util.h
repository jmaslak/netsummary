/*
 * util.h
 *
 * General utility functions
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#ifndef _UTIL_H_
#define _UTIL_H_ 1

#include <time.h>

extern void * util_newstring(char * str);
extern void * util_malloc(size_t size);
extern void   util_free(void * ptr);
extern void   util_error(int err);
extern void   util_error_long(int err, char * msg);

extern int    is_integer(char * str);

extern time_t util_get_time(void);

extern char * util_printable_ip(unsigned int ip);

#endif

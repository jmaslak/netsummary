/*
 * util.c
 *
 * General utility functions
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */
#define _UTIL_C_ 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "netsummary.h"
#include "util.h"

void * util_newstring(char * str) {
	size_t size;
	char * ptr;

	assert(str != NULL);
	
	size = strlen(str);
	ptr = util_malloc(size);
	strcpy(ptr, str);

	return ptr;
}

void * util_malloc(size_t size) {
	void * ret;

	assert(size > 0);

	ret = malloc(size);
	if (ret == NULL) {
		util_error(ERR_ALLOCATION_FAILURE);
	}

	return ret;
}

void util_free(void * ptr) {
	assert(ptr != NULL);
	
	free(ptr);
}

void util_error(int err) {
	assert(err > ERR_SUCCESS);
	assert(err < ERR_UNKNOWN);
	
	fprintf(stderr, "Error %d - %s\n", err, ERROR[err]);
	exit(err);
}

void util_error_long(int err, char * msg) {
	assert(err > ERR_SUCCESS);
	assert(err < ERR_UNKNOWN);
	assert(msg != NULL);
	
	fprintf(stderr, "Error %d - %s\n", err, ERROR[err]);
	fprintf(stderr, "  Details: %s\n", msg);
	exit(err);
}

int is_integer(char * str) {
	int ctr = 0;

	while (*str != '\0') {
		if (ctr == 1) {
			if (((*str < '0') || (*str > '9'))
			    && (*str != '-')) {
				return 0;
			}
		} else {
			if ((*str < '0') || (*str > '9')) {
				return 0;
			}
		}
		ctr++;
		str++;
	}

	return 1;
}

time_t util_get_time(void) {
	time_t current;

	current = time(NULL);
	
	if (current == ((time_t) -1)) {
		util_error_long(ERR_SYSTEM_CALL_FAIL, "time() call failed");
	}

	return current;
}

char * util_printable_ip(unsigned int ip) {
	static char printable[16];
	unsigned char * c;
	c = (void *) &ip;

	assert(c[0] < 256);
	assert(c[1] < 256);
	assert(c[2] < 256);
	assert(c[3] < 256);
	
	sprintf(printable,
		"%u.%u.%u.%u",
		c[0], c[1], c[2], c[3]);

	assert(strlen(printable) >= 7);
	assert(strlen(printable) <= 15);

	return printable;
}

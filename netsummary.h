/*
 * netsummary.h
 *
 * Global include file
 *
 * Copyright (C) 2003,2021 Joelle Maslak
 * All Rights Reserved
 *
 */

#ifndef _NETSUMMARY_H_
#define _NETSUMMARY_H_

#include <time.h>

#include "error.h"

#define DEBUG (0)

#define FREE  0xc0c0
#define MAGIC 0xf32e

#define DEFAULT_GRE       0
#define DEFAULT_INTERFACE "eth0"
#define DEFAULT_TIME      60
#define DEFAULT_TOP       10

// Must be a power of 2 minus 1
#define MAX_ADDRESSES  (2097152 - 1)
#define HEAP_ADDRESSES (MAX_ADDRESSES * 2 + 1)

struct struct_cmd_options {
	int gre;
	char * interface;
	time_t time;
	int top;
	int magic;
};

extern struct struct_cmd_options cmd_options;

struct struct_heap_element {
	unsigned long long bytes;
	unsigned long packets;
	unsigned long ip;
};

struct struct_port_element {
	unsigned int port; 
	unsigned long packets;
	unsigned long long bytes;
};

#endif

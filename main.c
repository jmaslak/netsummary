/*
 * Net Summary
 *
 * Tracks and summarizes active connecitons over a given time period
 *
 * Copyright (C) 2003,2021 Joelle Maslak
 * All Rights Reserved
 *
 */

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
#include "netsummary.h"
#include "ports.h"
#include "sniff.h"
#include "summarize.h"
#include "util.h"

#define OPTIONS "gi:s:t:"

struct struct_cmd_options cmd_options;

void init_options(void);
int  parse_options(int argc, char ** argv);
void usage(void);

int main(int argc, char ** argv) {
	assert(argc >= 0);
	
	if (!parse_options(argc, argv)) {
		usage();
	}

	heap_init();
	ports_init();

	sniff();

	summarize();
	
	return 0;
}

int parse_options(int argc, char ** argv) {
	int c;
	
	assert(argc >= 0);	

	init_options();

	while (1) {
		c = getopt(argc, argv, OPTIONS);
		if (c == -1) { break; }

		switch (c) {
			case 'g':
				cmd_options.gre = 1;
				break;
			case 'i':
				cmd_options.interface = util_newstring(optarg);
				break;
			case 's':
				if (is_integer(optarg)) {
					cmd_options.time = atoi(optarg);
					if (cmd_options.time < 0) {
						return 0;
					}
				} else {
					return 0;
				}
				break;
			case 't':
				if (is_integer(optarg)) {
					cmd_options.top = atoi(optarg);
					if (cmd_options.top < 0) {
						return 0;
					}
				} else {
					return 0;
				}
				break;
			default :
				return 0;
		}
	}

	if (optind < argc) {
		return 0;
	}

	return 1;
}

void init_options(void) {
	cmd_options.gre       = DEFAULT_GRE;
	cmd_options.interface = DEFAULT_INTERFACE;
	cmd_options.time      = DEFAULT_TIME;
	cmd_options.top       = DEFAULT_TOP;
	cmd_options.magic     = MAGIC;
}

void usage(void) {
	fprintf(stderr, "Usage: \n");
	fprintf(stderr, "   netsummary [-i interface] [-s seconds] ");
	fprintf(stderr,               "[-t count] [-g]\n");
	fprintf(stderr, "     interface  Network interface (default: eth0)\n");
	fprintf(stderr, "     seconds    Amount of time to gather stats for ");
	fprintf(stderr,                 "(default: 60 seconds)\n");
	fprintf(stderr, "     count      Number of events to report ");
	fprintf(stderr,                 "(default: 10)\n");
	fprintf(stderr, "     -g         Indicates GRE-encapsulated traffic\n");

	fprintf(stderr, "\n");
	
	util_error(ERR_INVALID_PARAMETERS);
}
		
		

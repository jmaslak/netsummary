/*
 * ports.c
 *
 * Track port usage
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#include <assert.h>
#include <stdio.h>

#include "netsummary.h"

struct struct_port_element ports[65536];

void ports_init(void) {
	int i;

	for (i=0; i<65536; i++) {
		ports[i].port    = i;
		ports[i].packets = 0;
		ports[i].bytes   = 0;
	}
}

void ports_update(unsigned int portnumber, unsigned int bytes) {
	assert(ports[portnumber].port == portnumber);

#if DEBUG >= 10
	fprintf(stderr, "Updating port %d counts\n", portnumber);
#endif
	
	ports[portnumber].packets++;
	ports[portnumber].bytes += bytes;
}

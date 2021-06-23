/*
 * summarize.c
 *
 * Generate & display summaries
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "netsummary.h"
#include "heap.h"
#include "ports.h"
#include "util.h"

void summarize_ip(void);
int  summarize_build_ip_list(void);
int  summarize_compare_ip_packets(const void * a, const void * b);
void summarize_port(void);
void summarize_build_port_list(void);
int  summarize_compare_port_packets(const void * a, const void * b);

void summarize(void) {
	summarize_ip();
	summarize_port();
}

void summarize_ip(void) {
	int eles;
	int i;
	int top;
	int max;
	
	eles = summarize_build_ip_list();
	
	printf("IP Summary by Packet Count:\n");
	printf("   Packets    IP Address    (    Bytes   )\n");
	printf("  --------- --------------- --------------\n");

	if (cmd_options.top > eles) {
		top = eles;
	} else {
		top = cmd_options.top;
	}

	max = eles - 1;

	for (i=0; i<top; i++) {
		printf("  %9ld %15s (%12lld)\n",
		       list[max-i].packets,
		       util_printable_ip(list[max-i].ip),
		       list[max-i].bytes);
	}

	printf("\n");
}

int summarize_build_ip_list(void) {
	int eles;

	assert(heap != NULL);
	assert(list != NULL);

	eles = heap_sort(heap, list);

	qsort(list, eles, sizeof(struct struct_heap_element),
	      &summarize_compare_ip_packets);

	return eles;
}

int summarize_compare_ip_packets(const void * a, const void * b) {
	struct struct_heap_element * as;
	struct struct_heap_element * bs;

	as = (struct struct_heap_element *) a;
	bs = (struct struct_heap_element *) b;

	return (as->packets - bs->packets);	
}

void summarize_port(void) {
	int i;
	
	summarize_build_port_list();
	
	printf("Port Summary (TCP & UDP) by Packet Count:\n");
	printf("   Packets  Port# (    Bytes   )\n");
	printf("  --------- ----- --------------\n");

	for (i=0; i<cmd_options.top; i++) {
		if (ports[65535-i].packets > 0) {
			printf("  %9ld %5d (%12lld)\n",
			       ports[65535-i].packets,
			       ports[65535-i].port,
			       ports[65535-i].bytes);
		}
	}

	printf("\n");
}

void summarize_build_port_list(void) {
	qsort(ports, 65536, sizeof(struct struct_port_element),
	      &summarize_compare_port_packets);
}

int summarize_compare_port_packets(const void * a, const void * b) {
	struct struct_port_element * as;
	struct struct_port_element * bs;

	as = (struct struct_port_element *) a;
	bs = (struct struct_port_element *) b;

	return (as->packets - bs->packets);	
}


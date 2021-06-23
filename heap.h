/*
 * heap.h
 *
 * Heap handling
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#ifndef _HEAP_H_
#define _HEAP_H_ 1

#ifdef UBUNTU
#include <dumbnet.h>
#include <dumbnet/ip.h>
#else
#include <dnet.h>
#include <dnet/ip.h>
#endif

#include "netsummary.h"

extern void heap_init(void);
extern struct struct_heap_element * heap_find(unsigned int ip);
extern struct struct_heap_element * heap_insert(
	struct struct_heap_element * ele);
extern void heap_update(ip_addr_t ip, int size);
extern int heap_sort(struct struct_heap_element * h,
	             struct struct_heap_element * l);

extern struct struct_heap_element * heap;
extern struct struct_heap_element * list;

#endif


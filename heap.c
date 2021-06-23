/*
 * heap.c
 *
 * Heap handling
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#include <assert.h>

#ifdef UBUNTU
#include <dumbnet.h>
#include <dumbnet/ip.h>
#else
#include <dnet.h>
#include <dnet/ip.h>
#endif

#include <stdio.h>

#include "netsummary.h"
#include "util.h"

#define HEAPSIZE (sizeof(struct struct_heap_element) * HEAP_ADDRESSES)
#define LISTSIZE (sizeof(struct struct_heap_element) * MAX_ADDRESSES)

struct struct_heap_element * heap = NULL;
struct struct_heap_element * list = NULL;

static unsigned int elements = 0;

void heap_empty_heap(void);
void heap_empty_list(void);
void heap_rebalance(void);
int heap_sort(struct struct_heap_element * h,
	      struct struct_heap_element * l);
void heap_insert_recursive(struct struct_heap_element * l, int eles);

void heap_init(void) {
	assert(heap == NULL);
	assert(list == NULL);
	assert(elements == 0);

	heap = util_malloc(HEAPSIZE);
	list = util_malloc(LISTSIZE);

	heap_empty_heap();
	heap_empty_list();
}

void heap_empty_heap(void) {
	int i;

	for (i=0; i<HEAP_ADDRESSES; i++) {
		(heap+i)->ip      = 0;
		(heap+i)->packets = FREE;
		(heap+i)->bytes   = FREE;
	}
}

void heap_empty_list(void) {
	int i;
	
	for (i=0; i<MAX_ADDRESSES; i++) {
		(heap+i)->ip      = 0;
		(heap+i)->packets = FREE;
		(heap+i)->bytes   = FREE;
	}
}

struct struct_heap_element * heap_find(unsigned int ip) {
	int start = 0;
	int end   = HEAP_ADDRESSES - 1;
	int pos   = (HEAP_ADDRESSES / 2);

	struct struct_heap_element * current;
	
	assert(heap != NULL);

	while (1) {
		assert(start <= pos);
		assert(pos   <= end);
		
		current = (heap + pos);

		if ((current->ip) == 0) {
#if DEBUG
			unsigned char * c;
			c = (void *) &ip;
			fprintf(stderr,
				"Inserting IP address %u.%u.%u.%u\n",
				c[0], c[1], c[2], c[3]);
#endif
	
			current->ip      = ip;
			current->packets = 0;
			current->bytes   = 0;
			elements++;
			return current;
		} else if ((current->ip) == ip) {
			return current;
		} else if ((current->ip) < ip) {
			start = pos + 1;
		} else if ((current->ip) > ip) {
			end = pos - 1;
		}
		
		if (start > end) {
			assert (elements <= MAX_ADDRESSES);
			if (elements == MAX_ADDRESSES) {
				util_error(ERR_TOO_MANY_ADDRESSES);
			}
			heap_rebalance();
			start = 0;
			end   = HEAP_ADDRESSES - 1;
		}
		
		pos = (end + start) / 2;
	}
}

struct struct_heap_element * heap_insert(struct struct_heap_element * ele) {
	struct struct_heap_element * record;
			 
	assert(heap != NULL);

	record = heap_find(ele->ip);
	
	assert(record != NULL);
	
	record->packets = ele->packets;
	record->bytes   = ele->bytes;

	return record;
}

void heap_update(ip_addr_t ip, int size) {
	struct struct_heap_element * record;

	assert(heap != NULL);

	record = heap_find(ip);

	assert (record != NULL);

	record->packets++;
	record->bytes += size;
}

void heap_rebalance(void) {
	int eles;

#if DEBUG
	fprintf(stderr, "Rebalancing heap\n");
#endif
		
	assert(heap != NULL);
	assert(list != NULL);

	eles = heap_sort(heap, list);
	assert(eles == elements);
	elements = 0;
	
	heap_empty_heap();

	heap_insert_recursive(list, eles);
	assert(eles == elements);
}

int heap_sort(struct struct_heap_element * h,
	      struct struct_heap_element * l)
{
	int eles = 0;
	int i;
	
	assert(h != NULL);
	assert(l != NULL);

	for (i=0; i < HEAP_ADDRESSES; i++) {
		if (((h+i)->ip) != 0) {
			l->ip      = (h+i)->ip;
			l->packets = (h+i)->packets;
			l->bytes   = (h+i)->bytes;
			l++;
			eles++;
		}
	}

	return eles;
}

void heap_insert_recursive(struct struct_heap_element * l, int eles) {
	int mid;
	int max;
	
	assert (l != NULL);
	assert (eles > 0);
	assert (eles <= MAX_ADDRESSES);

	max = eles - 1;

	mid = eles / 2;

	assert((l+mid)->ip != 0);
	heap_insert(l+mid);

	if (mid > 0) {
		heap_insert_recursive(l, mid);
	}
	if (mid < (max)) {
		heap_insert_recursive(l+mid+1, max-mid);
	}
}

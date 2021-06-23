/*
 * ports.h
 *
 * Track port usage
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#ifndef _PORTS_H_
#define _PORTS_H_ 1

#include "netsummary.h"

extern struct struct_port_element ports[];

extern void ports_update(unsigned int portnumber, unsigned int bytes);
extern void ports_init(void);

#endif


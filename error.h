/*
 * error.h
 *
 * Error messages
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#ifndef _ERROR_H_
#define _ERROR_H_ 1

#ifdef _UTIL_C_
const char * ERROR[] = {
	"Operation completed successfully",
	"Could not allocate memory",
	"Invalid command line parameters",
	"pcap_open_live failed",
	"pcap_compile failed",
	"pcap_setfilter failed",
	"System call failed",
	"Too many addresses added to heap -- recompile",
	"UNKNOWN ERROR - PROGRAM ERROR"
};
#else
extern char * ERROR[];
#endif

enum {
	ERR_SUCCESS,
	ERR_ALLOCATION_FAILURE,
	ERR_INVALID_PARAMETERS,
	ERR_PCAP_OPEN_FAIL,
	ERR_PCAP_COMPILE_FAIL,
	ERR_PCAP_FILTER_FAIL,
	ERR_SYSTEM_CALL_FAIL,
	ERR_TOO_MANY_ADDRESSES,
	ERR_UNKNOWN
} ERROR_VALUE;

#endif

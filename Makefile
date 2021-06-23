# Makefile for netsummary

# If compiling on CentOS, remove DUBUNTU
CFLAGS=-Wall -O2

netsummary	: main.o util.o sniff.o heap.o summarize.o ports.o
	gcc $(CFLAGS) -o netsummary main.o util.o sniff.o heap.o \
			summarize.o ports.o -static -lpcap

main.o		: main.c error.h netsummary.h util.h sniff.h heap.h \
		  summarize.h ports.h

util.o		: util.c util.h error.h netsummary.h

heap.o		: heap.c util.h error.h netsummary.h

sniff.o		: sniff.c sniff.h error.h util.h netsummary.h ports.h

summarize.o	: summarize.c summarize.h heap.h netsummary.h util.h

ports.o         : ports.c ports.h

clean		:
	rm *.o netsummary





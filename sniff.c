/*
 * sniff.c
 *
 * The actual sniffer
 *
 * Copyright (C) 2003 Joelle Maslak
 * All Rights Reserved
 *
 */

#include <assert.h>

#ifdef UBUNTU
#include <dumbnet.h>
#include <dumbnet/eth.h>
#include <dumbnet/ip.h>
#include <dumbnet/tcp.h>
#include <dumbnet/udp.h>
#else
#include <dnet.h>
#include <dnet/eth.h>
#include <dnet/ip.h>
#include <dnet/tcp.h>
#include <dnet/udp.h>
#endif

#include <pcap.h>

#include "netsummary.h"
#include "heap.h"
#include "ports.h"
#include "util.h"

// How big of packets do we support?
#define SNIFF_BUFFER 65535

char errbuf[PCAP_ERRBUF_SIZE];
pcap_t *handle;
struct bpf_program filter;

void open_pcap(void);
void filter_iponly(void);
void loop(void);
void sniff_eth(const u_char * packet, int size, int origsize);
void sniff_ip(const u_char * packet, int size, int origsize);
void sniff_tcp(const u_char * packet, int size, int origsize);
void sniff_udp(const u_char * packet, int size, int origsize);

void sniff(void) {
	open_pcap();
	filter_iponly();
	loop();
}

void open_pcap(void) {
	assert(cmd_options.magic == MAGIC);
	assert(cmd_options.interface != NULL);
	
	handle = pcap_open_live(cmd_options.interface,
				SNIFF_BUFFER,
				1,
				0,
				errbuf);

	if (handle == NULL) {
		util_error_long(ERR_PCAP_OPEN_FAIL, errbuf);
	}
}

void filter_iponly(void) {
	if (pcap_compile(handle, &filter, "ip", 1, 0) < 0) {
		util_error(ERR_PCAP_COMPILE_FAIL);
	}
	if (pcap_setfilter(handle, &filter) < 0) {
		util_error(ERR_PCAP_FILTER_FAIL);
	}
}

void loop(void) {
	time_t start, current;
	struct pcap_pkthdr(header);
	const u_char * packet;
	unsigned long long count = 0;

	assert(cmd_options.magic == MAGIC);
	assert(cmd_options.time >= 0);

	start = current = util_get_time();
	while ((start + cmd_options.time) > util_get_time()) {

		// The main loop

		packet = pcap_next(handle, &header);
		if (count++ == 0) { continue; } // Discard first packet
		
		assert(header.len == header.caplen);

		if (header.len >= sizeof(struct eth_hdr)) {
			sniff_eth(packet, header.len, header.len);
		}
	}
}

void sniff_eth (const u_char * packet, int size, int origsize) {
	const struct eth_hdr * hdr;
	
	assert(size >= sizeof(struct eth_hdr));

	hdr = (const struct eth_hdr *) packet;

	if (ntohs(hdr->eth_type) != ETH_TYPE_IP) {
		return;  // Don't know what to do with this - wrong kind of
		         // packet
	}
	
	size -= sizeof(struct eth_hdr);
	packet += sizeof(struct eth_hdr);

	if (size < sizeof(struct ip_hdr)) {
		return; // Too small
	}

	sniff_ip(packet, size, origsize);
}

void sniff_ip (const u_char * packet, int size, int origsize) {
	const struct ip_hdr * hdr;
	       
	assert(size >= sizeof(struct ip_hdr));

	hdr = (const struct ip_hdr *) packet;

	if (hdr->ip_src != 0) {
		heap_update(hdr->ip_src, origsize);
	}
	if (hdr->ip_dst != 0) {
		heap_update(hdr->ip_dst, origsize);
	}

	if ((hdr->ip_p) == IP_PROTO_TCP) {
		size -= 4 * (hdr->ip_hl);
		packet += 4 * (hdr->ip_hl);

		if (size < sizeof(struct tcp_hdr)) {
			return; // Too small
		}
		sniff_tcp(packet, size, origsize);
		
	} else if ((hdr->ip_p) == IP_PROTO_UDP) {
		size -= 4 * (hdr->ip_hl);
		packet += 4 * (hdr->ip_hl);

		if (size < sizeof(struct udp_hdr)) {
			return; // Too small
		}
		sniff_udp(packet, size, origsize);
		
	}
}

void sniff_udp (const u_char * packet, int size, int origsize) {
	const struct udp_hdr * hdr;
	       
	assert(size >= sizeof(struct udp_hdr));

	hdr = (const struct udp_hdr *) packet;

	ports_update(ntohs(hdr->uh_sport), origsize);
	ports_update(ntohs(hdr->uh_dport), origsize);
}

void sniff_tcp (const u_char * packet, int size, int origsize) {
	const struct tcp_hdr * hdr;
	       
	assert(size >= sizeof(struct tcp_hdr));

	hdr = (const struct tcp_hdr *) packet;

	ports_update(ntohs(hdr->th_sport), origsize);
	ports_update(ntohs(hdr->th_dport), origsize);
}


WHY netsummary?
  
  This is intended as a tool to be used to find sources of high traffic
  on a network, to find out where bandwidth utilization may be occuring
  or determine nodes with network problems (such as worm infections).

  Netsummary is similar to ntop, but much more basic and command-line
  focused.  Programs such as ntop do not scale appropriately at high
  traffic volumes.

  Netsummary provides a simple list of the "top" talking IP addresses
  and port numbers.  The IP list includes IP addresses in either source
  or destination.  The port list includes ports in either source or
  destination, and both TCP and UDP packets.  Thus, typically the output
  might lead you to figure out where your top talkers are, but you'll
  need to do further digging to find out the nature of the traffic.

  This tool is not intended to solve all network problems.  Rather, it
  is designed to efficiently monitor relatively high traffic networks
  and point a network administrator in the right direction.  It's very
  primitive in its output, deliberatelively (to minimize memory
  consumption and processing overhead).  It can handle several hundred
  megabytes of traffic easily Dell PowerEdge 2450 hardware, with only
  3-4% processor utilization.

Configuration:

  For very large networks (> 2,000,000 valid source or destination IP
  addresses), you will need to adjust MAX_ADDRESSES in netsummary.h .

Usage: 
   netsummary [-i interface] [-s seconds] [-t count]
     interface  Network interface (default: eth0)
     seconds    Amount of time to gather stats for (default: 60 seconds)
     count      Number of events to report (default: 10)

  Typically, you would use an interface that is a span or monitor port
  on a switch.

To compile on Linux using GCC ---> See the appropriate section below.

For CentOS/RedHat/Etc:
  Edit Makefile and remove the -DUBUNTU definition
  Make sure you install the package "libdnet-devel"
  Make sure you install the package "libpcap-devel"
  Use "make"
(note: you will see some compile warnings about range comparisons - these
are expected on RHEL which uses 8 bit character types; These checks are
applicable to other OSes which use longer character types, but will not
do any harm on RHEL based distros as they are only used in assert()
statements)

For Ubuntu/Some Debian:
  Make sure you install the package "libdumbnet-dev"
  Make sure you install the package "libpcap-dev"
  Use "make"
  


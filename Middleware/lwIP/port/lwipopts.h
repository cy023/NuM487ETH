#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

// #define LWIP_HTTPD 1

#define LWIP_IPV4 1

#define LWIP_IPV6 0

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT 0

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS 1

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 */
#define NO_SYS_NO_TIMERS 0

/* ---------- Memory options ---------- */
/** ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure
 * alignment of payload after that header. Since the header is 14 bytes long,
 * without this padding e.g. addresses in the IP header will not be aligned
 * on a 32-bit boundary, so setting this to 2 can speed up 32-bit-platforms.
 */
// #define ETH_PAD_SIZE 2

/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
    lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
    byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT 4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE (30 * 1024)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
    sends a lot of data out of ROM (or other static memory), this
    should be set high. */
#define MEMP_NUM_PBUF 50
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
    per active UDP "connection". */
#define MEMP_NUM_UDP_PCB 6
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
    connections. */
#define MEMP_NUM_TCP_PCB 10
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
    connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 6
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
    segments. */
#define MEMP_NUM_TCP_SEG 12
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
    timeouts. */
#define MEMP_NUM_SYS_TIMEOUT 10

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE 10

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE 500

/* ---------- TCP options ---------- */
#define LWIP_TCP 1
#define TCP_TTL  255

/* Controls if TCP should queue segments that arrive out of
    order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ 0

/* TCP Maximum segment size. */
#define TCP_MSS (1500 - 40)

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF (4 * TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */

#define TCP_SND_QUEUELEN (2 * TCP_SND_BUF / TCP_MSS)

/* TCP receive window. */
#define TCP_WND (2 * TCP_MSS)

/* ---------- ICMP options ---------- */
#define LWIP_ICMP 1

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
turning this on does currently not work. */
#define LWIP_DHCP 1

/* ---------- UDP options ---------- */
#define LWIP_UDP 1
#define UDP_TTL  255

/* ---------- Statistics options ---------- */
#define LWIP_STATS         0
#define LWIP_PROVIDE_ERRNO 1

/* ---------- link callback options ---------- */
/* LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK 0
/*
    --------------------------------------
    ---------- Checksum options ----------
    --------------------------------------
*/
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
#define CHECKSUM_GEN_IP 1
/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP
 * packets.*/
#define CHECKSUM_GEN_UDP 1
/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP
 * packets.*/
#define CHECKSUM_GEN_TCP 1
/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
#define CHECKSUM_CHECK_IP 1
/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP
 * packets.*/
#define CHECKSUM_CHECK_UDP 1
/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP
 * packets.*/
#define CHECKSUM_CHECK_TCP 1
/*CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP
 * packets.*/
#define CHECKSUM_GEN_ICMP 1

/*
    ----------------------------------------------
    ---------- Sequential layer options ----------
    ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN 0

/*
    ------------------------------------
    ---------- Socket options ----------
    ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET 0

/*
    ----------------------------------------
    ---------- Lwip Debug options ----------
    ----------------------------------------
*/
#define LWIP_DEBUG       1
#define ICMP_DEBUG       LWIP_DBG_ON
#define ICMP_ECHO_DEBUG  LWIP_DBG_ON

// #define TCP_DEBUG        LWIP_DBG_ON
// #define TCP_INPUT_DEBUG  LWIP_DBG_ON
// #define TCP_FR_DEBUG     LWIP_DBG_ON
// #define TCP_RTO_DEBUG    LWIP_DBG_ON
// #define TCP_CWND_DEBUG   LWIP_DBG_ON
// #define TCP_WND_DEBUG    LWIP_DBG_ON
// #define TCP_OUTPUT_DEBUG LWIP_DBG_ON
// #define TCP_RST_DEBUG    LWIP_DBG_ON
// #define TCP_QLEN_DEBUG   LWIP_DBG_ON
// #define UDP_DEBUG        LWIP_DBG_ON
// #define TCPIP_DEBUG      LWIP_DBG_ON

#endif /* __LWIPOPTS_H__ */

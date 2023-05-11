/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "NuMicro.h"
#include "ethernetif.h"
#include <string.h>
#include <stdio.h>

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};

/* EMAC Tx/Rx descriptor's owner bit */
#define EMAC_DESC_OWN_EMAC 0x80000000UL /*!<  Set owner to EMAC */
#define EMAC_DESC_OWN_CPU  0x00000000UL /*!<  Set owner to CPU */

/* Rx Frame Descriptor Status */
#define EMAC_RXFD_RTSAS  0x00800000UL /*!<  Time Stamp Available */
#define EMAC_RXFD_RP     0x00400000UL /*!<  Runt Packet */
#define EMAC_RXFD_ALIE   0x00200000UL /*!<  Alignment Error */
#define EMAC_RXFD_RXGD   0x00100000UL /*!<  Receiving Good packet received */
#define EMAC_RXFD_PTLE   0x00080000UL /*!<  Packet Too Long Error */
#define EMAC_RXFD_CRCE   0x00020000UL /*!<  CRC Error */
#define EMAC_RXFD_RXINTR 0x00010000UL /*!<  Interrupt on receive */

/* Tx Frame Descriptor's Control bits */
#define EMAC_TXFD_TTSEN  0x08UL /*!<  Tx time stamp enable */
#define EMAC_TXFD_INTEN  0x04UL /*!<  Tx interrupt enable */
#define EMAC_TXFD_CRCAPP 0x02UL /*!<  Append CRC */
#define EMAC_TXFD_PADEN  0x01UL /*!<  Padding mode enable */

/* Tx Frame Descriptor Status */
#define EMAC_TXFD_TXINTR 0x00010000UL /*!<  Interrupt on Transmit */
#define EMAC_TXFD_DEF    0x00020000UL /*!<  Transmit deferred  */
#define EMAC_TXFD_TXCP   0x00080000UL /*!<  Transmission Completion  */
#define EMAC_TXFD_EXDEF  0x00100000UL /*!<  Exceed Deferral */
#define EMAC_TXFD_NCS    0x00200000UL /*!<  No Carrier Sense Error */
#define EMAC_TXFD_TXABT  0x00400000UL /*!<  Transmission Abort  */
#define EMAC_TXFD_LC     0x00800000UL /*!<  Late Collision  */
#define EMAC_TXFD_TXHA   0x01000000UL /*!<  Transmission halted */
#define EMAC_TXFD_PAU    0x02000000UL /*!<  Paused */
#define EMAC_TXFD_SQE    0x04000000UL /*!<  SQE error  */
#define EMAC_TXFD_TTSAS  0x08000000UL /*!<  Time Stamp available */

/** Tx/Rx buffer descriptor structure */
typedef struct {
    uint32_t u32Status1; /*!<  Status word 1 */
    uint32_t u32Data;    /*!<  Pointer to data buffer */
    uint32_t u32Status2; /*!<  Status word 2 */
    uint32_t u32Next;    /*!<  Pointer to next descriptor */
    uint32_t u32Backup1; /*!<  For backup descriptor fields over written by time
                            stamp */
    uint32_t u32Backup2; /*!<  For backup descriptor fields over written by time
                            stamp */
} EMAC_DESCRIPTOR_T;

unsigned char mac_addr[6] = {0x66, 0x66, 0x66, 0x88, 0x88, 0x88};
extern uint32_t u32CurrentTxDesc, u32NextTxDesc, u32CurrentRxDesc;

static void phy_layer_init(void)
{
    EMAC_PhyInit();
}

static void mac_layer_init(void)
{
    EMAC_Open(mac_addr);

    NVIC_EnableIRQ(EMAC_TX_IRQn);
    NVIC_EnableIRQ(EMAC_RX_IRQn);
    EMAC_ENABLE_TX();
    EMAC_ENABLE_RX();
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = mac_addr[0];
    netif->hwaddr[1] = mac_addr[1];
    netif->hwaddr[2] = mac_addr[2];
    netif->hwaddr[3] = mac_addr[3];
    netif->hwaddr[4] = mac_addr[4];
    netif->hwaddr[5] = mac_addr[5];

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
                   NETIF_FLAG_ETHERNET |
                   NETIF_FLAG_LINK_UP /*| NETIF_FLAG_IGMP*/;

#if LWIP_IPV6 && LWIP_IPV6_MLD
    /*
     * For hardware/netifs that implement MAC filtering.
     * All-nodes link-local is handled by default, so we must let the hardware
     * know to allow multicast packets in.
     * Should set mld_mac_filter previously. */
    if (netif->mld_mac_filter != NULL) {
        ip6_addr_t ip6_allnodes_ll;
        ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
        netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
    }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */

    /* Do whatever else is needed to initialize interface. */
    mac_layer_init();
    phy_layer_init();
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and
 * type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    EMAC_DESCRIPTOR_T *desc;
    u32_t status;

    /* Get Tx frame descriptor & data pointer */
    desc = (EMAC_DESCRIPTOR_T *) u32NextTxDesc;

    status = desc->u32Status1;

    /* Check descriptor ownership */
    if ((status & EMAC_DESC_OWN_EMAC) == EMAC_DESC_OWN_EMAC)
        return ERR_USE;

#if ETH_PAD_SIZE
    pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

    memcpy((u8_t *) desc->u32Data, p->payload, p->len);

    /* Set Tx descriptor transmit byte count */
    desc->u32Status2 = p->len;

#if ETH_PAD_SIZE
    pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    /* Change descriptor ownership to EMAC */
    desc->u32Status1 |= EMAC_DESC_OWN_EMAC;

    /* Get next Tx descriptor */
    u32NextTxDesc = (u32_t)(desc->u32Next);

    /* Trigger EMAC to send the packet */
    EMAC_TRIGGER_TX();

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p;
    volatile EMAC_DESCRIPTOR_T *cur_rx_desc =
        (volatile EMAC_DESCRIPTOR_T *) u32CurrentRxDesc;
    u32_t status;
    u16_t len;

    status = EMAC->INTSTS & 0xFFFF;
    EMAC->INTSTS = status;

    if (status & EMAC_INTSTS_RXBEIF_Msk) {
        // Shouldn't goes here, unless descriptor corrupted
        printf("[Error]: EMAC_INTSTS_RXBEIF\n");
        while (1)
            ;
    }

    status = cur_rx_desc->u32Status1;

    if (status & EMAC_DESC_OWN_EMAC)
        return NULL;

    if (status & EMAC_RXFD_RXGD) {
        len = status & 0xFFFF;

#if ETH_PAD_SIZE
        len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

        /* Allocate pbuf from pool (avoid using heap in interrupts) */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

        if (p != NULL) {

#if ETH_PAD_SIZE
            pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

            /* Copy ethernet frame into pbuf */
            memcpy((u8_t *) p->payload, (u8_t *) cur_rx_desc->u32Data, p->len);

#if ETH_PAD_SIZE
            pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        } else {
            printf("pbuf_alloc() failed.\n");
        }
    }
    cur_rx_desc->u32Status1 = EMAC_DESC_OWN_EMAC;
    cur_rx_desc = (volatile EMAC_DESCRIPTOR_T *) cur_rx_desc->u32Next;

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(struct netif *netif)
{
    err_t err;
    struct pbuf *p;

    /* move received packet into a new pbuf */
    p = low_level_input(netif);

    while (p) {
        /* entry point to the LwIP stack */
        err = netif->input(p, netif);

        if (err != ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
        }
        p = low_level_input(netif);
    }

    EMAC_RecvPktDone();
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "M487";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd,
                    LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
#if LWIP_IPV4
    netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *) &(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

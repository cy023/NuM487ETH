#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "system.h"
#include "NuMicro.h"

#include "ethernetif.h"
#include "ethernet_phy.h"
#include "netif/ethernet.h"

#include "lwip/tcpip.h"
#include "lwip/apps/lwiperf.h"
#include "lwip/etharp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"

#include "udpecho_raw.h"

volatile bool recv_flag = false;
struct netif gnetif;

void lwip_layer_init(void);

void printIPaddr(void)
{
    static char tmp_buff[16];
    printf("IP_ADDR    : %s\r\n",
           ipaddr_ntoa_r((const ip_addr_t *) &(gnetif.ip_addr), tmp_buff, 16));
    printf("NET_MASK   : %s\r\n",
           ipaddr_ntoa_r((const ip_addr_t *) &(gnetif.netmask), tmp_buff, 16));
    printf("GATEWAY_IP : %s\r\n\r\n",
           ipaddr_ntoa_r((const ip_addr_t *) &(gnetif.gw), tmp_buff, 16));
}

void timer0_init(void)
{
    // Set timer frequency to 100HZ
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 100);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);
}

int main(void)
{
    system_init();
    timer0_init();
    printf("[test]: TCP/IP Ping Test over lwIP Stack\n\n");
    lwip_layer_init();

    bool link_up = false;
    ethernet_phy_get_link_status(&link_up);
    /* Print IP address info */
    if (link_up && gnetif.ip_addr.addr) {
        printf("Hello Connection!\n");
        printIPaddr();
    }

    NVIC_EnableIRQ(EMAC_TX_IRQn);
    NVIC_EnableIRQ(EMAC_RX_IRQn);
    EMAC_ENABLE_TX();  // TODO: Enable opportunity?
    EMAC_ENABLE_RX();  // TODO: Enable opportunity?

    udpecho_raw_init();

    while (1) {
        /* LWIP timers - ARP, DHCP, TCP, etc. */
        sys_check_timeouts();
        if (recv_flag) {
            recv_flag = false;
            printf(":)\n");
        }
    }
}

void lwip_layer_init(void)
{
    ip_addr_t ipaddr, netmask, gw;

    IP4_ADDR(&gw, 192, 168, 0, 1);
    IP4_ADDR(&ipaddr, 192, 168, 0, 23);
    IP4_ADDR(&netmask, 255, 255, 255, 0);

    /* Initilialize the LwIP stack without RTOS */
    lwip_init();

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init,
              &netif_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);

    if (netif_is_link_up(&gnetif)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&gnetif);
        printf("netif_set_up\n\n");
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&gnetif);
        printf("netif_set_down\n\n");
    }
}

void EMAC_RX_IRQHandler(void)
{
    PH5 ^= 1;
    recv_flag = true;
    ethernetif_input(&gnetif);
}

void EMAC_TX_IRQHandler(void)
{
    PH4 ^= 1;
    // Clean up Tx resource occupied by previous sent.
    EMAC_SendPktDone();
}

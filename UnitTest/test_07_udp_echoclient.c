/**
 * @file test_06_udp_echoclient.c
 * @author cy023
 * @date 2023.05.08
 * @brief lwIP - UDP echo client
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "system.h"
#include "NuMicro.h"

#include "ethernetif.h"
#include "ethernet_phy.h"
#include "netif/ethernet.h"

#include "lwip/tcpip.h"
#include "lwip/etharp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"

#include "udpclient_raw.h"

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

void check_connection(void)
{
    bool link_up = false;
    ethernet_phy_get_link_status(&link_up);
    /* Print IP address info */
    if (link_up && gnetif.ip_addr.addr) {
        printf("Hello Connection!\n");
        printIPaddr();
    }
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
    printf("[test]: UDP echo client.\n\n");

    lwip_layer_init();
    check_connection();

    udp_echoclient_connect();

    while (1) {
        udp_echoclient_send();
        system_delay_ms(1000);

        // if (recv_flag) {
        //     recv_flag = false;
        //     printf("Data received.\n");
        // }
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

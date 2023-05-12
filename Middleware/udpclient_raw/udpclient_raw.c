#include <stdio.h>
#include <string.h>
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"

#include "udpclient_raw.h"

#define DEST_IP_ADDR0   192
#define DEST_IP_ADDR1   168
#define DEST_IP_ADDR2   0
#define DEST_IP_ADDR3   220

#define UDP_SERVER_PORT 7

static uint8_t data[100];
static volatile uint32_t message_count = 0;
static struct udp_pcb *upcb;

/**
 * @brief This function is called when an UDP datagrm has been received on the
 * port UDP_PORT.
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 * @retval None
 */
void udp_receive_callback(void *arg,
                          struct udp_pcb *upcb,
                          struct pbuf *p,
                          const ip_addr_t *addr,
                          u16_t port)
{
    /* increment message count */
    message_count++;

    printf("Received echo: %.*s\n", p->len, (char *) p->payload);

    /* Free receive pbuf */
    pbuf_free(p);
}


/**
 * @brief  Connect to UDP echo server
 * @param  None
 * @retval None
 */
void udp_echoclient_connect(void)
{
    ip_addr_t dest_ip_addr;
    err_t err;

    /* Create a new UDP control block  */
    upcb = udp_new();

    if (upcb != NULL) {
        /* assign destination IP address */
        IP4_ADDR(&dest_ip_addr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2,
                 DEST_IP_ADDR3);

        /* configure destination IP address and port */
        err = udp_connect(upcb, &dest_ip_addr, UDP_SERVER_PORT);

        if (err == ERR_OK) {
            /* Set a receive callback for the upcb */
            udp_recv(upcb, udp_receive_callback, NULL);
        }
    }
}

/**
 * @brief This function is called when an UDP datagrm has been received on the
 * port UDP_PORT.
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 * @retval None
 */
void udp_echoclient_send(void)
{
    struct pbuf *p;

    sprintf((char *) data, "sending udp client message %d",
            (int) message_count);

    /* allocate pbuf from pool */
    p = pbuf_alloc(PBUF_TRANSPORT, strlen((char *) data), PBUF_POOL);

    if (p != NULL) {
        /* copy data to pbuf */
        pbuf_take(p, (char *) data, strlen((char *) data));

        /* send udp data */
        udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
    }
}

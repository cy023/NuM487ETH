#include <stdio.h>
#include <string.h>
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
#include "system.h"

#include "tcpclient_raw.h"

#define DEST_IP_ADDR0 192
#define DEST_IP_ADDR1 168
#define DEST_IP_ADDR2 0
#define DEST_IP_ADDR3 220

#define TCP_SERVER_PORT 7

static uint8_t data[100];
static volatile uint32_t message_count = 0;
static struct tcp_pcb *tcpclient_pcb;

/* ECHO protocol states */
enum echoclient_states {
    ES_NOT_CONNECTED = 0,
    ES_CONNECTED,
    ES_RECEIVED,
    ES_CLOSING,
};

/* structure to be passed as argument to the tcp callbacks */
struct echoclient {
    enum echoclient_states state; /* connection status */
    struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
    struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};

static err_t tcp_echoclient_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb,
                                            struct echoclient *es);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient *es);
static err_t tcp_echoclient_connected(void *arg,
                                      struct tcp_pcb *tpcb,
                                      err_t err);

/**
 * @brief  Connects to the TCP echo server
 * @param  None
 * @retval None
 */
void tcp_echoclient_connect(void)
{
    err_t err;
    ip_addr_t dest_ip_addr;

    printf("[INFO]: tcp_echoclient_connect()\n");

    /* create new tcp pcb */
    tcpclient_pcb = tcp_new();
    if (tcpclient_pcb == NULL) {
        printf("[ERROR]: Failed to create TCP PCB\n");
        return;
    }

    IP4_ADDR(&dest_ip_addr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2,
             DEST_IP_ADDR3);

    /* connect to destination address/port */
    err = tcp_connect(tcpclient_pcb, &dest_ip_addr, TCP_SERVER_PORT,
                      tcp_echoclient_connected);
    if (err != ERR_OK) {
        printf("[ERROR]: Failed to connect to server. Error code: %d\n", err);
        tcp_abort(tcpclient_pcb);
        tcpclient_pcb = NULL;
    }
}

/**
 * @brief Function called when TCP connection established
 * @param tpcb: pointer on the connection contol block
 * @param err: when connection correctly established err should be ERR_OK
 * @retval err_t: returned error
 */
static err_t tcp_echoclient_connected(void *arg,
                                      struct tcp_pcb *tpcb,
                                      err_t err)
{
    struct echoclient *es = NULL;

    printf("[INFO]: tcp_echoclient_connected()\n");

    if (err != ERR_OK) {
        tcp_echoclient_connection_close(tpcb, es);
        return err;
    }

    /* allocate structure es to maintain tcp connection informations */
    es = (struct echoclient *) mem_malloc(sizeof(struct echoclient));
    if (es == NULL) {
        tcp_echoclient_connection_close(tpcb, es);
        return ERR_MEM;
    }
    es->state = ES_CONNECTED;
    es->pcb = tpcb;

    /* pass newly allocated es structure as argument to tpcb */
    tcp_arg(tpcb, es);

    /* initialize LwIP tcp_recv callback function */
    tcp_recv(tpcb, tcp_echoclient_recv);

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(tpcb, tcp_echoclient_sent);

    /* initialize LwIP tcp_poll callback function */
    tcp_poll(tpcb, tcp_echoclient_poll, 1);

    return ERR_OK;
}

/**
 * @brief tcp_receiv callback
 * @param arg: argument to be passed to receive callback
 * @param tpcb: tcp connection control block
 * @param err: receive error code
 * @retval err_t: retuned error
 */
static err_t tcp_echoclient_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err)
{
    struct echoclient *es = (struct echoclient *) arg;

    printf("[INFO]: tcp_echoclient_recv()\n");

    LWIP_ASSERT("arg != NULL", arg != NULL);

    /* if we receive an empty tcp frame from server => close connection */
    if (p == NULL) {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if (es->p_tx == NULL) {
            /* we're done sending, close connection */
            tcp_echoclient_connection_close(tpcb, es);
        } else {
            /* send remaining data */
            tcp_echoclient_send(tpcb, es);
        }
        return ERR_OK;
    }

    /* a non empty frame was received from echo server but err != ERR_OK */
    if (err != ERR_OK) {
        /* free received pbuf */
        if (p != NULL)
            pbuf_free(p);
        return err;
    }

    if (es->state == ES_CONNECTED) {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);
        message_count++;
        printf("Received echo: %.*s\n", p->len, (char *) p->payload);
        pbuf_free(p);
    } else { /* data received when connection already closed */
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);
        pbuf_free(p);
    }
    return ERR_OK;
}

/**
 * @brief function used to send data
 * @param  tpcb: tcp control block
 * @param  es: pointer on structure of type echoclient containing info on data
 *             to be sent
 * @retval None
 */
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    printf("[INFO]: tcp_echoclient_send()\n");

    while ((wr_err == ERR_OK) && (es->p_tx != NULL) &&
           (es->p_tx->len <= tcp_sndbuf(tpcb))) {
        /* get pointer on pbuf from es structure */
        ptr = es->p_tx;

        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK) {
            /* continue with next pbuf in chain (if any) */
            es->p_tx = ptr->next;

            if (es->p_tx != NULL) {
                /* increment reference count for es->p */
                pbuf_ref(es->p_tx);
            }

            /* free pbuf: will free pbufs up to es->p (because es->p has a
             * reference count > 0) */
            pbuf_free(ptr);
        } else if (wr_err == ERR_MEM) {
            /* we are low on memory, try later, defer to poll */
            es->p_tx = ptr;
        } else {
            /* TODO: other problem */
        }
    }
}

/**
 * @brief  This function implements the tcp_poll callback function
 * @param  arg: pointer on argument passed to callback
 * @param  tpcb: tcp connection control block
 * @retval err_t: error code
 */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
    struct echoclient *es = (struct echoclient *) arg;

    printf("[INFO]: tcp_echoclient_poll()\n");

    if (es == NULL) {
        tcp_abort(tpcb);
        return ERR_ABRT;
    }

    if (message_count == 10) {
        tcp_echoclient_connection_close(tpcb, es);
        return ERR_OK;
    }

    if (es->state == ES_CONNECTED) {
        sprintf((char *) data, "sending tcp client message %d",
                (int) message_count);

        /* allocate pbuf */
        es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char *) data), PBUF_POOL);
        if (es->p_tx == NULL) {
            tcp_echoclient_connection_close(tpcb, es);
            return ERR_MEM;
        }

        /* copy data to pbuf */
        pbuf_take(es->p_tx, (char *) data, strlen((char *) data));

        /* send data */
        tcp_echoclient_send(tpcb, es);
    }
    return ERR_OK;
}

/**
 * @brief  This function implements the tcp_sent LwIP callback (called when ACK
 *         is received from remote host for sent data)
 * @param  arg: pointer on argument passed to callback
 * @param  tcp_pcb: tcp connection control block
 * @param  len: length of data sent
 * @retval err_t: returned error code
 */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    LWIP_UNUSED_ARG(len);

    struct echoclient *es = (struct echoclient *) arg;

    printf("[INFO]: tcp_echoclient_sent()\n");

    if (es->p_tx != NULL) {
        tcp_echoclient_send(tpcb, es);
    }
    return ERR_OK;
}

/**
 * @brief This function is used to close the tcp connection with server
 * @param tpcb: tcp connection control block
 * @param es: pointer on echoclient structure
 * @retval None
 */
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb,
                                            struct echoclient *es)
{
    printf("[INFO]: tcp_echoclient_connection_close()\n");

    /* remove callbacks */
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es != NULL) {
        mem_free(es);
    }

    /* close tcp connection */
    tcp_close(tpcb);
}

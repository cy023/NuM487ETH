/**
 * @file test_04_ethframe.c
 * @author cy023
 * @date 2023.04.20
 * @brief Ethernet Frame R/W.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "NuMicro.h"
#include "system.h"
#include "ethernet_phy.h"

typedef struct eth_frame {
    uint8_t DA[6];
    uint8_t SA[6];
    uint16_t type;
    uint8_t payload[50];
} eth_frame_t;

eth_frame_t outframe, inframe;
uint32_t packetLen;

// MAC address
uint8_t mac_addr[6] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};

char tx_msg[] = "HELLO Ethernet! I'am NuM487.";

volatile bool recv_flag = false;

void EMAC_TX_IRQHandler(void)
{
    PH4 ^= 1;
    // Clean up Tx resource occupied by previous sent packet(s)
    EMAC_SendPktDone();
}

void EMAC_RX_IRQHandler(void)
{
    NVIC_DisableIRQ(EMAC_RX_IRQn);
    PH5 ^= 1;
    recv_flag = true;
    while (1) {
        // Check if there's any packets available
        if (EMAC_RecvPkt((uint8_t *) &inframe, &packetLen) == 0)
            break;
        // Clean up Rx resource occupied by previous received packet
        EMAC_RecvPktDone();
    }
    NVIC_EnableIRQ(EMAC_RX_IRQn);
}

void mac_layer_init(void)
{
    EMAC_Open(mac_addr);

    // TODO: set filter

    NVIC_EnableIRQ(EMAC_TX_IRQn);
    NVIC_EnableIRQ(EMAC_RX_IRQn);

    EMAC_ENABLE_TX();
    EMAC_ENABLE_RX();
}

void phy_layer_init(void)
{
    EMAC_PhyInit();
    // ethernet_phy_reset();
    // ethernet_phy_init();

    // bool link_up = false;
    // while (!link_up) {
    //     ethernet_phy_get_link_status(&link_up);
    // }
    // printf("Ethernet link up\n");
}

void printEtherFrame(eth_frame_t *ethframe, const char *str)
{
    if (ethframe->type != 0x3713)
        return;

    printf("\n\t\x1b[0;32;32mINFO\x1b[0;m : %s\n", str);
    printf("\t===== \x1b[;31;1mETHERNET FRAME HEADER\x1b[0;m =====\n");
    printf("\t| DA      : %2x:%2x:%2x:%2x:%2x:%2x   |\n", 
                    ethframe->DA[0], ethframe->DA[1], ethframe->DA[2],
                    ethframe->DA[3], ethframe->DA[4], ethframe->DA[5]);
    printf("\t| SA      : %2x:%2x:%2x:%2x:%2x:%2x   |\n", 
                    ethframe->SA[0], ethframe->SA[1], ethframe->SA[2],
                    ethframe->SA[3], ethframe->SA[4], ethframe->SA[5]);
    printf("\t| Type    : 0x%2x%2x              |\n",
                    (ethframe->type & 0xFF), ((ethframe->type >> 8) & 0xFF));
    printf("\t=================================\n");
    printf("\t============ \x1b[;31;1mPAYLOAD\x1b[0;m ============\n");
    printf("\t| Payload : %s\n", ethframe->payload);
    printf("\t=================================\n\n");
}


int main()
{
    system_init();
    printf("[test]: R/W Ethernet Frame with EtherTypes 0x1337.\n\n");

    outframe.DA[0] = 0xFF;
    outframe.DA[1] = 0xFF;
    outframe.DA[2] = 0xFF;
    outframe.DA[3] = 0xFF;
    outframe.DA[4] = 0xFF;
    outframe.DA[5] = 0xFF;
    outframe.SA[0] = 0x66;
    outframe.SA[1] = 0x66;
    outframe.SA[2] = 0x66;
    outframe.SA[3] = 0x66;
    outframe.SA[4] = 0x66;
    outframe.SA[5] = 0x66;
    outframe.type = 0x3713;
    strncpy((char *) outframe.payload, tx_msg, strlen(tx_msg));

    mac_layer_init();
    phy_layer_init();

    uint32_t res = EMAC_SendPkt((uint8_t *) &outframe, sizeof(outframe));
    printf("res = %ld\n", res);
    printEtherFrame(&outframe, "I'am outframe.");

    while (1) {
        if (recv_flag) {
            printEtherFrame(&inframe, "I'am inframe.");
            recv_flag = false;
        }
    }
    return 0;
}

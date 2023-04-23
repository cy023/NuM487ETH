/**
 * @file test_03_phy_loopback.c
 * @author cy023
 * @date 2023.04.20
 * @brief Ethernet MAC Layer Loopback test.
 *
 * Reference: DP83848C/I/VYB/YB PHYTER™ QFP Single Port 10/100 Mb/s Ethernet
 *              Physical Layer Transceiver Datasheet P32.
 *
 * 6.3.4 Internal Loopback
 *  The DP83848VYB includes a Loopback Test mode for facilitating system
 *  diagnostics. The Loopback mode is selected through bit 14 (Loopback)
 *  of the Basic Mode Control Register (BMCR). Writing 1 to this bit enables
 *  MII transmit data to be routed to the MII receive outputs.
 *  Loopback status may be checked in bit 3 of the PHY Status Register (PHYSTS).
 *  While in Loopback mode the data will not be transmitted onto the media.
 *  To ensure that the desired operating mode is maintained, Auto-Negotiation
 *  should be disabled before selecting the Loopback mode.
 *
 * NOTE: in config_phy.h
 *
 *      #define CONF_ETHERNET_PHY_LOOPBACK_EN 1
 *      // Set PHY be placed in a loopback mode of operation.
 *
 *      #define CONF_ETHERNET_PHY_CONTROL_AUTONEG_EN 0
 *      // Indicates whether the Auto-Negotiation enable or not
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "NuMicro.h"
#include "system.h"
#include "ethernet_phy.h"

#define TXBUFLEN 1514
#define RXBUFLEN 1514

uint8_t tx_buffer[TXBUFLEN] = {0};
uint8_t rx_buffer[RXBUFLEN] = {0};

char tx_msg[] = "Ethernet PHY Loopback Test!";
uint32_t packetLen;

volatile bool recv_flag = false;

// MAC address
uint8_t mac_addr[6] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};

void mac_layer_init(void);
void phy_layer_init(void);

void print_IEEE802_3_reg(void)
{
    uint16_t ethphy_reg;

    // IEEE802.3 PHY Register
    printf("\nIEEE802.3 PHY Register:\n");
    ethernet_phy_read_reg(ETH_PHY_BMCR    , &ethphy_reg);
    printf("\tBMCR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_BMCR    , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_BMSR    , &ethphy_reg);
    printf("\tBMSR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_BMSR    , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_PHYIDR1 , &ethphy_reg);
    printf("\tPHYIDR1  (0x%02Xh) : (0x%04X)\n", ETH_PHY_PHYIDR1 , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_PHYIDR2 , &ethphy_reg);
    printf("\tPHYIDR2  (0x%02Xh) : (0x%04X)\n", ETH_PHY_PHYIDR2 , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANAR    , &ethphy_reg);
    printf("\tANAR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANAR    , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANLPAR  , &ethphy_reg);
    printf("\tANLPAR   (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANLPAR  , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANLPARNP, &ethphy_reg);
    printf("\tANLPARNP (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANLPARNP, ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANER    , &ethphy_reg);
    printf("\tANER     (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANER    , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANNPTR  , &ethphy_reg);
    printf("\tANNPTR   (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANNPTR  , ethphy_reg);
    
    // IEEE802.3 PHY IC Extended Register
    printf("\nIEEE802.3 PHY IC Extended Register:\n");
    ethernet_phy_read_reg(ETH_PHY_DP83848_PHYSTS , &ethphy_reg);
    printf("\tPHYSTS   (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PHYSTS , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_MICR   , &ethphy_reg);
    printf("\tMICR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_MICR   , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_MISR   , &ethphy_reg);
    printf("\tMISR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_MISR   , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_FCSCR  , &ethphy_reg);
    printf("\tFCSCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_FCSCR  , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_RECR   , &ethphy_reg);
    printf("\tRECR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_RECR   , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_PCSR   , &ethphy_reg);
    printf("\tPCSR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PCSR   , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_RBR    , &ethphy_reg);
    printf("\tRBR      (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_RBR    , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_LEDCR  , &ethphy_reg);
    printf("\tLEDCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_LEDCR  , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_PHYCR  , &ethphy_reg);
    printf("\tPHYCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PHYCR  , ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_10BTSCR, &ethphy_reg);
    printf("\t10BTSCR  (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_10BTSCR, ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_CDCTRL1, &ethphy_reg);
    printf("\tCDCTRL1  (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_CDCTRL1, ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_EDCR   , &ethphy_reg);
    printf("\tEDCR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_EDCR   , ethphy_reg);
}

int main()
{
    system_init();
    printf("[test]: IEEE802.3 PHY Loopback Test ...\n\n");
    strncpy((char *) tx_buffer, tx_msg, strlen(tx_msg));

    mac_layer_init();
    phy_layer_init();

    print_IEEE802_3_reg();

    printf("--------------------------------------------\n\n");

    uint32_t res = EMAC_SendPkt(tx_buffer, TXBUFLEN);
    printf("\n[Response] EMAC_SendPkt: %ld\n", res);
    printf("Transmit to mac : %s\n", tx_buffer);

    while (1) {
        if (recv_flag) {
            printf("Receive from mac : %s\n\n", rx_buffer);
            break;
        }
    }
    return 0;
}

void mac_layer_init(void)
{
    EMAC_Open(mac_addr);

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

void EMAC_TX_IRQHandler(void)
{
    PH4 ^= 1;
    // Clean up Tx resource occupied by previous sent packet(s)
    EMAC_SendPktDone();
}

void EMAC_RX_IRQHandler(void)
{
    PH5 ^= 1;
    recv_flag = true;
    while (1) {
        // Check if there's any packets available
        if (EMAC_RecvPkt(rx_buffer, &packetLen) == 0)
            break;
        // Clean up Rx resource occupied by previous received packet
        EMAC_RecvPktDone();
    }
}

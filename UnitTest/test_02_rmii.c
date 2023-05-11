/**
 * @file test_02_rmii.c
 * @author cy023
 * @date 2023.04.20
 * @brief
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include "NuMicro.h"
#include "system.h"
#include "ethernet_phy.h"

// MAC address
// uint8_t mac_addr[6] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};
extern uint8_t mac_addr[6];

void mac_layer_init(void);
void phy_layer_init(void);

void print_IEEE802_3_reg(void)
{
    uint16_t ethphy_reg;

    // IEEE802.3 PHY Register
    printf("\nIEEE802.3 PHY Register:\n");
    ethernet_phy_read_reg(ETH_PHY_BMCR    , &ethphy_reg);
    printf("\tBMCR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_BMCR    ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_BMSR    , &ethphy_reg);
    printf("\tBMSR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_BMSR    ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_PHYIDR1 , &ethphy_reg);
    printf("\tPHYIDR1  (0x%02Xh) : (0x%04X)\n", ETH_PHY_PHYIDR1 ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_PHYIDR2 , &ethphy_reg);
    printf("\tPHYIDR2  (0x%02Xh) : (0x%04X)\n", ETH_PHY_PHYIDR2 ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANAR    , &ethphy_reg);
    printf("\tANAR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANAR    ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANLPAR  , &ethphy_reg);
    printf("\tANLPAR   (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANLPAR  ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANLPARNP, &ethphy_reg);
    printf("\tANLPARNP (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANLPARNP,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANER    , &ethphy_reg);
    printf("\tANER     (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANER    ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_ANNPTR  , &ethphy_reg);
    printf("\tANNPTR   (0x%02Xh) : (0x%04X)\n", ETH_PHY_ANNPTR  ,  ethphy_reg);
    
    // IEEE802.3 PHY IC Extended Register
    printf("\nIEEE802.3 PHY IC Extended Register:\n");
    ethernet_phy_read_reg(ETH_PHY_DP83848_PHYSTS , &ethphy_reg);
    printf("\tPHYSTS   (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PHYSTS ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_MICR   , &ethphy_reg);
    printf("\tMICR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_MICR   ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_MISR   , &ethphy_reg);
    printf("\tMISR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_MISR   ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_FCSCR  , &ethphy_reg);
    printf("\tFCSCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_FCSCR  ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_RECR   , &ethphy_reg);
    printf("\tRECR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_RECR   ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_PCSR   , &ethphy_reg);
    printf("\tPCSR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PCSR   ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_RBR    , &ethphy_reg);
    printf("\tRBR      (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_RBR    ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_LEDCR  , &ethphy_reg);
    printf("\tLEDCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_LEDCR  ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_PHYCR  , &ethphy_reg);
    printf("\tPHYCR    (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_PHYCR  ,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_10BTSCR, &ethphy_reg);
    printf("\t10BTSCR  (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_10BTSCR,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_CDCTRL1, &ethphy_reg);
    printf("\tCDCTRL1  (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_CDCTRL1,  ethphy_reg);
    ethernet_phy_read_reg(ETH_PHY_DP83848_EDCR   , &ethphy_reg);
    printf("\tEDCR     (0x%02Xh) : (0x%04X)\n", ETH_PHY_DP83848_EDCR   ,  ethphy_reg);
}

int main()
{
    system_init();
    printf("System Boot.\n");
    printf("[test]: Print IEEE802.3 PHY Registers ...\n");

    mac_layer_init();
    phy_layer_init();

    print_IEEE802_3_reg();
    return 0;
}

void mac_layer_init(void)
{
    EMAC_Open(mac_addr);
}

void phy_layer_init(void)
{
    ethernet_phy_reset();
    ethernet_phy_init();
}

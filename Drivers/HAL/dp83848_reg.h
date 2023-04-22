/**
 * @file dp83848_reg.h
 * @author cy023
 * @brief Registers of Ethernet PHY IC - DP83848
 * @date 2022.08.18
 *
 * Reference: DP83848C/I/VYB/YB PHYTER™ QFP Single Port 10/100 Mb/s Ethernet
 *        Physical Layer Transceiver datasheet (Rev. E) Table 6-7. Register Map
 */

#ifndef DP83848_REG_H
#define DP83848_REG_H

/*******************************************************************************
 * Register Map
 ******************************************************************************/

/* IEEE802.3 PHY REGISTERS */
#define ETH_PHY_BMCR                0x00    /*< Basic Mode Control Register */
#define ETH_PHY_BMSR                0x01    /*< Basic Mode Status Register */
#define ETH_PHY_PHYIDR1             0x02    /*< PHY Identifier Register #1 */
#define ETH_PHY_PHYIDR2             0x03    /*< PHY Identifier Register #2 */
#define ETH_PHY_ANAR                0x04    /*< Auto-Negotiation Advertisement Register */
#define ETH_PHY_ANLPAR              0x05    /*< Auto-Negotiation Link Partner Ability Register (Base Page) */
#define ETH_PHY_ANLPARNP            0x05    /*< Auto-Negotiation Link Partner Ability Register (Next Page) */
#define ETH_PHY_ANER                0x06    /*< Auto-Negotiation Expansion Register */
#define ETH_PHY_ANNPTR              0x07    /*< Auto-Negotiation Next Page TX */

/* EXTENDED REGISTERS */
#define ETH_PHY_DP83848_PHYSTS      0x10    /*< PHY Status Register */
#define ETH_PHY_DP83848_MICR        0x11    /*< MII Interrupt Control Register */
#define ETH_PHY_DP83848_MISR        0x12    /*< MII Interrupt Status Register */
#define ETH_PHY_DP83848_FCSCR       0x14    /*< False Carrier Sense Counter Register */
#define ETH_PHY_DP83848_RECR        0x15    /*< Receive Error Counter Register */
#define ETH_PHY_DP83848_PCSR        0x16    /*< PCS Sub-Layer Configuration and Status Register */
#define ETH_PHY_DP83848_RBR         0x17    /*< RMII and Bypass Register */
#define ETH_PHY_DP83848_LEDCR       0x18    /*< LED Direct Control Register */
#define ETH_PHY_DP83848_PHYCR       0x19    /*< PHY Control Register */
#define ETH_PHY_DP83848_10BTSCR     0x1A    /*< 10Base-T Status/Control Register*/
#define ETH_PHY_DP83848_CDCTRL1     0x1B    /*< CD Test Control Register and BIST Extensions Register */
#define ETH_PHY_DP83848_EDCR        0x1D    /*< Energy Detect Control Register */

/*******************************************************************************
 * Register Detail
 ******************************************************************************/

/* PHY BMCR register bit definitions */
#define ETH_PHY_BMCR_RESET_POS              (15)
#define ETH_PHY_BMCR_LOOPBACK_POS           (14)
#define ETH_PHY_BMCR_SPEED_SELECT_POS       (13)
#define ETH_PHY_BMCR_AUTONEG_EN_POS         (12)
#define ETH_PHY_BMCR_POWER_DOWN_POS         (11)
#define ETH_PHY_BMCR_ISOLATE_POS            (10)
#define ETH_PHY_BMCR_RESTART_AUTONEG_POS    (9)
#define ETH_PHY_BMCR_DUPLEX_MODE_POS        (8)
#define ETH_PHY_BMCR_COLLISION_TEST_POS     (7)

#define ETH_PHY_BMCR_RESET_MASK             (1 << ETH_PHY_BMCR_RESET_POS)
#define ETH_PHY_BMCR_LOOPBACK_MASK          (1 << ETH_PHY_BMCR_LOOPBACK_POS)
#define ETH_PHY_BMCR_SPEED_SELECT_MASK      (1 << ETH_PHY_BMCR_SPEED_SELECT_POS)
#define ETH_PHY_BMCR_AUTONEG_EN_MASK        (1 << ETH_PHY_BMCR_AUTONEG_EN_POS)
#define ETH_PHY_BMCR_POWER_DOWN_MASK        (1 << ETH_PHY_BMCR_POWER_DOWN_POS)
#define ETH_PHY_BMCR_ISOLATE_MASK           (1 << ETH_PHY_BMCR_ISOLATE_POS)
#define ETH_PHY_BMCR_RESTART_AUTONEG_MASK   (1 << ETH_PHY_BMCR_RESTART_AUTONEG_POS)
#define ETH_PHY_BMCR_DUPLEX_MODE_MASK       (1 << ETH_PHY_BMCR_DUPLEX_MODE_POS)
#define ETH_PHY_BMCR_COLLISION_TEST_MASK    (1 << ETH_PHY_BMCR_COLLISION_TEST_POS)

/* PHY BMSR register bit definitions */
#define ETH_PHY_BMSR_100BASE_T4_POS             (15) /*!< T4 mode */
#define ETH_PHY_BMSR_100BASE_TX_FDX_POS         (14) /*!< 100MBps full duplex */
#define ETH_PHY_BMSR_100BASE_TX_HDX_POS         (13) /*!< 100MBps half duplex */
#define ETH_PHY_BMSR_10BASE_T_FDX_POS           (12) /*!< 10MBps full duplex */
#define ETH_PHY_BMSR_10BASE_T_HDX_POS           (11) /*!< 10MBps half duplex */
#define ETH_PHY_BMSR_MF_PREAMBLE_SUPRESS_POS    (6)  /*!< Auto-negotation complete */
#define ETH_PHY_BMSR_AUTONEG_COMP_POS           (5)  /*!< Auto-negotation complete */
#define ETH_PHY_BMSR_REMOTE_FAULT_POS           (4)  /*!< Fault */
#define ETH_PHY_BMSR_AUTONEG_ABILITY_POS        (3)  /*!< Auto-negotation supported */
#define ETH_PHY_BMSR_LINK_STATUS_POS            (2)  /*!< 1=Link active */
#define ETH_PHY_BMSR_JABBER_DETECT_POS          (1)  /*!< Jabber detect */
#define ETH_PHY_BMSR_EXTEND_CAPABILITY_POS      (0)  /*!< Supports extended capabilities */

#define ETH_PHY_BMSR_100BASE_T4_MASK            (1 << ETH_PHY_BMSR_100BASE_T4_POS)
#define ETH_PHY_BMSR_100BASE_TX_FDX_MASK        (1 << ETH_PHY_BMSR_100BASE_TX_FDX_POS)
#define ETH_PHY_BMSR_100BASE_TX_HDX_MASK        (1 << ETH_PHY_BMSR_100BASE_TX_HDX_POS)
#define ETH_PHY_BMSR_10BASE_T_FDX_MASK          (1 << ETH_PHY_BMSR_10BASE_T_FDX_POS)
#define ETH_PHY_BMSR_10BASE_T_HDX_MASK          (1 << ETH_PHY_BMSR_10BASE_T_HDX_POS)
#define ETH_PHY_BMSR_MF_PREAMBLE_SUPRESS_MASK   (1 << ETH_PHY_BMSR_MF_PREAMBLE_SUPRESS_POS)
#define ETH_PHY_BMSR_AUTONEG_COMP_MASK          (1 << ETH_PHY_BMSR_AUTONEG_COMP_POS)
#define ETH_PHY_BMSR_REMOTE_FAULT_MASK          (1 << ETH_PHY_BMSR_REMOTE_FAULT_POS)
#define ETH_PHY_BMSR_AUTONEG_ABILITY_MASK       (1 << ETH_PHY_BMSR_AUTONEG_ABILITY_POS)
#define ETH_PHY_BMSR_LINK_STATUS_MASK           (1 << ETH_PHY_BMSR_LINK_STATUS_POS)
#define ETH_PHY_BMSR_JABBER_DETECT_MASK         (1 << ETH_PHY_BMSR_JABBER_DETECT_POS)
#define ETH_PHY_BMSR_EXTEND_CAPABILITY_MASK     (1 << ETH_PHY_BMSR_EXTEND_CAPABILITY_POS)

/* PHY ANLPAR register bit definitions */
#define ETH_PHY_ANLPAR_100BASE_TX_FDX_POS       (8) /*!< 100MBps full duplex */
#define ETH_PHY_ANLPAR_100BASE_TX_HDX_POS       (7) /*!< 100MBps half duplex */
#define ETH_PHY_ANLPAR_10BASE_T_FDX_POS         (6) /*!< 10MBps full duplex */
#define ETH_PHY_ANLPAR_10BASE_T_HDX_POS         (5) /*!< 10MBps half duplex */

#define ETH_PHY_ANLPAR_100BASE_TX_FDX_MASK      (1 << ETH_PHY_ANLPAR_100BASE_TX_FDX_POS)
#define ETH_PHY_ANLPAR_100BASE_TX_HDX_MASK      (1 << ETH_PHY_ANLPAR_100BASE_TX_HDX_POS)
#define ETH_PHY_ANLPAR_10BASE_T_FDX_MASK        (1 << ETH_PHY_ANLPAR_10BASE_T_FDX_POS)
#define ETH_PHY_ANLPAR_10BASE_T_HDX_MASK        (1 << ETH_PHY_ANLPAR_10BASE_T_HDX_POS)

#endif /* DP83848_REG_H */

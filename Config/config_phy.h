/**
 * @file config_phy.c
 * @author cy023
 * @date 2022.08.23
 * @brief PHY Layer configuration
 * 
 */

#ifndef CONFIG_PHY_H
#define CONFIG_PHY_H

#include "dp83848_reg.h"

/*******************************************************************************
 * Basic configuration
 ******************************************************************************/

// PHY Address <0-31>
//
// The PHY Address is five bits, allowing 32 unique PHY addresses. A PHY
// that is connected to the station management entity via the mechanical
// interface defined in IEEE 802.3 22.6 shall always respond to
// transactions addressed to PHY Address zero b00000. A station management
// entity that is attached to multiple PHYs must have prior knowledge of
// the appropriate PHY Address for each PHY.
#ifndef CONF_ETHERNET_PHY_ADDRESS
#define CONF_ETHERNET_PHY_ADDRESS	0x01
#endif

// Loopback Enable
//
// Set PHY be placed in a loopback mode of operation.
#ifndef CONF_ETHERNET_PHY_LOOPBACK_EN
#define CONF_ETHERNET_PHY_LOOPBACK_EN 0
#endif

// Speed Selection
//
// These bits select the PHY speed.
// 0: 10 Mb/s
// 1: 100 Mb/s
// 2: 1000 Mb/s (not support)
#ifndef CONF_ETHERNET_PHY_CONTROL_SPEED
#define CONF_ETHERNET_PHY_CONTROL_SPEED 1
#endif

// Auto-Negotiation Enable
//
// Indicates whether the Auto-Negotiation enable or not
#ifndef CONF_ETHERNET_PHY_CONTROL_AUTONEG_EN
#define CONF_ETHERNET_PHY_CONTROL_AUTONEG_EN 1
#endif

// Power Down Enable
//
// Set PHY in a low-power consumption state, The specific behavior of a
// PHY in the power-down state is implementation specific. While in the
// power-down state, the PHY shall respond to management transactions.
// During the transition to the power-down state and while in the
// power-down state, the PHY shall not generate spurious signals on the
// MII or GMII.
#ifndef CONF_ETHERNET_PHY_CONTROL_POWER_DOWN_EN
#define CONF_ETHERNET_PHY_CONTROL_POWER_DOWN_EN 0
#endif

// Isolate Enable
//
// Set PHY forced to electrically isolate its data paths from the MII or
// GMII. When the PHY is isolated from the MII or GMII it shall not
// respond to the TXD data bundle, TX_EN, TX_ER and GTX_CLK inputs, and it
// shall present a high impedance on its TX_CLK, RX_CLK, RX_DV, RX_ER, RXD
// data bundle, COL, and CRS outputs. When the PHY is isolated from the
// MII or GMII it shall respond to management transactions.
#ifndef CONF_ETHERNET_PHY_CONTROL_ISOLATE_EN
#define CONF_ETHERNET_PHY_CONTROL_ISOLATE_EN 0
#endif

// Duplex Mode Selection
//
// The duplex mode can be selected via either the Auto-Negotiation enable,
// or manual duplex selection. Manual duplex selection is allowed when
// Auto-Negotiation is disabled. When Auto-Negotiation is enabled, this
// setting has no effect on the link configuration.
//
// 0: half duplex
// 1: full duplex
#ifndef CONF_ETHERNET_PHY_CONTROL_DUPLEX_MODE
#define CONF_ETHERNET_PHY_CONTROL_DUPLEX_MODE 1
#endif

#ifndef CONF_ETHERNET_PHY_CONTROL_BMCR
#define CONF_ETHERNET_PHY_CONTROL_BMCR                                            	    \
    (CONF_ETHERNET_PHY_LOOPBACK_EN           ? ETH_PHY_BMCR_LOOPBACK_MASK        : 0) | \
    (CONF_ETHERNET_PHY_CONTROL_SPEED         ? ETH_PHY_BMCR_SPEED_SELECT_MASK    : 0) | \
    (CONF_ETHERNET_PHY_CONTROL_AUTONEG_EN    ? ETH_PHY_BMCR_RESTART_AUTONEG_MASK : 0) | \
    (CONF_ETHERNET_PHY_CONTROL_POWER_DOWN_EN ? ETH_PHY_BMCR_POWER_DOWN_MASK      : 0) | \
    (CONF_ETHERNET_PHY_CONTROL_ISOLATE_EN    ? ETH_PHY_BMCR_ISOLATE_MASK         : 0) | \
    (CONF_ETHERNET_PHY_CONTROL_DUPLEX_MODE   ? ETH_PHY_BMCR_DUPLEX_MODE_MASK     : 0)
#endif

#endif /* CONFIG_PHY_H */

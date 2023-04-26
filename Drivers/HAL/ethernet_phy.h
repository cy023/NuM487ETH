/**
 * @file ethernet_phy.h
 * @author cy023
 * @date 2022.08.21
 * @brief 
 * 
 */

#ifndef ETHERNET_PHY_H
#define ETHERNET_PHY_H

#include <stdint.h>
#include <stdbool.h>
#include "dp83848_reg.h"

/*******************************************************************************
 * Ethernet - Physical Layer
 ******************************************************************************/

void ethernet_phy_read_reg(uint16_t reg, uint16_t *val);
void ethernet_phy_write_reg(uint16_t reg, uint16_t val);
void ethernet_phy_set_reg_bit(uint16_t reg, uint16_t offset);
void ethernet_phy_clear_reg_bit(uint16_t reg, uint16_t offset);

void ethernet_phy_init(void);
void ethernet_phy_set_powerdown(bool state);
void ethernet_phy_set_isolate(bool state);
void ethernet_phy_restart_autoneg(void);
void ethernet_phy_set_loopback(bool state);
void ethernet_phy_get_link_status(bool *status);
void ethernet_phy_reset(void);

#endif /* ETHERNET_PHY_H */

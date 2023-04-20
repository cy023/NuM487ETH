/**
 * @file ethernet_phy.c
 * @author cy023
 * @date 2022.08.21
 * @brief
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include "NuMicro.h"

#include "config_phy.h"
#include "ethernet_phy.h"

#define ASSERT(condition)     \
    do {                      \
        if (condition == 0) { \
            printf("err\n");  \
        }                     \
    } while (0)

/*******************************************************************************
 * EMAC low level
 ******************************************************************************/

/**
 * @brief  Write PHY register
 * @param[in]  u32Reg PHY register number
 * @param[in]  u32Addr PHY address, this address is board dependent
 * @param[in] u32Data data to write to PHY register
 * @return None
 * @note This function sets g_EMAC_i32ErrCode to EMAC_TIMEOUT_ERR if
 * EMAC_MIIMCTL_BUSY_Msk busy bit does not auto clear after transfer done.
 */
static void EMAC_MdioWrite(uint32_t u32Reg, uint32_t u32Addr, uint32_t u32Data)
{
    /* From preamble to idle is 64-bit transfer, MDC shouldn't be slower than
     * 1MHz */
    uint32_t u32Delay = SystemCoreClock / 1000000 * 64;
    /* Set data register */
    EMAC->MIIMDAT = u32Data;
    /* Set PHY address, PHY register address, busy bit and write bit */
    EMAC->MIIMCTL = u32Reg | (u32Addr << 8) | EMAC_MIIMCTL_BUSY_Msk |
                    EMAC_MIIMCTL_WRITE_Msk | EMAC_MIIMCTL_MDCON_Msk;

    /* Wait write complete by polling busy bit. */
    while ((EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk) && (--u32Delay)) {
        ;
    }
    g_EMAC_i32ErrCode = u32Delay > 0 ? 0 : EMAC_TIMEOUT_ERR;
}

/**
 * @brief  Read PHY register
 * @param[in]  u32Reg PHY register number
 * @param[in]  u32Addr PHY address, this address is board dependent
 * @return Value read from PHY register
 * @note This function sets g_EMAC_i32ErrCode to EMAC_TIMEOUT_ERR if
 * EMAC_MIIMCTL_BUSY_Msk busy bit does not auto clear after transfer done.
 */
static uint32_t EMAC_MdioRead(uint32_t u32Reg, uint32_t u32Addr)
{
    /* From preamble to idle is 64-bit transfer, MDC shouldn't be slower than
     * 1MHz */
    uint32_t u32Delay = SystemCoreClock / 1000000 * 64;
    /* Set PHY address, PHY register address, busy bit */
    EMAC->MIIMCTL = u32Reg | (u32Addr << EMAC_MIIMCTL_PHYADDR_Pos) |
                    EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_MDCON_Msk;

    /* Wait read complete by polling busy bit */
    while ((EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk) && (--u32Delay)) {
        ;
    }
    g_EMAC_i32ErrCode = u32Delay > 0 ? 0 : EMAC_TIMEOUT_ERR;
    /* Get return data */
    return EMAC->MIIMDAT;
}

/*******************************************************************************
 * MDIO Operation
 ******************************************************************************/

void mac_write_phy_reg(uint16_t addr, uint16_t reg, uint16_t val)
{
    EMAC_MdioWrite(reg, addr, val);
}

void mac_read_phy_reg(uint16_t addr, uint16_t reg, uint16_t *val)
{
    *val = EMAC_MdioRead(reg, addr);
}

/*******************************************************************************
 * Read / Write, Set / Clear Operation
 ******************************************************************************/

void ethernet_phy_read_reg(uint16_t reg, uint16_t *val)
{
    ASSERT((reg <= 0x1F) && val);
    mac_read_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, val);
}

void ethernet_phy_write_reg(uint16_t reg, uint16_t val)
{
    ASSERT((reg <= 0x1F));
    mac_write_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, val);
}

void ethernet_phy_set_reg_bit(uint16_t reg, uint16_t offset)
{
    uint16_t val;
    ASSERT((reg <= 0x1F));
    mac_read_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, &val);
    val |= offset;
    mac_write_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, val);
}

void ethernet_phy_clear_reg_bit(uint16_t reg, uint16_t offset)
{
    uint16_t val;
    ASSERT((reg <= 0x1F));
    mac_read_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, &val);
    val &= ~offset;
    mac_write_phy_reg(CONF_ETHERNET_PHY_ADDRESS, reg, val);
}

/*******************************************************************************
 * Basic Operation
 ******************************************************************************/

void ethernet_phy_init(void)
{
    ethernet_phy_write_reg(ETH_PHY_BMCR, CONF_ETHERNET_PHY_CONTROL_BMCR);
}

void ethernet_phy_set_powerdown(bool state)
{
    if (state)
        ethernet_phy_set_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_POWER_DOWN_MASK);
    else
        ethernet_phy_clear_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_POWER_DOWN_MASK);
}

void ethernet_phy_set_isolate(bool state)
{
    if (state)
        ethernet_phy_set_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_ISOLATE_MASK);
    else
        ethernet_phy_clear_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_ISOLATE_MASK);
}

void ethernet_phy_restart_autoneg(void)
{
    ethernet_phy_set_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_RESTART_AUTONEG_MASK);
}

void ethernet_phy_set_loopback(bool state)
{
    if (state)
        ethernet_phy_set_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_LOOPBACK_MASK);
    else
        ethernet_phy_clear_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_LOOPBACK_MASK);
}

void ethernet_phy_get_link_status(bool *status)
{
    uint16_t val;
    mac_read_phy_reg(CONF_ETHERNET_PHY_ADDRESS, ETH_PHY_BMSR, &val);
    *status = (val & ETH_PHY_BMSR_LINK_STATUS_MASK) ? true : false;
}

void ethernet_phy_reset(void)
{
    ethernet_phy_set_reg_bit(ETH_PHY_BMCR, ETH_PHY_BMCR_RESET_MASK);
    uint16_t val = 0;
    ethernet_phy_read_reg(ETH_PHY_BMCR, &val);
    while (val & ETH_PHY_BMCR_RESET_MASK) {
        ethernet_phy_read_reg(ETH_PHY_BMCR, &val);
    }
}

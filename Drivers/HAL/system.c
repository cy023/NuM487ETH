/**
 * @file system.c
 * @author cy023
 * @date 2023.03.20
 * @brief
 */

#include "system.h"
#include "NuMicro.h"


/*******************************************************************************
 * Peripheral Driver - private function
 ******************************************************************************/
/**
 * @brief Clock peripheral initialization.
 */
static void system_clock_init(void)
{
    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable External XTAL (4~24 MHz) : 12MHz */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Switch HCLK clock source to HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Enable UART clock */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Enable EMAC clock */
    CLK_EnableModuleClock(EMAC_MODULE);

    /* Select UART clock source from HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT,
                       CLK_CLKDIV0_UART0(1));

    // Configure MDC clock rate to HCLK / (127 + 1) = 1.5 MHz if system is
    // running at 192 MHz
    CLK_SetModuleClock(EMAC_MODULE, 0, CLK_CLKDIV3_EMAC(127));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and
     * CyclesPerUs automatically. */
    SystemCoreClockUpdate();
}

/**
 * @brief Clock peripheral initialization.
 */
static void system_clock_deinit(void)
{
    // TODO:
}

/**
 * @brief GPIO peripheral initialization.
 *
 *  - PB.5 for prog/run dectect pin
 *  - PH.4 BOOT pin as output
 *  - PH.5 LED0 pin as output
 */
static void system_gpio_init(void)
{
    // set PB5 as prog/run detect pin
    GPIO_SetMode(PB, BIT5, GPIO_MODE_INPUT);

    // set BOOT pin (PH.4) as output
    GPIO_SetMode(PH, BIT4, GPIO_MODE_OUTPUT);

    // set LED0 pin (PH.5) as output
    GPIO_SetMode(PH, BIT5, GPIO_MODE_OUTPUT);
}

/**
 * @brief GPIO peripheral deinitialization.
 */
static void system_gpio_deinit(void)
{
    // TODO:
}

/**
 * @brief UART peripheral initialization.
 */
static void system_uart0_init(void)
{
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |=
        (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Init UART to 38400-8n1 for print message */
    UART_Open(UART0, 38400);
}

/**
 * @brief UART peripheral deinitialization.
 */
static void system_uart0_deinit(void)
{
    UART_Close(UART0);
}

/**
 * @brief For External Flash - w25q128jv
 *
 *  FLASH_MOSI  : PA.8
 *  FLASH_SCK   : PA.10
 *  FLASH_MISO  : PG.4
 *  FLASH_CS    : PA.11
 */
static void system_spi_init(void)
{
    // TODO:
}

static void system_spi_deinit(void)
{
    // TODO:
}

/**
 * @brief For Ethernet PHY - dp83848
 *
 *  ETH_RMII_MDC   : PE.8
 *  ETH_RMII_MDIO  : PE.9
 *  ETH_RMII_TXD0  : PE.10
 *  ETH_RMII_TXD1  : PE.11
 *  ETH_RMII_TXEN  : PE.12
 *  ETH_RMII_CLK   : PC.8
 *  ETH_RMII_RXD0  : PC.7
 *  ETH_RMII_RXD1  : PC.6
 *  ETH_RMII_CRSDV : PA.7
 *  ETH_RMII_RXER  : PA.6
 */
static void system_emac_init(void)
{
    // Configure RMII pins
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA6MFP_EMAC_RMII_RXERR | SYS_GPA_MFPL_PA7MFP_EMAC_RMII_CRSDV;
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC6MFP_EMAC_RMII_RXD1 | SYS_GPC_MFPL_PC7MFP_EMAC_RMII_RXD0;
    SYS->GPC_MFPH |= SYS_GPC_MFPH_PC8MFP_EMAC_RMII_REFCLK;
    SYS->GPE_MFPH |= SYS_GPE_MFPH_PE8MFP_EMAC_RMII_MDC |
                     SYS_GPE_MFPH_PE9MFP_EMAC_RMII_MDIO |
                     SYS_GPE_MFPH_PE10MFP_EMAC_RMII_TXD0 |
                     SYS_GPE_MFPH_PE11MFP_EMAC_RMII_TXD1 |
                     SYS_GPE_MFPH_PE12MFP_EMAC_RMII_TXEN;

    // Enable high slew rate on all RMII TX output pins
    PE->SLEWCTL = (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN10_Pos) |
                  (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN11_Pos) |
                  (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN12_Pos);
}

static void system_emac_deinit(void)
{
    // TODO:
}

/*******************************************************************************
 * Public Function
 ******************************************************************************/

void system_init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    system_gpio_init();
    system_clock_init();
    system_uart0_init();
    system_spi_init();
    system_emac_init();

    /* Lock protected registers */
    SYS_LockReg();
}

void system_deinit(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    system_emac_deinit();
    system_spi_deinit();
    system_uart0_deinit();
    system_clock_deinit();
    system_gpio_deinit();

    /* Lock protected registers */
    SYS_LockReg();
}

void system_delay_ms(uint32_t ms)
{
    uint16_t delay;
    volatile uint32_t i;
    for (delay = ms; delay > 0; delay--) {
        // 1 ms loop with -O0 optimization.
        for (i = 19200; i > 0; i--) {
            ;
        }
    }
}

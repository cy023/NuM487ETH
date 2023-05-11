/**
 * @file test_emac_TimeStamp.c
 * @author cy023
 * @date 2023.04.20
 * @brief
 *
 */

#include <stdio.h>
#include "NuMicro.h"
#include "system.h"

// MAC address
// uint8_t mac_addr[6] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};
extern uint8_t mac_addr[6];

void EMAC_TX_IRQHandler(void)
{
    if (EMAC_GET_ALARM_FLAG()) {
        printf(
            "Alarm interrupt!! Rewind current time by 5 second : 0 nano "
            "second\n");
        // First parameter set 1 means rewind current time, second parameter is
        // second, and third parameter is nano second
        EMAC_UpdateTime(1, 5, 0);
    }

    // This sample sends no packet out, so this must be an alarm interrupt
    EMAC_CLR_ALARM_FLAG();
}

int main(void)
{
    uint32_t s = 0, ns = 0;  // current time
    uint32_t old_s = 0;

    system_init();
    printf("EMAC Timer Stamp Sample\n");

    // Open EMAC interface. No need to enable Rx and Tx in this sample
    // But still connect Ethernet cable to pass auto-negotiation.
    EMAC_Open(mac_addr);

    // Init phy
    EMAC_PhyInit();

    // Init time stamp and set current time to 1000s:0ns
    EMAC_EnableTS(1000, 0);

    // Time stamp alarm triggers Tx interrupt, so no need to enable Rx interrupt
    // in this sample
    NVIC_EnableIRQ(EMAC_TX_IRQn);

    // Set Alarm at 1010s:0ns
    EMAC_EnableAlarm(1010, 0);

    while (1) {
        EMAC_GetTime(&s, &ns);
        if (s != old_s) {
            printf("Current time %lds. %ldns\n", s, ns);
            old_s = s;
        }
    }
}

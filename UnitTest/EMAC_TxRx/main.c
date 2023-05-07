#include <stdio.h>
#include "NuMicro.h"
#include "system.h"
#include "net.h"

// Our MAC address
uint8_t mac_addr[6] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};

// Our IP address
uint8_t volatile ip_addr[4] = {0, 0, 0, 0};

uint8_t rx_buffer[1514];
uint32_t packetLen;

// Descriptor pointers holds current Tx and Rx used by IRQ handler here.
uint32_t u32CurrentTxDesc, u32CurrentRxDesc;

void EMAC_TX_IRQHandler(void)
{
    // Clean up Tx resource occupied by previous sent packet(s)
    EMAC_SendPktDone();
}

void EMAC_RX_IRQHandler(void)
{
    while (1) {
        // Check if there's any packets available
        if (EMAC_RecvPkt(rx_buffer, &packetLen) == 0)
            break;
        // Process receive packet
        process_rx_packet(rx_buffer, packetLen);
        // Clean up Rx resource occupied by previous received packet
        EMAC_RecvPktDone();
    }
}


int main()
{
    system_init();
    printf("[Test] EMAC_TxRx\n");

    // Select RMII interface by default
    EMAC_Open(mac_addr);

    // Init phy
    EMAC_PhyInit();

    NVIC_EnableIRQ(EMAC_TX_IRQn);
    NVIC_EnableIRQ(EMAC_RX_IRQn);

    EMAC_ENABLE_RX();
    EMAC_ENABLE_TX();

    if (dhcp_start() < 0) {
        // Cannot get a DHCP lease, use static IP.
        printf("DHCP failed, use static IP 192.168.10.10\n");
        ip_addr[0] = 0xC0;
        ip_addr[1] = 0xA8;
        ip_addr[2] = 0x0A;
        ip_addr[3] = 0x0A;
    }
    while (1)
        ;
}

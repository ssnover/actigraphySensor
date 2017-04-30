/*
 * File: mcp2515.c
 * Author: Shane Snover <ssnover05@gmail.com>
 */

#include "mcp2515.h"
#include "spi.h"

#define CMD_SPI_RESET   (0xC0)

/* ============================================================================
 * Private Structure Definitions
 * ============================================================================
*/

typedef enum mcp2515_registers_t
{
    MCP2515_TXRTSCTRL = 0x0D,
    MCP2515_CANCTRL   = 0x0F,
    MCP2515_CNF3      = 0x28,
    MCP2515_CNF2      = 0x29,
    MCP2515_CNF1      = 0x2A,
    MCP2515_TXB0CTRL  = 0x30,
    MCP2515_TXB0SIDH  = 0x31,
    MCP2515_TXB0SIDL  = 0x32,
    MCP2515_TXB0DLC   = 0x35,
    MCP2515_TXB1CTRL  = 0x40,
    MCP2515_TXB1SIDH  = 0x41,
    MCP2515_TXB1SIDL  = 0x42,
    MCP2515_TXB1DLC   = 0x45,
    MCP2515_TXB2CTRL  = 0x50,
    MCP2515_TXB2SIDH  = 0x51,
    MCP2515_TXB2SIDL  = 0x52,
    MCP2515_TXB2DLC   = 0x55,
} mcp2515_registers_t;


/* ============================================================================
 * Private Structure Definitions
 * ============================================================================
*/

typedef union mcp2515_canctrl_S
{
    uint8_t reg;
    struct
    {
        /* LSB */
        uint8_t clkpre :2;
        uint8_t clken  :1;
        uint8_t osm    :1;
        uint8_t abat   :1;
        uint8_t reqop  :3;
        /* MSB */
    };
} mcp2515_canctrl_S;


typedef union mcp2515_txbnctrl_S
{
    uint8_t reg;
    struct
    {
        /* LSB */
        uint8_t txp     :2;
        uint8_t blank   :1;
        uint8_t txreq   :1;
        uint8_t txerr   :1;
        uint8_t mloa    :1;
        uint8_t abtf    :1;
        uint8_t blank   :1;
        /* MSB */
    };
} mcp2515_txbnctrl_S;


typedef union mcp2515_txbnsid_S
{
    uint16_t reg;
    struct
    {
        /* High Byte. LSB. */
        uint8_t sidh    :8;
        /* High Byte. MSB. */
        /* Low Byte. LSB. */
        uint8_t eidh    :2;
        uint8_t blank   :1;
        uint8_t exide   :1;
        uint8_t blank   :1;
        uint8_t sidl    :3;
        /* Low Byte. MSB. */
    };
} mcp2515_txbnsid_S;


/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/
static mcp_mode_t deviceMode = MCP2515_MODE_NORMAL;

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/

void spiReset(void);
void spiWriteRegister(mcp2515_registers_t reg, uint8_t data);
void spiReadRegister(mcp2515_registers_t reg);
void setBaudRate(void);


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void mcp2515_init(void)
{
    spiReset();

    mcp2515_setMode(MCP2515_MODE_CONFIG);
    setBaudRate();
    initBuffers();
}


void mcp2515_setMode(mcp_mode_t mode)
{
    deviceMode = mode;
    mcp2515_canctrl_S canctrl;

    canctrl.reg = spiReadRegister(MCP2515_CANCTRL);
    canctrl.reqop = (uint8_t) mode;

    spiWriteRegister(MCP2515_CANCTRL, canctrl.reg);

    return;
}


void setBaudRate(void)
{
    /* Eventually this should be programmable for a few different settings,
     * but for now this sets for external oscillator of 16 MHz and bus
     * baud rate of 250 Kbps. */

    spiWriteRegister(MCP2515_CNF1, 0x41);
    spiWriteRegister(MCP2515_CNF2, 0xF1);
    spiWriteRegister(MCP2515_CNF3, 0x85);

    return;
}


void initBuffers(void)
{
    /* ============= CLEARING TX BUFFERS ============= */
    /* For each buffer, there are 14 registers. One for configuration, two for
     * the standard ID, two for the extended ID, and one for the DLC. Finally,
     * there are 8 bytes for holding the message. These must be cleared on
     * initialization.
     */
    for (uint8_t i = 0; i < 14; i++)
    {
        spiWriteRegister(MCP2515_TXB0CTRL+i, 0x00);
        spiWriteRegister(MCP2515_TXB1CTRL+i, 0x00);
        spiWriteRegister(MCP2515_TXB2CTRL+i, 0x00);
    }

    /* ============= CLEARING RX BUFFERS ============= */
    
}


void spiReset(void)
{
    spi_transfer(CMD_SPI_RESET);
    return;
}


void spiWriteRegister(mcp2515_register_t reg, uint8_t data)
{

}

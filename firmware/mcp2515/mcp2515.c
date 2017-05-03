/*
 * File: mcp2515.c
 * Author: Shane Snover <ssnover05@gmail.com>
 */

#include <stdint.h>
#include <stdbool.h>
#include "mcp2515.h"
#include "spi.h"

#define CMD_SPI_RESET   (0xC0)

/* ============================================================================
 * Private Structure Definitions
 * ============================================================================
*/

typedef enum mcp2515_registers_t
{
    MCP2515_RXF0SIDH  = 0x00,
    MCP2515_RXF0SIDL  = 0x01,
    MCP2515_RXF1SIDH  = 0x04,
    MCP2515_RXF1SIDL  = 0x05,
    MCP2515_RXF2SIDH  = 0x08,
    MCP2515_RXF2SIDL  = 0x09,
    MCP2515_BFPCTRL   = 0x0C,
    MCP2515_TXRTSCTRL = 0x0D,
    MCP2515_CANSTAT   = 0x0E,
    MCP2515_CANCTRL   = 0x0F,
    MCP2515_RXF3SIDH  = 0x10,
    MCP2515_RXF3SIDL  = 0x11,
    MCP2515_RXF4SIDH  = 0x14,
    MCP2515_RXF4SIDL  = 0x15,
    MCP2515_RXF5SIDH  = 0x18,
    MCP2515_RXF5SIDL  = 0x19,
    MCP2515_TEC       = 0x1C,
    MCP2515_REC       = 0x1D,
    MCP2515_RXM0SIDH  = 0x20,
    MCP2515_RXM0SIDL  = 0x21,
    MCP2515_RXM1SIDH  = 0x24,
    MCP2515_RXM1SIDL  = 0x25,
    MCP2515_CNF3      = 0x28,
    MCP2515_CNF2      = 0x29,
    MCP2515_CNF1      = 0x2A,
    MCP2515_CANINTE   = 0x2B,
    MCP2515_CANINTF   = 0x2C,
    MCP2515_EFLG      = 0x2D,
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


typedef enum mcp2515_idFilterMode_t
{
    MCP2515_FILTER_NONE = 00u,
    MCP2515_FILTER_STD  = 01u,
    MCP2515_FILTER_EXT  = 10u,
    MCP2515_FILTER_OFF  = 11u,
} mcp2515_idFilterMode_t;


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
        uint8_t blank2  :1;
        /* MSB */
    };
} mcp2515_txbnctrl_S;


typedef union mcp2515_rxbnctrl_S
{
    uint8_t reg;
    struct
    {
        /* LSB */
        uint8_t filhit  :1;
        uint8_t bukt1   :1;
        uint8_t bukt    :1;
        uint8_t rxrtr   :1;
        uint8_t blank   :1;
        uint8_t rxm     :2;
        uint8_t blank2  :1;
        /* MSB */
    };
} mcp2515_rxbnctrl_S;


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
        uint8_t blank2  :1;
        uint8_t sidl    :3;
        /* Low Byte. MSB. */
    };
} mcp2515_txbnsid_S;


typedef union mcp2515_txbndlc_S
{
    uint8_t reg;
    struct
    {
        /* LSB */
        dlc     :4;
        blank   :2;
        rtr     :1;
        blank2  :1;
        /* MSB */
    };
} mcp2515_txbndlc_S;


typedef union mcp2515_rxfnsid_S
{
    uint16_t reg;
    struct
    {
        /* High Byte. LSB. */
        sidh        :8;
        /* High Byte. MSB. */
        /* Low Byte. LSB. */
        eidh        :2;
        blank       :1;
        exide       :1;
        blank2      :1;
        sidl        :3;
        /* Low Byte. MSB. */
    };
} mcp2515_rxfnsid_S;


typedef union mcp2515_bfpctrl_S
{
    uint8_t reg;
    struct
    {
        /* LSB. */
        uint8_t b0bfm   :1;
        uint8_t b1bfm   :1;
        uint8_t b0bfe   :1;
        uint8_t b1bfe   :1;
        uint8_t b0bfs   :1;
        uint8_t b1bfs   :1;
        uint8_t blank   :2;
        /* MSB. */
    };
} mcp2515_bfpctrl_S;


typedef union mcp2515_canstat_S
{
    uint8_t reg;
    struct
    {
        /* LSB. */
        uint8_t blank   :1;
        uint8_t icod    :3;
        uint8_t blank2  :1;
        uint8_t opmod   :3;
        /* MSB. */
    };
} mcp2515_canstat_S;


typedef union mcp2515_rxmnsid_S
{
    uint16_t reg;
    struct
    {
        /* High Byte. */
        uint8_t sidh    :8;
        /* Low Byte. LSB. */
        uint8_t eidh    :2;
        uint8_t blank   :3;
        uint8_t sidl    :3;
        /* Low Byte. MSB. */
    };
} mcp2515_rxmnsid_S;


/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/
static mcp_mode_t deviceMode = MCP2515_MODE_CONFIG;

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/

void spiReset(void);
void writeRegister(mcp2515_registers_t reg, uint8_t data);
uint8_t readRegister(mcp2515_registers_t reg);
void setBaudRate(void);
void initBuffers(void);
void setFilterMode(mcp2515_idFilterMode_t mode);


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
    setFilterMode(MCP2515_FILTER_NONE);
    deviceMode = MCP2515_MODE_NORMAL;
    mcp2515_setMode(MCP2515_MODE_NORMAL);
    return;
}


void mcp2515_setMode(mcp_mode_t mode)
{
    deviceMode = mode;
    mcp2515_canctrl_S canctrl;

    canctrl.reg = readRegister(MCP2515_CANCTRL);
    canctrl.reqop = (uint8_t) mode;

    writeRegister(MCP2515_CANCTRL, canctrl.reg);

    return;
}


bool mcp2515_transmitMessage(mcp2515_message_S * message)
{
    /* Find an empty buffer and load it. */
    mcp2515_txbnctrl_S buf_ctrl.reg = readRegister(MCP2515_TXB0CTRL);
    if (!buf_ctrl.txreq)
    {
        return loadTransmitBuffer(MCP2515_TX_BUFFER_0, message);
    }

    buf_ctrl.reg = readRegister(MCP2515_TXB1CTRL);
    if (!buf_ctrl.txreq)
    {
        return loadTransmitBuffer(MCP2515_TX_BUFFER_1, message);
    }

    buf_ctrl.reg = readRegister(MCP2515_TXB2CTRL);
    if (!buf_ctrl.txreq)
    {
        return loadTransmitBuffer(MCP2515_TX_BUFFER_2, message);
    }

    /* No empty transmit buffers at this time. */
    return false;

}


void setBaudRate(void)
{
    /* Eventually this should be programmable for a few different settings,
     * but for now this sets for external oscillator of 16 MHz and bus
     * baud rate of 250 Kbps. */

    writeRegister(MCP2515_CNF1, 0x41);
    writeRegister(MCP2515_CNF2, 0xF1);
    writeRegister(MCP2515_CNF3, 0x85);

    return;
}


void initBuffers(void)
{
    /* ============= CLEARING TX CONFIG & BUFFERS ============= */
    /* For each buffer, there are 14 registers. One for configuration, two for
     * the standard ID, two for the extended ID, and one for the DLC. Finally,
     * there are 8 bytes for holding the message. These must be cleared on
     * initialization.
     */
    for (uint8_t i = 0; i < 14; i++)
    {
        writeRegister(MCP2515_TXB0CTRL+i, 0x00);
        writeRegister(MCP2515_TXB1CTRL+i, 0x00);
        writeRegister(MCP2515_TXB2CTRL+i, 0x00);
    }

    /* ============= CLEARING RX MASKS & FILTERS ============= */
    writeRegister(MCP2515_RXF0SIDH, 0x00);
    writeRegister(MCP2515_RXF1SIDH, 0x00);
    writeRegister(MCP2515_RXF2SIDH, 0x00);
    writeRegister(MCP2515_RXF3SIDH, 0x00);
    writeRegister(MCP2515_RXF4SIDH, 0x00);
    writeRegister(MCP2515_RXF5SIDH, 0x00);
    writeRegister(MCP2515_RXF0SIDL, 0x00);
    writeRegister(MCP2515_RXF1SIDL, 0x00);
    writeRegister(MCP2515_RXF2SIDL, 0x00);
    writeRegister(MCP2515_RXF3SIDL, 0x00);
    writeRegister(MCP2515_RXF4SIDL, 0x00);
    writeRegister(MCP2515_RXF5SIDL, 0x00);
    writeRegister(MCP2515_RXM0SIDH, 0x00);
    writeRegister(MCP2515_RXM1SIDH, 0x00);
    writeRegister(MCP2515_RXM0SIDL, 0x00);
    writeRegister(MCP2515_RXM1SIDL, 0x00);

    /* ============= CLEARING RX CONFIG ============= */
    writeRegister(MCP2515_RXB0CTRL, 0x00);
    writeRegister(MCP2515_RXB1CTRL, 0x00);

    return;
}


void setFilterMode(mcp2515_idFilterMode_t mode)
{
    mcp2515_rxbnctrl_S ctrl;
    ctrl.reg = readRegister(MCP2515_RXB0CTRL);
    ctrl.rxm = (uint8_t) mode;

    return;
}


void spiReset(void)
{
    spi_select(MCP2515_CS);
    spi_transfer(CMD_SPI_RESET);
    spi_deselect(MCP2515_CS);

    return;
}


void writeRegister(mcp2515_register_t reg, uint8_t data)
{
    spi_select(MCP2515_CS);
    spi_transfer((uint8_t) reg);
    spi_transfer(data);
    spi_deselect(MCP2515_CS);

    return;
}


uint8_t readRegister(mcp2515_register_t reg)
{
    uint8_t data;

    spi_select(MCP2515_CS);
    spi_transfer((uint8_t) reg);
    data = spi_transfer(0);
    spi_deselect(MCP2515_CS);

    return data;
}

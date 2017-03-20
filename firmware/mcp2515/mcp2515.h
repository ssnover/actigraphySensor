/*
 * File:    mcp2515.h
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Simple library for interfacing with the MCP2515 CAN Controller with the
 * Atmel MegaAVR via SPI communication.
 *
 * Developed with help of the Arduino CAN Shield library in C++ by Seeed
 * Technology Inc. and Cory J. Fowler. Their repositories can be found at the
 * following linkes respectively:
 *  - https://github.com/reeedstudio/CAN_BUS_Shield
 *  - https://github.com/coryjfowler/MCP_CAN_lib
 */

#ifndef MCP2515_H
#define MCP2515_H

#include <stdbool.h>

/* ============================================================================
 * Enumerated Type Definitions
 * ============================================================================
*/

typedef enum can_baud_t
{
    CAN_5KBPS,
    CAN_10KBPS,
    CAN_20KBPS,
    CAN_40KBPS,
    CAN_50KBPS,
    CAN_80KBPS,
    CAN_100KBPS,
    CAN_125KBPS,
    CAN_200KBPS,
    CAN_250KBPS,
    CAN_500KBPS,
    CAN_1000KBPS
} can_baud_t;


/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Confirms connection to the CAN controller and sets the baud rate for it
 * to communicate on the bus with.
 *
 * Arguments:
 *  - baud: The baud rate of the CAN bus.
 *
 * Returns:
 *  - A boolean representing whether the device was successfully initialized.
 */
bool mcp2515_init(can_baud_t baud);


#endif /* MCP2515_H */

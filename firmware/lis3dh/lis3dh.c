/*
 * File:    lis3dh.c
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Implements an interface for using the LIS3DH accelerometer via SPI.
*/

#include "lis3dh.h"
#include <avr/io.h>

/* ============================================================================
 * Private Preprocessor Macros
 * ============================================================================
*/
#define SPI_DDR             DDRB
#define SPI_PORT            PORTB
#define LIS3DH_CS           PB1
#define LIS3DH_DEVICE_ID    0b00110011

/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/

uint8_t ctrl_reg1 = 0b00110111;
uint8_t ctrl_reg2 = 0b00000000;
uint8_t ctrl_reg3 = 0b00000000;
uint8_t ctrl_reg4 = 0b11011000;


/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/

/* Read a value from one of the LIS3DH's registers.
 *
 * Arguments:
 *  - reg_addr: The byte representing the LIS3DH's register to read.
 *
 * Returns:
 *  - The byte of data stored in the requested register.
 */
uint8_t readRegister(lis3dh_reg_t reg_addr);


/* Write data to one of the LIS3DH's internal registers.
 *
 * Arguments:
 *  - reg_addr: The byte representing the LIS3DH's register.
 *  - data: The byte of data to write to the register.
 */
void writeRegister(lis3dh_reg_t reg_addr, uint8_t data);

/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

/* Initializes the accelerometer to a default configuration with SPI
 * communication enabled. */
bool lis3dh_init()
{
    /* Make sure the chip select pin is set as output. */
    SPI_DDR |= (1 << LIS3DH_CS);

    if (readRegister(LIS3DH_REG_WHOAMI) != LIS3DH_DEVICE_ID)
    {
        return false;
    }

    /* Configure with base configurations. */
    /* Enables all 3 axes and 25 Hz sampling rate. */
    writeRegister(LIS3DH_REG_CTRL1, ctrl_reg1);
    /* Sets for block data updating, data MSB at the lower address
     * +/- 4G output scale, and high resolution mode. */
    writeRegister(LIS3DH_REG_CTRL4, ctrl_reg4);

    return true;
}


/* Set the maximum and minimum range of the acceleration measurements. */
void lis3dh_setRange(lis3dh_range_t range)
{
    ctrl_reg4 |= (range << 4);
    writeRegister(LIS3DH_REG_CTRL4, ctrl_reg4);
    return;
}


/* Get the current range setting for the LIS3DH acceleration measurements. */
lis3dh_range_t lis3dh_getRange()
{
    return ((ctrl_reg4 >> 4) & 0b0011);
}


/* Set the sampling rate of the LIS3DH accelerometer. */
void lis3dh_setDataRate(lis3dh_datarate_t data_rate)
{
    ctrl_reg1 |= (data_rate << 4);
    writeRegister(ctrl_reg1);
    return;
}


/* Get the current sampling rate setting for the LIS3DH. */
lis3dh_datarate_t lis3dh_getDataRate()
{
    return (ctrl_reg1 >> 4);
}


/* Request the current acceleration data from the LIS3DH. */
lis3dh_accelerometer_data_t lis3dh_read()
{
    lis3dh_accelerometer_data_t accel_data;
    uint8_t temp_high, temp_low;

    temp_high = readRegister(LIS3DH_REG_X_H);
    temp_low = readRegister(LIS3DH_REG_X_L);
    accel_data.x = (temp_high << 8 ) | temp_low;

    temp_high = readRegister(LIS3DH_REG_Y_H);
    temp_low = readRegister(LIS3DH_REG_Y_L);
    accel_data.y = (temp_high << 8 ) | temp_low;

    temp_high = readRegister(LIS3DH_REG_Z_H);
    temp_low = readRegister(LIS3DH_REG_Z_L);
    accel_data.z = (temp_high << 8 ) | temp_low;

    return accel_data;
}


/* Read a value from one of the LIS3DH's registers. */
uint8_t readRegister(lis3dh_reg_t reg_addr)
{
    SPI_PORT &= ~(1 << LIS3DH_CS);
    /* Setting bit 7 to 1 in order to denote a read. */
    spi_transfer(reg_addr | 0b10000000);
    /* Send nothing, but read in the bytes received. */
    uint8_t data = spi_transfer(0x00);

    SPI_PORT |= (1 << LIS3DH_CS);

    return data;
}


/* Write data to one of the LIS3DH's internal registers. */
void writeRegister(lis3dh_reg_t reg_addr, uint8_t data)
{
    SPI_PORT &= ~(1 << LIS3DH_CS);
    spi_transfer(reg_addr);
    spi_transfer(data);
    SPI_PORT |= (1 << LIS3DH_CS);
    
    return;
}

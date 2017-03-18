/*
 * File:    lis3dh.h
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Includes function declarations for interfacing with the Adafruit LIS3DH
 * accelerometer breakout board's accelerometer via SPI.
 *
 * Loosely based off of Adafruit's Arduino C++ library source found here:
 * https://github.com/adafruit/Adafruit_LIS3DH/
 *
 * The board this library is written for can be purchased here:
 * https://www.adafruit.com/products/2809
 */

#ifndef LIS3DH_H
#define LIS3DH_H

/* ============================================================================
 * Enumerated Type Definitions
 * ============================================================================
*/

typedef enum lis3dh_reg_t
{
    LIS3DH_REG_STATUS1      = 0x07,
    LIS3DH_REG_STATUS2      = 0x27,
    LIS3DH_REG_INTCOUNT     = 0x0E,
    LIS3DH_REG_WHOAMI       = 0x0F,
    LIS3DH_REG_TEMPCFG      = 0x1F,
    LIS3DH_REG_CTRL1        = 0x20,
    LIS3DH_REG_CTRL2        = 0x21,
    LIS3DH_REG_CTRL3        = 0x22,
    LIS3DH_REG_CTRL4        = 0x23,
    LIS3DH_REG_CTRL5        = 0x24,
    LIS3DH_REG_CTRL6        = 0x25,
    LIS3DH_REG_REFERENCE    = 0x26,
    LIS3DH_REG_OUT_X_L      = 0x28,
    LIS3DH_REG_OUT_X_H      = 0x29,
    LIS3DH_REG_OUT_Y_L      = 0x2A,
    LIS3DH_REG_OUT_Y_H      = 0x2B,
    LIS3DH_REG_OUT_Z_L      = 0x2C,
    LIS3DH_REG_OUT_Z_H      = 0x2D,
    LIS3DH_REG_FIFO_CTRL    = 0x2E,
    LIS3DH_REG_FIFO_SRC     = 0x2F
} lis3dh_reg_t;


typedef enum lis3dh_range_t
{
  LIS3DH_RANGE_16_G         = 0b11,
  LIS3DH_RANGE_8_G          = 0b10,
  LIS3DH_RANGE_4_G          = 0b01,
  LIS3DH_RANGE_2_G          = 0b00
} lis3dh_range_t;


typedef enum lis3dh_axis_t
{
    LIS3DH_AXIS_X,
    LIS3DH_AXIS_Y,
    LIS3DH_AXIS_Z
} lis3dh_axis_t;


typedef enum lis3dh_datarate_t
{
  LIS3DH_DATARATE_400_HZ     = 0b0111,
  LIS3DH_DATARATE_200_HZ     = 0b0110,
  LIS3DH_DATARATE_100_HZ     = 0b0101,
  LIS3DH_DATARATE_50_HZ      = 0b0100,
  LIS3DH_DATARATE_25_HZ      = 0b0011,
  LIS3DH_DATARATE_10_HZ      = 0b0010,
  LIS3DH_DATARATE_1_HZ       = 0b0001,
  LIS3DH_DATARATE_POWERDOWN  = 0,
  LIS3DH_DATARATE_LOWPOWER_1K6HZ  = 0b1000,
  LIS3DH_DATARATE_LOWPOWER_5KHZ  =  0b1001
} lis3dh_datarate_t;


#endif /* LIS3DH_H */
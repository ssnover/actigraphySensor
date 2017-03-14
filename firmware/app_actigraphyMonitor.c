/*
 * File:    app_actigraphyMonitor.c
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Runs the main application of the actigraphy sensor system.
 */

#include "app_actigraphyMonitor.h"
#include "spi.h"
#include "lis3dh.h"
#include "mcp2515.h"

/* ============================================================================
 * Private Module Variables
 * ============================================================================
 */

app_opstatus_t init_flag = APP_ERROR;
app_opstatus_t rx_flag = APP_ERROR;
app_opstatus_t upload_flag = APP_ERROR;
uint8_t accelerometer_data[6];


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
 */

/* Begins and queues all initialization operations. */
void app_actigraphyMonitor_init()
{
    init_flag = APP_INIT_INCOMPLETE;

    spi_init();

    if (!lis3dh_init())
    {
        init_flag = APP_ERROR;
    }
    /* Additional LIS3DH settings. */

    if (!mcp2515_init())
    {
        init_flag = APP_ERROR;
    }
    /* Additional MCP2515 settings. */

    init_flag = APP_INIT_COMPLETE;

    return;
}


/* Sends a request to the sensor for data. */
void app_actigraphyMonitor_requestData()
{
    rx_flag = APP_WAITING_FOR_DATA;

    uint16_t accel_temp = lis3dh_readX();
    accelerometer_data[0] = accel_temp >> 8;
    accelerometer_data[1] = accel_temp & 0xFF;

    accel_temp = lis3dh_readY();
    accelerometer_data[2] = accel_temp >> 8;
    accelerometer_data[3] = accel_temp & 0xFF;

    accel_temp = lis3dh_readZ();
    accelerometer_data[4] = accel_temp >> 8;
    accelerometer_data[5] = accel_temp & 0xFF;

    rx_flag = APP_DATA_RECEIVED;

    return;
}


/* Begins sending data packets to CAN controller. */
void app_actigraphyMonitor_sendData()
{
    upload_flag = APP_SENDING;

    mcp2515_sendPacket(&accelerometer_data, sizeof(accelerometer_data));

    upload_flag = APP_DATA_SENT;
}


/* Checks if initialization routine is completed. */
app_opstatus_t app_actigraphyMonitor_getInitStatus()
{
    return init_flag;
}


/* Checks if sensor data has been received. */
app_opstatus_t app_actigraphyMonitor_dataReceived()
{
    return rx_flag;
}


/* Checks if entire data packet has finished uploading. */
app_opstatus_t app_actigraphyMonitor_dataUploaded()
{
    return upload_flag;
}

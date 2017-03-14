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


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
 */

/* Begins and queues all initialization operations. */
void app_actigraphyMonitor_init()
{
    init_flag = APP_INIT_INCOMPLETE;

    spi_init();
    lis3dh_init();
    mcp2515_init();

    init_flag = APP_INIT_COMPLETE;
}


/* Sends a request to the sensor for data. */
void app_actigraphyMonitor_requestData()
{
    rx_flag = APP_WAITING_FOR_DATA;

    /* Logic. */
}


/* Begins sending data packets to CAN controller. */
void app_actigraphyMonitor_sendData()
{
    upload_flag = APP_SENDING;

    /* Logic. */
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

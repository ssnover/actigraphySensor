/* 
 * File:    app_actigraphyMonitor.h
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Contains high-level and state machine function declarations.
 */

#ifndef APP_ACTIGRAPHY_MONITOR
#define APP_ACTIGRAPHY_MONITOR

/* ============================================================================
 * Enumerated Type Definitions
 * ============================================================================
*/

typedef enum app_opstatus_t
{
    APP_INIT_COMPLETE,
    APP_INIT_INCOMPLETE,
    APP_DATA_RECEIVED,
    APP_WAITING_FOR_DATA,
    APP_DATA_SENT,
    APP_SENDING,
    APP_ERROR
} app_opstatus_t;

/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Begins and queues all initialization operations. */
void app_actigraphyMonitor_init();


/* Sends a request to the sensor for data. */
void app_actigraphyMonitor_requestData();


/* Begins sending data packets to CAN controller. */
void app_actigraphyMonitor_sendData();


/* Checks if initialization routine is completed.
 *
 * Returns:
 *  - The current status of the operation.
 */
app_opstatus_t app_actigraphyMonitor_getInitStatus();


/* Checks if sensor data has been received. 
 *
 * Returns:
 *  - The current status of the operation.
 */
app_opstatus_t app_actigraphyMonitor_dataReceived();


/* Checks if entire data packet has finished uploading.
 *
 * Returns:
 *  - The current status of the operation.
 */
app_opstatus_t app_actigraphyMonitor_dataUploaded();

#endif /* APP_ACTIGRAPHY_MONITOR */

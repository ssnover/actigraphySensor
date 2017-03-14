/* 
 * File:    app_stateMachine.h
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Contains state machine logic to run the system.
 */

#ifndef APP_STATEMACHINE
#define APP_STATEMACHINE

#include "app_actigraphyMonitor.h"

/* ============================================================================
 * Public Enumerated Types
 * ============================================================================
*/

typedef enum state_t
{
    STATE_ERROR,
    STATE_INIT,
    STATE_START_TIMER,
    STATE_DATA_ACK,
    STATE_DATA_UPLOAD,
    STATE_IDLE
} state_t;


/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Runs the logic for changing between states. */
void app_stateMachine();


/* Gets the current state of the state machine.
 *
 * Returns:
 *  - An enumerated state object representing the state of the application.
*/
state_t app_stateMachine_getState();

#endif /* APP_STATEMACHINE */
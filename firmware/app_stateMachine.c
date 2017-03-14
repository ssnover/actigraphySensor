/*
 * File:    app_stateMachine.c
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Contains functions for managing the state machine logic.
 */

#include <stdbool.h>
#include "app_stateMachine.h"
#include "app_actigraphyMonitor.h"

/* ============================================================================
 * Private Module Variables
 * ============================================================================
 */

bool state_changed = false;
state_t current_state = STATE_INIT;
state_t next_state = STATE_INIT;


/* ============================================================================
 * Private Function Declarations
 * ============================================================================
 */

/* Handles the introduction and one time asynchronous tasks for a state. 
 *
 * Arguments:
 *  - state: The state to enter and execute init tasks for.
 */
void enterState(state_t state);


/* Handles the main process for a task.
 *
 * Arguments:
 *  - state: The state to be run.
 */
void processState(state_t state);


/* Cleans up and sets flags for finishing up a task.
 *
 * Arguments:
 *  - state: The state to be cleaned up.
 */
void exitState(state_t state);


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
 */

/* Runs the logic for changing between states. */
void app_stateMachine()
{
    if (state_changed)
    {
        state_changed = false;
        current_state = next_state;
        enterState(current_state);
    }

    processState(current_state);

    if (current_state != next_state)
    {
        exitState(current_state);
        state_changed = true;
    }
}


/* Handles the introduction and one time asynchronous tasks for a state. */
void enterState(state_t state)
{
    switch (state)
    {
        case STATE_INIT:
        {
            break;
        }
        case STATE_DATA_ACK:
        {
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            break;
        }
        case STATE_IDLE:
        {
            break;
        }
        case STATE_ERROR:
        {
            break;
        }
        default:
        {
            next_state = STATE_ERROR;
            break;
        }
    }

    return;
}


/* Handles the main process for a task. */
void processState(state_t state)
{
    switch (state)
    {
        case STATE_INIT:
        {
            break;
        }
        case STATE_DATA_ACK:
        {
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            break;
        }
        case STATE_IDLE:
        {
            break;
        }
        case STATE_ERROR:
        {
            break;
        }
        default:
        {
            next_state = STATE_ERROR;
            break;
        }
    }

    return;
}


/* Cleans up and sets flags for finishing up a task. */
void exitState(state_t state)
{
    switch (state)
    {
        case STATE_INIT:
        {
            break;
        }
        case STATE_DATA_ACK:
        {
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            break;
        }
        case STATE_IDLE:
        {
            break;
        }
        case STATE_ERROR:
        {
            break;
        }
        default:
        {
            next_state = STATE_ERROR;
            break;
        }
    }

    return;
}


/* Gets the current state of the state machine. */
state_t app_stateMachine_getState()
{
    return current_state;
}

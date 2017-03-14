/*
 * File:    app_stateMachine.c
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Contains functions for managing the state machine logic.
 */

#include "app_stateMachine.h"
#include "app_actigraphyMonitor.h"

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

}


/* Handles the introduction and one time asynchronous tasks for a state. */
void enterState(state_t state)
{

}


/* Handles the main process for a task. */
void processState(state_t state)
{

}


/* Cleans up and sets flags for finishing up a task. */
void exitState(state_t state)
{

}


/* Gets the current state of the state machine. */
state_t app_stateMachine_getState()
{

}

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
const uint16_t timer_wakeup_value = 1565;
volatile bool wakeup_flag = false;


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


/* Sets a timer to coordinate the periodic wake up from idle. */
void setTimer();


/* ============================================================================
 * Interrupt Service Routines
 * ============================================================================
 */

/* Sounds the alarm when it's time to wake up. */
ISR(TIMER1_COMPA_vect)
{
    wakeup_flag = true;
}


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
            /* Initialize hardware peripherals and slave devices. */
            app_actigraphyMonitor_init();
            break;
        }
        case STATE_START_TIMER:
        {
            /* Sets the timer for periodic wake up. */
            setTimer();
            break;
        }
        case STATE_DATA_ACK:
        {
            /* Request data from sensor. */
            app_actigraphyMonitor_requestData();
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            /* Send the data packet to the transceiver. */
            app_actigraphyMonitor_sendData();
            break;
        }
        case STATE_IDLE:
        {
            /* Nothing to set up. */
            break;
        }
        case STATE_ERROR:
        {
            /* Currently no action in Error State. */
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
            app_opstatus_t status = app_actigraphyMonitor_getInitStatus();

            if (status == APP_INIT_COMPLETE)
            {
                /* Initialization completed successfully. */
                next_state = STATE_START_TIMER;
            }
            else if (status == APP_INIT_INCOMPLETE)
            {
                /* Initialization in progress. */
            }
            else
            {
                /* Error has occurred during initialization. */
                next_state = STATE_ERROR;
            }
            break;
        }
        case STATE_START_TIMER:
        {
            /* State actions are completely asynchronous. */
            next_state = STATE_DATA_ACK;
            break;
        }
        case STATE_DATA_ACK:
        {
            app_opstatus_t status = app_actigraphyMonitor_dataReceived();

            if (status == APP_DATA_RECEIVED)
            {
                /* Data was received. */
                next_state = STATE_DATA_UPLOAD;
            }
            else if (status == APP_WAITING_FOR_DATA)
            {
                /* Waiting for sensor to send data. */
            }
            else
            {
                /* Error has occurred with reading sensor. */
                next_state = STATE_ERROR;
            }
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            app_opstatus_t status = app_actigraphyMonitor_dataUploaded();
            
            if (status == APP_DATA_SENT)
            {
                /* Complete packet has been sent. */
                next_state = STATE_IDLE;
            }
            else if (status == APP_SENDING)
            {
                /* Data upload in process. */
            }
            else
            {
                /* The request has timed out or an error has occurred. */
                next_state = STATE_ERROR;
            }
            break;
        }
        case STATE_IDLE:
        {
            if (wakeup_flag)
            {
                /* Alarm is blaring, time to wake up. */
                next_state = STATE_START_TIMER;
            }
            break;
        }
        case STATE_ERROR:
        {
            /* Currently no action in Error State. */
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
            /* Nothing to clean up after an initialization. */
            break;
        }
        case STATE_START_TIMER:
        {
            /* Nothing to clean up after setting timer. */
            break;
        }
        case STATE_DATA_ACK:
        {
            /* Nothing to clean up after data acquisition. */
            break;
        }
        case STATE_DATA_UPLOAD:
        {
            /* Remove sent data from queue. */
            app_actigraphyMonitor_clearUploadQueue();
            break;
        }
        case STATE_IDLE:
        {
            /* Leave sleep mode. */
            wakeUp();
            break;
        }
        case STATE_ERROR:
        {
            /* Currently no action in Error State. */
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


/* Sets a timer to coordinate the periodic wake up from idle. */
void setTimer()
{
    /* Assuming a 16 MHz clock source, the prescaler 1/1024 provides a clock
     * tick every 1/16th milliseconds. */
    TCCR1B |= (1 << CS12) | (1 << CS10);

    /* Generate a number of ticks to set compare for 100 ms. */
    OCR1AH = timer_wakeup_value >> 8;
    OCR1AL = timer_wakeup_value & 0xff;

    /* Reset timer. */
    TCNT1 = 0;

    /* Enable interrupt. */
    TIMSK1 |= (1 << OCIE1A);

    return;
}


/* Resets changed clocks for sleep mode and disables timer interrupt. */
void wakeUp()
{
    /* Snooze the alarm. */
    wakeup_flag = false;
    /* Disable further interrupts. */
    TIMSK1 &= ~(1 << OCIE1A);
}

/*
 * File: communications.h
 * Author: Shane Snover
*/

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

/* ============================================================================
 * Public Enumerated Type Definitions
 * ============================================================================
*/

typedef enum comms_state_E
{
    COMMS_STATE_INIT        = 0,
    COMMS_STATE_IDLE        = 1,
    COMMS_STATE_RECEIVE     = 2,
    COMMS_STATE_TRANSMIT    = 3,
    COMMS_STATE_ERROR       = 4,
    /* NUM_STATES must be last. */
    COMMS_NUM_STATES,
} comms_state_E;


/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Initializes the state machine for the communications app. */
void communications_init(void);


/* Runs the communications app state machine.
 *
 * Returns:
 *  The state of the communications state machine.
*/
comms_state_E communications_run(void);

#endif /* COMMUNICATIONS_H */
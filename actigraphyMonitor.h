/*
 * File: actigraphyMonitor.h
 * Author: Shane Snover
 */

#ifndef ACTIGRAPHY_MONITOR_H
#define ACTIGRAPHY_MONITOR_H

/* ============================================================================
 * Public Enumerated Type Definitions
 * ============================================================================
*/

typedef enum actigraphyMonitor_state_E
{
    ACTIGRAPHY_STATE_INIT    = 0,
    ACTIGRAPHY_STATE_IDLE    = 1,
    ACTIGRAPHY_STATE_MEASURE = 2,
    ACTIGRAPHY_STATE_ERROR   = 3,
    /* NUM_STATES should always be last. */
    ACTIGRAPHY_NUM_STATES
} actigraphyMonitor_state_E;


/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Initializes the actigraphy data acquisition application. */
void actigraphyMonitor_init(void);


/* Runs the actigraphy application state machine for a cycle and captures it's
 * upcoming state.
 *
 * Returns:
 *  The upcoming state of the actigraphy monitor state machine.
*/
actigraphyMonitor_state_E actigraphyMonitor_run(void);

#endif /* ACTIGRAPHY_MONITOR_H */

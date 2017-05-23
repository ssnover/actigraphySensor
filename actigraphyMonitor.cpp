/*
 * File: actigraphyMonitor.cpp
 * Author: Shane Snover
 */

#include <stdbool.h>
#include "actigraphyMonitor.h"
#include "Adafruit_LIS3DH.h"

namespace /* Unnamed namespace */
{

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/
void enterState(void);
void processState(void);
void exitState(void);


/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/
actigraphyMonitor_state_E current_state, next_state;
bool state_changed;
Adafruit_LIS3DH sensor;
bool sensor_initialized;
const uint8_t SENSOR_CHIP_SELECT_PIN = 10;


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void actigraphyMonitor_init(void)
{
    /* initialize state machine */
    current_state = ACTIGRAPHY_STATE_INIT;
    state_changed = true;

    return;
}


actigraphyMonitor_state_E actigraphyMonitor_run(void)
{
    if (state_changed)
    {
        enterState();
        state_changed = false;
    }

    processState();

    if (current_state != next_state)
    {
        exitState();
        state_changed = true;
        current_state = next_state;
    }

    return current_state;
}


void enterState(void)
{
    /* Async startup */
    switch (current_state)
    {
        case ACTIGRAPHY_STATE_INIT:
        {
            sensor = Adafruit_LIS3DH(SENSOR_CHIP_SELECT_PIN);
            sensor_initialized = sensor.begin();
            break;
        }
        case ACTIGRAPHY_STATE_IDLE:
        {
            /* Nothing to do here. */
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


void processState(void)
{
    switch (current_state)
    {
        case ACTIGRAPHY_STATE_INIT:
        {
            if (sensor_initialized)
            {
                next_state = ACTIGRAPHY_STATE_IDLE;
            }
            else
            {
                next_state = ACTIGRAPHY_STATE_ERROR;
            }
            break;
        }
        case ACTIGRAPHY_STATE_IDLE:
        {
            /* Nothing to do here. */
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


void exitState(void)
{
    /* Cleanup */
    switch (current_state)
    {
        case ACTIGRAPHY_STATE_INIT:
        {
            /* Nothing to do here. */
            break;
        }
        case ACTIGRAPHY_STATE_IDLE:
        {
            /* Nothing to do here. */
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}

} /* Unnamed namespace */

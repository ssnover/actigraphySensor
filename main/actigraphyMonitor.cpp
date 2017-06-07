/*
 * File: actigraphyMonitor.cpp
 * Author: Shane Snover
 */

#include "Adafruit_LIS3DH.h"
#include "actigraphyMonitor.h"

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/
static void enterState(void);
static void processState(void);
static void exitState(void);

/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/

static actigraphyMonitor_state_E current_state, next_state;
static bool state_changed;
Adafruit_LIS3DH sensor;
bool sensor_initialized;
const uint8_t SENSOR_CHIP_SELECT_PIN = 10;
actigraphyMonitor_data_S sensor_data;
bool data_ready;
bool sample_requested;
bool application_error;

/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void actigraphyMonitor_init(void)
{
    /* initialize state machine */
    current_state = ACTIGRAPHY_STATE_INIT;
    state_changed = true;
    application_error = false;
    data_ready = false;
    sample_requested = false;

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

void actigraphyMonitor_sampleAlert(void)
{
    if (sample_requested)
    {
        /* Overrun of sample. Go to error state. */
        application_error = true;
    }
    else
    {
        sample_requested = true;
    }

    return;
}

bool actigraphyMonitor_dataReady(void)
{
    return data_ready;
}

void actigraphyMonitor_getData(actigraphyMonitor_data_S * exportData)
{
    if (data_ready)
    {
        exportData->acceleration_x = sensor_data.acceleration_x;
        exportData->acceleration_y = sensor_data.acceleration_y;
        exportData->acceleration_z = sensor_data.acceleration_z;
        data_ready = false;
    }
    else
    {
        application_error = true;
    }
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
            break;
        }
        default:
        {
            application_error = true;
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
                sensor.setDataRate(LIS3DH_DATARATE_25_HZ);
                sensor.setRange(LIS3DH_RANGE_2_G);
                next_state = ACTIGRAPHY_STATE_MEASURE;
            }
            else
            {
                application_error = true;
            }
            break;
        }
        case ACTIGRAPHY_STATE_IDLE:
        {
            if (sample_requested)
            {
                next_state = ACTIGRAPHY_STATE_MEASURE;
            }
            break;
        }
        case ACTIGRAPHY_STATE_MEASURE:
        {
            sample_requested = false;
            sensor_data.acceleration_x = sensor.x;
            sensor_data.acceleration_y = sensor.y;
            sensor_data.acceleration_z = sensor.z;
            data_ready = true;
            next_state = ACTIGRAPHY_STATE_MEASURE;
        }
        default:
        {
            application_error = true;
            break;
        }
    }

    if (application_error)
    {
        next_state = ACTIGRAPHY_STATE_ERROR;
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
        case ACTIGRAPHY_STATE_MEASURE:
        {
            break;
        }
        default:
        {
            application_error = true;
            break;
        }
    }
    return;
}

/*
 * File: communications.cpp
 * Author: Shane Snover
*/

#include "MCP_CAN.h"
#include "actigraphyMonitor.h"
#include "communications.h"

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/

static void enterState(void);
static void processState(void);
static void exitState(void);
static void packetizeData(actigraphyMonitor_data_S * data);

/* ============================================================================
 * Private Module Variables
 * ============================================================================
*/

static bool state_changed;
static comms_state_E current_state, next_state;
MCP_CAN CANBus(10);
uint8_t bus_connection_status;
uint8_t attempts;
actigraphyMonitor_data_S raw_data;
uint8_t * packaged_data;
const uint8_t PACKET_SIZE = 6;
const uint16_t DATA_PACKET_ID = 0x100;
const uint8_t MAX_TRANSMIT_ATTEMPTS = 3;

/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void communications_init(void)
{
    state_changed = true;
    current_state = COMMS_STATE_INIT;
    return;
}

comms_state_E communications_run(void)
{
    if (state_changed)
    {
        enterState();
        state_changed = false;
    }

    processState();

    if (state_changed)
    {
        exitState();
    }

    return current_state;
}

void enterState(void)
{
    switch (current_state)
    {
        case COMMS_STATE_INIT:
        {
            bus_connection_status = CANBus.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ);
            break;
        }
        case COMMS_STATE_IDLE:
        {
            break;
        }
        case COMMS_STATE_RECEIVE:
        {
            break;
        }
        case COMMS_STATE_TRANSMIT:
        {
            attempts = 0;
            actigraphyMonitor_getData(&raw_data);
            packetizeData(&raw_data);
            break;
        }
        case COMMS_STATE_ERROR:
        /* Intentional fall through. */
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
        case COMMS_STATE_INIT:
        {
            if (bus_connection_status == CAN_OK)
            {
                CANBus.setMode(MCP_NORMAL);
                next_state = COMMS_STATE_IDLE;
            }
            else
            {
                next_state = COMMS_STATE_ERROR;
            }
            break;
        }
        case COMMS_STATE_IDLE:
        {
            bool data_status = actigraphyMonitor_dataReady();
            if (data_status)
            {
                next_state = COMMS_STATE_TRANSMIT;
            }
            break;
        }
        case COMMS_STATE_RECEIVE:
        {
            break;
        }
        case COMMS_STATE_TRANSMIT:
        {
            uint8_t transmit_status = CANBus.sendMsgBuf(DATA_PACKET_ID, 0, PACKET_SIZE, packaged_data);
            if (transmit_status == CAN_OK)
            {
                next_state = COMMS_STATE_IDLE;
            }
            else
            {
                if (attempts >= MAX_TRANSMIT_ATTEMPTS)
                {
                    next_state = COMMS_STATE_ERROR;
                }
                attempts++;
            }
            break;
        }
        case COMMS_STATE_ERROR:
        /* Intentional fall through. */
        default:
        {
            break;
        }
    }

    if (current_state != next_state)
    {
        state_changed = true;
    }

    return;
}

void exitState(void)
{
    switch (current_state)
    {
        case COMMS_STATE_INIT:
        {
            break;
        }
        case COMMS_STATE_IDLE:
        {
            break;
        }
        case COMMS_STATE_RECEIVE:
        {
            break;
        }
        case COMMS_STATE_TRANSMIT:
        {
            break;
        }
        case COMMS_STATE_ERROR:
        /* Intentional fall through. */
        default:
        {
            break;
        }
    }

    current_state = next_state;

    return;
}

void packetizeData(actigraphyMonitor_data_S * data)
{
    *(packaged_data + 0) = (data->acceleration_x >> 8);
    *(packaged_data + 1) = (data->acceleration_x & 0xff);
    *(packaged_data + 2) = (data->acceleration_y >> 8);
    *(packaged_data + 3) = (data->acceleration_y & 0xff);
    *(packaged_data + 4) = (data->acceleration_z >> 8);
    *(packaged_data + 5) = (data->acceleration_z & 0xff);

    return;
}

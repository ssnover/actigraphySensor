/*
 * File: main.ino
 * Author: Shane Snover
 */

#include <avr/interrupt.h>
#include "timer.h"
#include "actigraphyMonitor.h"
#include "communications.h"

#define NOP()           __asm__("nop\n\t")
#define FREQ_MAIN       (1000)
#define FREQ_SAMPLE     (10)
#define SAMPLE_INDEX    ((FREQ_MAIN/FREQ_SAMPLE) - 1)

/* ============================================================================
 * Global Variables
 * ============================================================================
*/
volatile bool wakeup;
uint8_t loop_index;


/* ============================================================================
 * Interrupt Service Routines
 * ============================================================================
*/

ISR(TIMER1_OVF_vect)
{
    wakeup = true;
    timer_reset();
}


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void setup(void)
{
    /* Initialize the application timing. */
    timer_init();
    /* Initialize the applications. */
    actigraphyMonitor_init();
    communications_init();
    /* Enable interrupts. */
    sei();
    return;
}


void loop(void)
{
    if (loop_index >= (int) SAMPLE_INDEX)
    {
        loop_index = 0;
        actigraphyMonitor_run();
    }
    else
    {
        loop_index++;
    }
    
    communications_run();

    wakeup = false;

    while (wakeup == false)
    {
        NOP();
    }

    return;
}

/*
 * File: main.ino
 * Author: Shane Snover
 */

#include <avr/interrupt.h>
#include "timer.h"
#include "actigraphyMonitor.h"
#include "communications.h"

#define NOP()      __asm__("nop\n\t")

/* ============================================================================
 * Global Variables
 * ============================================================================
*/
volatile bool wakeup;


/* ============================================================================
 * Interrupt Service Routines
 * ============================================================================
*/

ISR(TIMER1_OVF_vect)
{
    wakeup = true;
    /* TODO: Check if the timer needs any variables reset. */
}


/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void setup(void)
{
    timer_init();
    actigraphyMonitor_init();
    communications_init()
    return;
}


void loop(void)
{
    actigraphyMonitor_run();
    communications_run();

    wakeup = false;

    while (wakeup == false)
    {
        NOP();
    }

    return;
}

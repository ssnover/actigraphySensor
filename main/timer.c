/*
 * File: timer.c
 * Author: Shane Snover
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include "timer.h"


#define TIMER_RELOAD_H      (0xC1)
#define TIMER_RELOAD_L      (0x80)

/* ============================================================================
 * Private Function Declarations
 * ============================================================================
*/

/* ============================================================================
 * Module Function Definitions
 * ============================================================================
*/

void timer_init(void)
{
    /* See the readme for calculations of timer reload values. */

    /* Set the prescaler for a division of 1. */
    TCCR1B &= ~(1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= (1 << CS10);
    /* Sets up the timer peripheral to clock the mainloop at 1 kHz. */
    timer_reset();

    /* Set the timer overflow interrupt. */
    TIMSK1 |= (1 << TOIE1);
    
    return;
}


void timer_reset(void)
{
    /* The high byte must be written before the low byte. */
    TCNT1H = TIMER_RELOAD_H;
    TCNT1L = TIMER_RELOAD_L;
    return;
}

#ifdef __cplusplus
}
#endif

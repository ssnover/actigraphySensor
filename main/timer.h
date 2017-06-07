/*
 * File: timer.h
 * Author: Shane Snover
 */

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Public Function Declarations
 * ============================================================================
*/

/* Initializes and sets the overflow value for the timer driving the
 * applications' run frequency. */
void timer_init(void);

/* Resets the timer variable to a reload value and let's it starting counting
 * again. */
void timer_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */

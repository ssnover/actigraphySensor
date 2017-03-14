/* 
 * File:    main.c
 * Author:  Shane Snover <ssnover95@gmail.com>
 *
 * Beginning of Actigraphy Monitor program execution. Contains state machine
 * logic for flow control.
 */

#include "config.h"
#include "app_stateMachine.h"

int main(void)
{
    while (1)
    {
        app_stateMachine();
    }
}

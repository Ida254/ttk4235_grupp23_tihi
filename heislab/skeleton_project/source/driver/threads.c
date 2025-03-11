/**
 * @file
 * @brief Implementation of elevator control threads.
 *
 * This file defines thread functions that continuously monitor and process button presses,
 * track the elevator's floor position, and detect emergency stop conditions.
 */

#include "threads.h"

void *button_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    if (!elevator)
    {
        // printf("Error: Elevator is NULL in emergency_listener\n");
        return NULL;
    }

    while (1)
    {
        on_button_press(elevator);
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *floor_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    if (!elevator)
    {
        // printf("Error: Elevator is NULL in emergency_listener\n");
        return NULL;
    }

    // while (!elevator->initialized)
    // {
    //     nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL); // Sleep for 100ms
    // }

    while (1)
    {
        at_right_floor(elevator);
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *emergency_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;

    if (!elevator)
    {
        // printf("Error: Elevator is NULL in emergency_listener\n");
        return NULL;
    }

    while (1)
    {
        check_emergency_stop(elevator);
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}
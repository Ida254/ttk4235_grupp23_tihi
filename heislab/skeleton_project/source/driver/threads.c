#include "threads.h"

void *button_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    while (1)
    {
        on_button_press(elevator);                                // Check if a button is pressed and update queue
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *floor_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    while (1)
    {
        at_right_floor(elevator);                                 // Check if the elevator should stop at a floor
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
        check_emergency_stop(elevator);                           // Constantly monitor if the emergency stop is triggered
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}
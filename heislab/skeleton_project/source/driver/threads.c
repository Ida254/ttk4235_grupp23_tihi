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
        pthread_mutex_lock(&elevator_mtx);
        if (elevator->initialized)
        {
            on_button_press(elevator);
        }
        pthread_mutex_unlock(&elevator_mtx);

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

    while (1)
    {
        pthread_mutex_lock(&elevator_mtx);
        if (!elevator->initialized && at_right_floor(elevator))
        {
            elevator->initialized = true;
            printf("Initialized ");   // db
            print_elevator(elevator); // db
        }
        // at_right_floor(elevator);
        if (at_right_floor(elevator) && elevator->in_motion)
        {

            stop_elevator_at_floor(elevator, elevator->current_floor);
            elevio_doorOpenLamp(0);
        }
        pthread_mutex_unlock(&elevator_mtx);

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
        pthread_mutex_lock(&elevator_mtx);
        // check_emergency_stop(elevator);
        if (is_emergency_stop(elevator))
        {
            elevio_motorDirection(DIRN_STOP);
            elevator->in_motion = false;
            elevio_stopLamp(1);
            print_elevator(elevator);
            kill(getpid(), SIGKILL); // Forcefully stops the program
        }
        pthread_mutex_unlock(&elevator_mtx);

        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}
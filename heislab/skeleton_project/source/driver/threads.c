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

        // if (button_pressed())
        // {
        //     printf("btn pressed: ");
        //     print_request(*button_pressed());
        // }
        if (button_pressed())
        {
            if (!elevator->initialized)
            {
                initialize_elevator(elevator);
            }

            if (elevator->is_stopped)
            {
                if (elevator->current_floor != -1)
                {
                    rest_elevator(elevator);
                }

                elevator->is_stopped = false;
                elevio_stopLamp(0);
            }
        }
        pthread_mutex_unlock(&elevator_mtx);

        nanosleep(&(struct timespec){0, SLEEP_TIME_NS}, NULL); // Sleep for 10ms
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

    // MotorDirection dirn = get_new_motor_direction(elevator);

    while (1)
    {
        pthread_mutex_lock(&elevator_mtx);
        if (!elevator->is_stopped)
        {
            elevio_doorOpenLamp(0);
        }

        if (elevator->is_stopped)
        {
            pthread_mutex_unlock(&elevator_mtx);
            continue;
        }

        elevator->current_floor = elevio_floorSensor();
        if (elevator->current_floor != -1)
        {
            elevator->last_floor = elevator->current_floor;
            elevio_floorIndicator(elevator->last_floor);
            // print_elevator(elevator);
        }

        if (elevator->motor_state != get_new_motor_direction(elevator))
        {
            elevator->motor_state = get_new_motor_direction(elevator);
            elevio_motorDirection(elevator->motor_state);
        }

        if (elevator->motor_state != DIRN_STOP)
        {
            elevator->in_motion = true;
        }

        if (elevator->current_floor == TOP_FLOOR)
        {
            elevator->moving_direction = DIRN_DOWN;
        }
        else if (elevator->current_floor == BOTTOM_FLOOR)
        {
            elevator->moving_direction = DIRN_UP;
        }
        else if (elevator->motor_state != DIRN_STOP)
        {
            elevator->moving_direction = elevator->motor_state;
        }

        if (at_right_floor(elevator))
        {
            if (!elevator->initialized)
            {
                elevator->initialized = true;
                printf("Initialized ");   // db
                print_elevator(elevator); // db
            }

            if (elevator->queue_size > 0)
            {
                elevio_buttonLamp(elevator->current_floor, elevator->request_queue[0].button, 0);
                remove_request_from_queue(elevator, elevator->current_floor);
            }

            elevio_doorOpenLamp(1);

            printf("Destination reached and elevator stopped \n"); // db
            print_elevator(elevator);                              // db

            rest_elevator(elevator);

            // elevio_doorOpenLamp(0);
        }

        // if (!elevator->initialized && at_right_floor(elevator))
        // {
        //     elevator->initialized = true;
        //     printf("Initialized ");   // db
        //     print_elevator(elevator); // db
        // }

        // if (elevator->queue_size == 0)
        // {
        //     return NULL;
        // }
        // at_right_floor(elevator);
        // if (at_right_floor(elevator))
        // {
        //     if (elevator->in_motion)
        //     {
        //         elevio_motorDirection(DIRN_STOP);
        //         elevator->in_motion = false;
        //     }

        //     elevio_buttonLamp(elevator->current_floor, elevator->request_queue[0].button, 0);

        //     switch_direction(elevator);

        //     remove_request_from_queue(elevator, elevator->current_floor);
        //     elevio_doorOpenLamp(1);

        //     printf("Destination reached and elevator stopped \n"); // db
        //     print_elevator(elevator);                              // db

        //     rest_elevator(elevator);

        //     elevio_doorOpenLamp(0);
        // }
        pthread_mutex_unlock(&elevator_mtx);

        nanosleep(&(struct timespec){0, SLEEP_TIME_NS}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *emergency_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;

    if (!elevator)
    {
        // printf("Error: Elevator is NULL in emergency_listener\n"); // db
        return NULL;
    }

    while (1)
    {
        pthread_mutex_lock(&elevator_mtx);
        // check_emergency_stop(elevator);
        if (!elevator->is_stopped && elevator->initialized)
        {
            elevator->is_stopped = elevio_stopButton();
            // elevator->is_stopped = is_emergency_stop(elevator);
            if (elevator->is_stopped)
            {
                empty_queue(elevator);

                elevio_motorDirection(DIRN_STOP);
                elevator->in_motion = false;
                elevator->motor_state = DIRN_STOP;
                elevio_stopLamp(1);

                if (elevator->current_floor != -1)
                {
                    elevio_doorOpenLamp(1);
                }

                printf("Stopped ");       // db
                print_elevator(elevator); // db

                // kill(getpid(), SIGKILL); // Forcefully stops the program
            }
        }

        pthread_mutex_unlock(&elevator_mtx);

        nanosleep(&(struct timespec){0, SLEEP_TIME_NS}, NULL); // Sleep for 10ms
    }
    return NULL;
}
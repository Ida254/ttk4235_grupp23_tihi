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
            elevator->is_stopped = false;
            elevio_stopLamp(0);
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

    MotorDirection newDir = switch_direction(elevator);

    while (1)
    {
        pthread_mutex_lock(&elevator_mtx);
        if (!elevator->initialized && at_right_floor(elevator))
        {
            elevator->initialized = true;
            printf("Initialized ");   // db
            print_elevator(elevator); // db
        }

        if (at_right_floor(elevator))
        {
            if (elevator->in_motion)
            {
                elevio_motorDirection(DIRN_STOP);
                elevator->in_motion = false;
                elevator->motor_state = DIRN_STOP;
            }

            newDir = switch_direction(elevator);
            
            elevio_buttonLamp(elevator->current_floor, elevator->request_queue[0].button, 0);
            
            remove_request_from_queue(elevator, elevator->current_floor);
            elevio_doorOpenLamp(1);
            
            printf("Destination reached and elevator stopped \n"); // db
            print_elevator(elevator);                              // db
            
            rest_elevator(elevator);
            
            elevio_doorOpenLamp(0);
        }
        
        if (elevator->queue_size != 0)
        {
            printf("1 new dir: %s \n", motor_direction_to_string(newDir));
            newDir = switch_direction(elevator);
            printf("2 new dir: %s \n", motor_direction_to_string(newDir));
        }

        if (elevator->motor_state != newDir)
        {
            elevator->motor_state = newDir;
            if (newDir != DIRN_STOP)
            {
                elevator->moving_direction = newDir;
                elevator->in_motion = true;
            }
        }

        if (newDir == DIRN_STOP) // db
        {
            printf("killed new dir: %s \n", motor_direction_to_string(newDir));
            print_elevator(elevator);
            kill(getpid(), SIGKILL);
        }
        elevio_motorDirection(newDir);

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
        if (!elevator->is_stopped)
        {
            elevator->is_stopped = is_emergency_stop(elevator);
            if (elevator->is_stopped)
            {
                empty_queue(elevator);

                elevio_motorDirection(DIRN_STOP);
                elevator->in_motion = false;
                elevator->motor_state = DIRN_STOP;

                elevio_stopLamp(1);

                // kill(getpid(), SIGKILL); // Forcefully stops the program

                printf("Stopped ");       // db
                print_elevator(elevator); // db
            }
        }

        pthread_mutex_unlock(&elevator_mtx);

        nanosleep(&(struct timespec){0, SLEEP_TIME_NS}, NULL); // Sleep for 10ms
    }
    return NULL;
}
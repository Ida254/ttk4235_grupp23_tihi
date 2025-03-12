/**
 * @file
 * @brief Implementation of the elevator control system.
 *
 * This file contains function definitions for managing elevator movement,
 * handling button presses, processing requests, and controlling the system state.
 */

#include "elevator.h"

void initialize_elevator(Elevator *elevator)
{
    if (elevator == NULL)
    {
        printf("Elevator pointer is NULL!\n");
        exit(1);
    }

    elevator->current_floor = elevio_floorSensor();
    elevator->last_floor = INITIAL_FLOOR;
    elevator->moving_direction = INITIAL_DIRECTION;
    elevator->motor_state = DIRN_STOP;
    elevator->last_motor_state = DIRN_STOP;
    elevator->in_motion = false;
    elevator->request_queue = NULL;
    elevator->queue_size = 0;
    elevator->queue_capacity = 0;
    elevator->initialized = false;
    elevator->is_stopped = false;

    pthread_mutex_init(&elevator_mtx, NULL);

    turn_off_all_lamps();

    // move to BOTTOM_FLOOR
    add_request_to_queue(elevator, INITIAL_REQUEST);
}

void free_elevator(Elevator *elevator)
{
    free(elevator->request_queue);
    elevator->request_queue = NULL;
}

void turn_off_all_lamps()
{
    elevio_doorOpenLamp(0);
    elevio_stopLamp(0);

    for (int floor = BOTTOM_FLOOR; floor <= TOP_FLOOR; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            elevio_buttonLamp(floor, btn, 0);
        }
    }
}

void sort_queue(Elevator *elevator)
{
    if (!elevator)
    {
        printf("Error: Elevator pointer is NULL!\n");
        return;
    }

    size_t queueSize = elevator->queue_size;

    if (queueSize < 2)
    {
        return;
    }

    int lastFloor = elevator->last_floor;
    MotorDirection currMovingDir = elevator->moving_direction;
    sort_requests(elevator->request_queue, queueSize, lastFloor, currMovingDir, elevator->in_motion);
}

void add_request_to_queue(Elevator *elevator, Request new_req)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    if (in_array(elevator->request_queue, *queueSize, new_req))
    {
        // printf("New req already in the queue \n   ");        // db
        // print_request(new_req);                              // db
        // print_requests(elevator->request_queue, *queueSize); // db
        return;
    }
    add_request(&elevator->request_queue, queueSize, queueCapacity, new_req);
    // print_elevator(elevator);

    sort_queue(elevator);

    // printf("\nAdded to queue and sorted\n"); // db
    // print_elevator(elevator);                // db
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    remove_request_by_floor(&elevator->request_queue, queueSize, queueCapacity, floor);
}

void empty_queue(Elevator *elevator)
{
    while (elevator->queue_size != 0)
    {
        remove_request_from_queue(elevator, elevator->request_queue[0].floor);
    }
}

void on_button_press(Elevator *elevator)
{
    Request *new_req = button_pressed();

    if (new_req)
    {
        elevio_buttonLamp(new_req->floor, new_req->button, 1);

        add_request_to_queue(elevator, *new_req);
    }
    free(new_req);
}

Request *button_pressed() // db, rather return the button and if none detected, return NULL
{
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            if (elevio_callButton(floor, btn))
            {
                Request *req = malloc(sizeof(Request));
                if (req == NULL)
                {
                    printf("Memory allocation failed!\n");
                    return NULL;
                }
                req->floor = floor;
                req->button = btn;
                return req;
            }
        }
    }
    return NULL;
}

bool at_right_floor(Elevator *elevator)
{
    if (elevator->queue_size == 0)
    {
        return false;
    }

    if (elevator->current_floor == elevator->request_queue[0].floor) // db, currFloor could just be elevio_floorSensor()
    {
        // stop_elevator_at_floor(elevator, currFloor);
        // elevio_doorOpenLamp(0);
        // printf("At right floor \n"); // db
        return true;
    }
    return false;
}

MotorDirection get_new_motor_direction(Elevator *elevator) // db, returns direction, decides movement
{
    MotorDirection newDir = DIRN_STOP;

    if (elevator->is_stopped || elevator->queue_size == 0 || elevator->current_floor == elevator->request_queue[0].floor)
    {
        return newDir;
    }

    int destinationFloor = elevator->request_queue[0].floor;

    // If there are requests, determine direction based on the first request
    int dirInInt = destinationFloor - elevator->last_floor;
    if (dirInInt == 0 && elevator->current_floor == -1)
    {
        if (elevator->in_motion == false)
        {
            newDir = elevator->moving_direction == DIRN_DOWN ? DIRN_UP : DIRN_DOWN;
            return newDir;
        }
        return elevator->motor_state;
    }

    newDir = (dirInInt > 0) ? DIRN_UP : (dirInInt < 0) ? DIRN_DOWN
                                                       : DIRN_STOP;
    return newDir;
}

void rest_elevator(Elevator *elevator)
{
    bool isObstruction = false;
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3 || elevio_obstruction())
    {
        on_button_press(elevator);
        if (elevio_stopButton())
        {
            return;
        }
        isObstruction |= elevio_obstruction(); // If true once, it stays true
    }
    if (isObstruction)
    {
        rest_elevator(elevator);
    }
}

void print_elevator(Elevator *elevator)
{
    printf("Elevator \n");
    printf("Current floor: %d\n", elevator->current_floor);
    printf("Last floor: %d\n", elevator->last_floor);
    printf("Current moving direction: %s\n", motor_direction_to_string(elevator->moving_direction));
    printf("Current motor state: %s\n", motor_direction_to_string(elevator->motor_state));
    printf("In motion: %s\n", bool_to_string(elevator->in_motion));

    if (elevator->queue_size > 0)
    {
        print_requests(elevator->request_queue, elevator->queue_size);
    }
    else
    {
        printf("No requests in the queue.\n");
    }

    printf("Queue size: %zu\n", elevator->queue_size);
    printf("Queue capacity: %zu\n", elevator->queue_capacity);
    printf("Initialized: %s\n", bool_to_string(elevator->initialized));
    printf("Is emergency stopped: %s\n\n", bool_to_string(elevator->is_stopped));
}
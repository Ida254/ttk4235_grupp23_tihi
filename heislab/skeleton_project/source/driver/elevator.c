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
    elevator->in_motion = false;
    elevator->request_queue = NULL;
    elevator->queue_size = 0;
    elevator->queue_capacity = 0;
    // elevator->initialized = false;

    turn_off_all_lamps();

    // move to BOTTOM_FLOOR
    // add_request_to_queue(elevator, INITIAL_REQUEST);
    int floor = elevio_floorSensor();
    if (floor == BOTTOM_FLOOR)
    {
        return;
    }

    elevio_motorDirection(DIRN_DOWN);
    while (floor != BOTTOM_FLOOR)
    {
        check_emergency_stop(elevator);
        floor = elevio_floorSensor();
        if (floor != -1)
        {
            elevio_floorIndicator(floor);
        }
    }
    elevio_motorDirection(DIRN_STOP);
    elevator->current_floor = floor;
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
    sort_requests(elevator->request_queue, queueSize, lastFloor, currMovingDir);
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

void on_button_press(Elevator *elevator)
{
    Request new_req;

    if (button_pressed(&new_req))
    {
        elevio_buttonLamp(new_req.floor, new_req.button, 1);

        add_request_to_queue(elevator, new_req);
    }
}

bool button_pressed(Request *req)
{
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            if (elevio_callButton(floor, btn))
            {
                req->floor = floor;
                req->button = btn;
                return true;
            }
        }
    }

    return false;
}

void moving_elevator(Elevator *elevator)
{
    if (elevator->queue_size == 0 || elevator->in_motion)
    {
        return;
    }

    int destinationFloor = elevator->request_queue[0].floor;

    int differenceInFloors = destinationFloor - elevator->current_floor;
    MotorDirection direction = (differenceInFloors > 0) ? DIRN_UP : (differenceInFloors < 0) ? DIRN_DOWN
                                                                                             : DIRN_STOP;

    elevator->in_motion = (direction != DIRN_STOP);
    elevio_motorDirection(direction);
}

void at_right_floor(Elevator *elevator)
{
    if (elevator->queue_size == 0)
    {
        return;
    }

    int currFloor = elevio_floorSensor();

    if (currFloor != -1)
    {
        elevator->current_floor = currFloor;
        elevio_floorIndicator(currFloor);
    }

    if (currFloor == elevator->request_queue[0].floor)
    {
        stop_elevator_at_floor(elevator, currFloor);
        elevio_doorOpenLamp(0);
    }

    moving_elevator(elevator);
}

void switch_direction(Elevator *elevator)
{
    MotorDirection *currDir = &elevator->moving_direction;
    int currFloor = elevator->current_floor;

    if (currFloor == TOP_FLOOR)
    {
        *currDir = DIRN_DOWN;
    }
    else if (currFloor == BOTTOM_FLOOR)
    {
        *currDir = DIRN_UP;
    }
    else if (elevator->queue_size > 0)
    {
        MotorDirection newDir = button_type_to_motor_direction(elevator->request_queue[0].button);

        if (*currDir != newDir && newDir != DIRN_STOP)
        {
            *currDir = newDir;
        }
    }

    sort_queue(elevator);

    printf("\nSwitched direction to %s\n", motor_direction_to_string(*currDir));
}

void rest_elevator(Elevator *elevator)
{
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3)
    {
        on_button_press(elevator);
        check_emergency_stop(elevator);
    }
}

void stop_elevator_at_floor(Elevator *elevator, int floor)
{
    elevio_motorDirection(DIRN_STOP);
    elevator->in_motion = false;
    elevio_buttonLamp(floor, elevator->request_queue[0].button, 0);
    elevator->last_floor = floor;
    switch_direction(elevator); // db, might be issues here ...

    remove_request_from_queue(elevator, floor);
    elevio_doorOpenLamp(1);

    rest_elevator(elevator);

    printf("Destination reached and elevator stopped \n"); // db
    print_elevator(elevator);                              // db
}

void check_emergency_stop(Elevator *elevator)
{
    int currFloor = elevator->current_floor;
    MotorDirection currDir = elevator->moving_direction;
    bool invalidMovement = (currFloor == BOTTOM_FLOOR && currDir == DIRN_DOWN) || (currFloor == TOP_FLOOR && currDir == DIRN_UP);
    if (elevio_stopButton() || invalidMovement)
    {
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        elevator->in_motion = false;
        while(elevator->queue_size > 0)
        {
            printf("Queue = %zu \n", elevator->queue_size);
            remove_request_from_queue(elevator, elevator->request_queue[0].floor);
        }
        // kill(getpid(), SIGKILL); // Forcefully stops the program
    }
}

void print_elevator(Elevator *elevator)
{
    printf("Elevator \n");
    printf("Current floor: %d\n", elevator->current_floor);
    printf("Last floor: %d\n", elevator->last_floor);
    printf("Current moving direction: %s\n", motor_direction_to_string(elevator->moving_direction));
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
    printf("Queue capacity: %zu\n\n", elevator->queue_capacity);
}
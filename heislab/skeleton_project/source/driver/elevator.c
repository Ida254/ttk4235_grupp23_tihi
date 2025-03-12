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

// void moving_elevator(Elevator *elevator) // db, rather return a direction and make the two last line be another place
// {
//     if (elevator->queue_size == 0 || elevator->in_motion || elevator->is_stopped)
//     {
//         return;
//     }

//     int destinationFloor = elevator->request_queue[0].floor;
//     int differenceInFloors = destinationFloor - elevator->last_floor;
//     MotorDirection direction = (differenceInFloors > 0) ? DIRN_UP : (differenceInFloors < 0) ? DIRN_DOWN
//                                                                                              : DIRN_STOP;
//     // printf("new moving dir: %s \n\n", motor_direction_to_string(direction)); // db
//     if (direction == DIRN_STOP && elevator->current_floor == -1)
//     {
//         if (elevator->last_motor_state == DIRN_UP)
//         {
//             direction = DIRN_DOWN;
//         }
//         else if (elevator->last_motor_state == DIRN_DOWN)
//         {
//             direction = DIRN_UP;
//         }
//         elevio_motorDirection(elevator->last_motor_state);
//     }

//     elevator->in_motion = (direction != DIRN_STOP); // db, maybe return direction instead and set new dir somewhere else
//     elevio_motorDirection(direction);
//     elevator->motor_state = direction;
//     if (direction != DIRN_STOP)
//     {
//         elevator->last_motor_state = direction;
//     }
// }

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

// bool at_right_floor(Elevator *elevator)
// {
//     if (elevator->queue_size == 0)
//     {
//         return false;
//     }

//     int currFloor = elevio_floorSensor();

//     if (currFloor != -1) // db, take this another place
//     {
//         elevator->current_floor = currFloor;
//         elevio_floorIndicator(currFloor);
//     }

//     if (currFloor == elevator->request_queue[0].floor) // db, currFloor could just be elevio_floorSensor()
//     {
//         // stop_elevator_at_floor(elevator, currFloor);
//         // elevio_doorOpenLamp(0);
//         // printf("At right floor \n"); // db
//         elevator->motor_state = DIRN_STOP;
//         return true;
//     }

//     if (!elevator->is_stopped)
//     {
//         moving_elevator(elevator);
//     }
//     return false;
// }

MotorDirection get_new_motor_direction(Elevator *elevator) // db, returns direction, decides movement
{
    MotorDirection newDir = DIRN_STOP;

    if (elevator->is_stopped || elevator->queue_size == 0 || elevator->current_floor == elevator->request_queue[0].floor)
    {
        return newDir;
    }

    int destinationFloor = elevator->request_queue[0].floor;

    // Determine direction based on the current floor and destination floor
    // if (currFloor == TOP_FLOOR)
    // {
    //     newDir = DIRN_DOWN;
    //     return newDir;
    // }
    // else if (currFloor == BOTTOM_FLOOR)
    // {
    //     newDir = DIRN_UP;
    //     return newDir;
    // }

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

    // if (newDir == DIRN_UP)
    // {
    //     printf("diff = %d \n", dirInInt);
    //     printf("dirn at kill: %s \n", motor_direction_to_string(newDir));
    //     kill(getpid(), SIGKILL);
    // }
    return newDir;
}

// void switch_direction(Elevator *elevator) // db, return the direction, if not new direction return NULL, should this be it's own thread?
// {
//     MotorDirection *currDir = &elevator->moving_direction;
//     int currFloor = elevator->current_floor;

//     if (currFloor == TOP_FLOOR)
//     {
//         *currDir = DIRN_DOWN;
//     }
//     else if (currFloor == BOTTOM_FLOOR)
//     {
//         *currDir = DIRN_UP;
//     }
//     else if (elevator->queue_size > 0)
//     {
//         MotorDirection newDir = button_type_to_motor_direction(elevator->request_queue[0].button);

//         if (*currDir != newDir && newDir != DIRN_STOP)
//         {
//             *currDir = newDir;
//         }
//     }

//     sort_queue(elevator);

//     printf("Switched direction to %s\n\n", motor_direction_to_string(*currDir));
// }

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

// void check_emergency_stop(Elevator *elevator)
// bool is_emergency_stop(Elevator *elevator) // db, should not kill ...
// {
//     // int lastFloor = elevator->current_floor;
//     // MotorDirection currDir = elevator->motor_state;
//     // bool invalidMovement = (lastFloor == BOTTOM_FLOOR && currDir == DIRN_DOWN) || (lastFloor == TOP_FLOOR && currDir == DIRN_UP);

//     // if (invalidMovement) // db, should be handled elsewhere, but not sure it is
//     // {
//     //     // switch_direction(elevator);
//     //     printf("Invalid movement: %s\n\n", bool_to_string(invalidMovement)); // db
//     //     return true;
//     // }

//     if (!elevio_stopButton())
//     // if (!elevio_stopButton() && !invalidMovement)
//     {
//         return false;
//     }

//     return true;
// }

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
/**
 * @file
 * @brief The implementation file of elevator.h
 */

#include "elevator.h"

void initialize_elevator(Elevator *elevator)
{
    if (elevator == NULL)
    {
        printf("Elevator pointer is NULL!\n");
        exit(1);
    }

    elevator->current_floor = INITIAL_FLOOR;
    elevator->moving_direction = INITIAL_DIRECTION;
    elevator->in_motion = false;
    elevator->request_queue = NULL;
    elevator->queue_size = 0;
    elevator->queue_capacity = 0;
    elevator->last_floor = 0;

    // move to BOTTOM_FLOOR
    int floor = elevio_floorSensor();
    // printf("Floor = %d \n", floor);
    if (floor == BOTTOM_FLOOR)
    {
        return;
    }

    elevio_motorDirection(DIRN_DOWN);
    while (floor != BOTTOM_FLOOR)
    {
        floor = elevio_floorSensor();
    }
    elevio_motorDirection(DIRN_STOP);
    elevator->current_floor = floor;

    printf("Elevator initialized \n"); // db
    print_elevator(elevator);
}

void free_elevator(Elevator *elevator)
{
    free(elevator->request_queue);
    elevator->request_queue = NULL;
}

void sort_queue(Elevator *elevator)
{
    if (!elevator)
    {
        printf("Error: Elevator pointer is NULL!\n");
        return;
    }

    size_t queueSize = elevator->queue_size;
    int currFloor = elevator->current_floor;
    MotorDirection currMovingDir = elevator->moving_direction;
    sort_requests(elevator->request_queue, queueSize, currFloor, currMovingDir);
}

void add_request_to_queue(Elevator *elevator, Request new_req)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    if (in_array(elevator->request_queue, *queueSize, new_req))
    {
        printf("New req already in the queue");              // db
        print_requests(elevator->request_queue, *queueSize); // db
        return;
    }
    add_request(&elevator->request_queue, queueSize, queueCapacity, new_req);
    // print_elevator(elevator);

    sort_queue(elevator);

    printf("\nAdded to queue and sorted\n"); // db
    print_elevator(elevator);
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    remove_request_by_floor(&elevator->request_queue, queueSize, queueCapacity, floor);
}

// thomas sin del
void on_button_press(Elevator *elevator)
{
    int buttonPressed;
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            buttonPressed = elevio_callButton(floor, btn);
            elevio_buttonLamp(floor, btn, buttonPressed);

            if (buttonPressed)
            {
                // printf("buttonPressed = %d \n", buttonPressed);
                // printf("floor = %d \n", floor);
                MotorDirection dirReq = int_to_motor_direction(btn); // db, should be a button here
                // printf("direction = %d \n", dirReq);
                Request destReq = {floor, dirReq};
                // bool inQueue = false;

                // if (elevator->queue_size != 0) // db, there are an error here ... check out
                // {
                //     inQueue = in_array(elevator->request_queue, elevator->queue_size, destReq);
                //     printf("inQueue = %d \n", inQueue);
                // }

                // if (!inQueue)
                // {
                //     printf("Adding request to queue \n"); // db
                add_request_to_queue(elevator, destReq);
                // printf("after add, sorting \n");
                // sort_queue(elevator);
                // printf("after sorting, floor = %d , direction = %d \n", elevator->request_queue->floor, elevator->request_queue->button);
                // }
            }
        }
    }
}

void moving_elevator(Elevator *elevator)
{ // moves the elevator, updates states
    // MotorDirection direction = elevator->request_queue[0].button;
    int destinationFloor = elevator->request_queue[0].floor;
    if (elevator->in_motion)
    {
        return;
    }

    int differenceInFloors = destinationFloor - elevator->current_floor;
    MotorDirection direction = (differenceInFloors > 0) ? DIRN_UP : (differenceInFloors < 0) ? DIRN_DOWN
                                                                                             : DIRN_STOP;

    elevator->in_motion = (direction != DIRN_STOP);
    elevio_motorDirection(direction);
}

void at_right_floor(Elevator *elevator)
{ // as long as door is closed, check if at right floor
    if (elevator->queue_size == 0)
    {
        // printf("Queue is empty \n"); // db
        return;
    }

    elevator->current_floor = elevio_floorSensor();
    int currFloor = elevator->current_floor;
    Request req = elevator->request_queue[0];

    if (currFloor == req.floor)
    {
        printf("At floor %d \n", currFloor); // db
        stop_elevator_at_floor(elevator, currFloor);

        rest_elevator(elevator);

        elevio_doorOpenLamp(0);
        // elevator->_doorOpen = false;
        // Ida: make 'remove from queue' function
    }
    if (elevator->queue_size == 0)
    {
        printf("Queue is empty \n");
        return;
    }
    // switch_direction(elevator);
    moving_elevator(elevator);
}

// void move_elevator_to_floor(Elevator *elevator, Request destinationRequest)
// {
//     MotorDirection direction = button_type_to_motor_direction(destinationRequest.button);
//     elevator->moving_direction = direction;

//     int floor = destinationRequest.floor;
//     bool stopped = false;

//     elevio_motorDirection(direction);

//     while (!stopped)
//     {
//         if (elevio_floorSensor() == floor)
//         {
//             elevio_motorDirection(DIRN_STOP);
//             elevio_stopLamp(floor);
//             elevator->current_floor = floor;
//             stopped = true;
//         }
//     }
// }

void switch_direction(Elevator *elevator)
{
    Request new_req = elevator->request_queue[0];
    if (elevator->moving_direction != button_type_to_motor_direction(new_req.button))
    {
        return;
    }

    elevator->moving_direction = button_type_to_motor_direction(new_req.button);
    printf("Switched direction \n");
}

void rest_elevator(Elevator *elevator)
{
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3)
    {
        on_button_press(elevator);
        check_emergency_stop();
    }
}

void stop_elevator_at_floor(Elevator *elevator, int floor)
{
    elevio_motorDirection(DIRN_STOP);
    elevator->in_motion = false;
    remove_request_from_queue(elevator, floor);
    switch_direction(elevator);
    elevio_buttonLamp(floor, elevator->moving_direction, 0);
    elevio_stopLamp(floor);
    elevio_doorOpenLamp(1);
}

void check_emergency_stop()
{
    if (elevio_stopButton())
    {
        elevio_motorDirection(DIRN_STOP);
        kill(getpid(), SIGKILL); // Forcefully stops the program
    }
}

void print_elevator(Elevator *elevator)
{
    printf("Elevator \n");
    printf("Current floor: %d\n", elevator->current_floor);
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
    printf("Queue capacity: %zu\n", elevator->queue_capacity);
}

void test_elevator(void)
{
    Elevator elevator;
    initialize_elevator(&elevator);
    printf("Initial elevator \n");
    print_elevator(&elevator);

    // size_t initial_requests_size = 13;
    // Request initial_requests[] = {
    //     {5, BUTTON_HALL_UP},
    //     {5, BUTTON_CAB},
    //     {5, BUTTON_HALL_DOWN},
    //     {3, BUTTON_HALL_UP},
    //     {8, BUTTON_HALL_DOWN},
    //     {2, BUTTON_HALL_UP},
    //     {1, BUTTON_HALL_UP},
    //     {7, BUTTON_HALL_DOWN},
    //     {3, BUTTON_HALL_DOWN},
    //     {6, BUTTON_CAB},
    //     {8, BUTTON_CAB},
    //     {10, BUTTON_HALL_UP},
    //     {2, BUTTON_CAB}};

    size_t initial_requests_size = 3;
    Request initial_requests[] = {{3, BUTTON_HALL_DOWN}, {2, BUTTON_HALL_UP}, {1, BUTTON_HALL_UP}};

    for (size_t i = 0; i < initial_requests_size; i++)
    {
        add_request_to_queue(&elevator, initial_requests[i]);
    }
    printf("\nAfter sorting \n");
    print_elevator(&elevator);

    // sort_queue(&elevator);

    // printf("\nAfter sorting, before removing \n");
    // print_elevator(&elevator);

    // remove_request_from_queue(&elevator, 5);
    // remove_request_from_queue(&elevator, 2);
    // remove_request_from_queue(&elevator, 3);

    // printf("\nAfter removing ");
    // print_elevator(&elevator);
}

// int main()
// {
//     test_elevator();
//     // gcc -o elevator_program source/driver/elevator.c source/driver/request.c source/driver/elevio.c
//     // ./elevator_program
//     // rm ./elevator_program
//     return 0;
// }

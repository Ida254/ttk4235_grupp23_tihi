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

    // move to BOTTOM_FLOOR
    int floor = elevio_floorSensor();
    printf("Floor = %d \n", floor);
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

    // printf("Elevator pointer address: %p\n", (void *)elevator); // db         // db

    size_t queueSize = elevator->queue_size;
    int currFloor = elevator->current_floor;
    MotorDirection currMovingDir = elevator->moving_direction;
    sort_requests(elevator->request_queue, queueSize, currFloor, currMovingDir);
}

void add_request_to_queue(Elevator *elevator, Request new_req)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    add_request(&elevator->request_queue, queueSize, queueCapacity, new_req);

    sort_queue(elevator);
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    remove_request_byfloor(&elevator->request_queue, queueSize, queueCapacity, floor);
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
                printf("buttonPressed = %d \n", buttonPressed);
                printf("floor = %d \n", floor);
                MotorDirection dirReq = int_to_motor_direction(btn); // db, should be a button here
                printf("direction = %d \n", dirReq);
                Request destReq = {floor, dirReq};
                bool inQueue = false;

                if (elevator->queue_size != 0) // db, there are an error here ... check out
                {
                    inQueue = in_array(elevator->request_queue, elevator->queue_size, destReq);
                    printf("inQueue = %d \n", inQueue);
                }

                if (!inQueue)
                {
                    printf("Adding request to queue \n");
                    add_request_to_queue(elevator, destReq);
                    printf("after add, sorting \n");
                    sort_queue(elevator);
                    printf("after sorting, floor = %d , direction = %d \n", elevator->request_queue->floor, elevator->request_queue->button);
                }
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
    // elevator->_motorState = direction;
    // elevator->moving_direction = direction;
    // printf("Moving elevator = %d \n", direction);
    // elevio_motorDirection(direction);
    // if (direction == DIRN_UP && elevator->_currentFloor > destinationFloor)
    // {
    //     elevio_motorDirection(DIRN_DOWN);
    //     elevator->_inMotion = true;
    // }
    // else if (direction == DIRN_DOWN && elevator->_currentFloor < destinationFloor)
    // {
    //     elevio_motorDirection(DIRN_UP);
    //     elevator->_inMotion = true;
    // }
    // else
    // {
    //     elevio_motorDirection(direction);
    //     elevator->_inMotion = true;
    // }
    int differenceInFloors = destinationFloor - elevator->current_floor;
    if (differenceInFloors > 0)
    {
        elevio_motorDirection(DIRN_UP);
        elevator->in_motion = true;
    }
    else if (differenceInFloors < 0)
    {
        elevio_motorDirection(DIRN_DOWN);
        elevator->in_motion = true;
    }
    else
    {
        elevio_motorDirection(DIRN_STOP);
        elevator->in_motion = false;
    }
    return;
}

void at_right_floor(Elevator *elevator)
{ // as long as door is closed, check if at right floor
    if (elevator->queue_size == 0)
    {
        printf("Queue is empty \n");
        return;
    }

    int floor1 = elevio_floorSensor();
    elevator->current_floor = floor1;

    if (elevator->current_floor == elevator->request_queue[0].floor)
    {
        printf("At right floor for reeeaaaal!!! \n");
        // elevator->_motorState = DIRN_STOP;
        elevio_motorDirection(DIRN_STOP);
        elevator->in_motion = false;
        elevio_doorOpenLamp(1);
        if (elevator->current_floor == 0)
        {
            elevio_buttonLamp(elevator->current_floor, 0, 0);
        }
        else if (elevator->current_floor == 3)
        {
            elevio_buttonLamp(elevator->current_floor, 1, 0);
        }
        else
        {
            elevio_buttonLamp(elevator->current_floor, elevator->request_queue[0].button, 0);
        }
        // elevator->_doorOpen = true;
        time_t start_time = time(NULL);
        while (time(NULL) - start_time < 3)
        {
            on_button_press(elevator);
            if (elevio_stopButton())
            {
                elevio_motorDirection(DIRN_STOP);
                kill(getpid(), SIGKILL); // Forcefully stops the program
            }
        }
        elevio_doorOpenLamp(0);
        remove_request_from_queue(elevator, elevator->current_floor);
        // elevator->_doorOpen = false;
        // Ida: make 'remove from queue' function
    }
    if (elevator->queue_size == 0)
    {
        printf("Queue is empty \n");
        return;
    }
    moving_elevator(elevator);
}

void move_elevator_tofloor(Elevator *elevator, Request destinationRequest)
{
    MotorDirection direction = button_type_to_motor_direction(destinationRequest.button);
    elevator->moving_direction = direction;

    int floor = destinationRequest.floor;
    bool stopped = false;

    elevio_motorDirection(direction);

    while (!stopped)
    {
        if (elevio_floorSensor() == floor)
        {
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(floor);
            elevator->current_floor = floor;
            stopped = true;
        }
    }
}

void print_elevator(Elevator *elevator)
{
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

    size_t initial_requests_size = 13;
    Request initial_requests[] = {
        {5, BUTTON_HALL_UP},
        {5, BUTTON_CAB},
        {5, BUTTON_HALL_DOWN},
        {3, BUTTON_HALL_UP},
        {8, BUTTON_HALL_DOWN},
        {2, BUTTON_HALL_UP},
        {1, BUTTON_HALL_UP},
        {7, BUTTON_HALL_DOWN},
        {3, BUTTON_HALL_DOWN},
        {6, BUTTON_CAB},
        {8, BUTTON_CAB},
        {10, BUTTON_HALL_UP},
        {2, BUTTON_CAB}};

    for (size_t i = 0; i < initial_requests_size; i++)
    {
        add_request_to_queue(&elevator, initial_requests[i]);
    }
    printf("\nBefore sorting \n");
    print_elevator(&elevator);

    // sort_queue(&elevator);

    printf("\nAfter sorting, before removing \n");
    print_elevator(&elevator);

    remove_request_from_queue(&elevator, 5);
    remove_request_from_queue(&elevator, 2);
    remove_request_from_queue(&elevator, 3);

    printf("\nAfter removing ");
    print_elevator(&elevator);
}

// int main()
// {
//     test_elevator();
//     // gcc -o elevator_program source/driver/elevator.c source/driver/request.c source/driver/elevio.c
//     // ./elevator_program
//     // rm ./elevator_program
//     return 0;
// }

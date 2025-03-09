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
    
    elevator->_currentFloor = INITIAL_FLOOR;
    elevator->_movingDirection = INITIAL_DIRECTION;
    printf("Elevator initialized \n");
    elevator->_inMotion = false;
    elevator->_requestQueue = NULL;
    elevator->_queueSize = 0;
    printf("In motion = %d \n", elevator->_inMotion);
    elevator->_queueCapacity = 0;
    printf("What \n");
    
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
    free(elevator->_requestQueue);
    elevator->_requestQueue = NULL;
}

void sort_queue(Elevator *elevator)
{
    if (!elevator)
    {
        printf("Error: Elevator pointer is NULL!\n");
        return;
    }

    // printf("Elevator pointer address: %p\n", (void *)elevator);         // db

    size_t queueSize = elevator->_queueSize;
    int currFloor = elevator->_currentFloor;
    MotorDirection currMovingDir = elevator->_movingDirection;
    printf("done with sorting \n");

    sort_requests(elevator->_requestQueue, queueSize, currFloor, currMovingDir);
}

void add_request_to_queue(Elevator *elevator, Request new_req)
{
    size_t *queueSize = &elevator->_queueSize;
    size_t *queueCapacity = &elevator->_queueCapacity;
    printf("Going to add \n");
    add_request(&elevator->_requestQueue, queueSize, queueCapacity, new_req);

    // sort_queue(elevator); // db, there are issues ... i dont know why
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t *queueSize = &elevator->_queueSize;
    size_t *queueCapacity = &elevator->_queueCapacity;

    remove_request_by_floor(&elevator->_requestQueue, queueSize, queueCapacity, floor);
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

                if (elevator->_queueSize != 0) // db, there are an error here ... check out
                {
                    inQueue = in_array(elevator->_requestQueue, elevator->_queueSize, destReq);
                    printf("inQueue = %d \n", inQueue);
                }

                if (!inQueue)
                {
                    printf("Adding request to queue \n");
                    add_request_to_queue(elevator, destReq);
                    printf("after add, sorting \n");
                    sort_queue(elevator);
                    printf("after sorting, floor = %d , direction = %d \n", elevator->_requestQueue->_floor, elevator->_requestQueue->_button);
                }
            }
        }
    }
}

void moving_elevator(Elevator *elevator)
{ // moves the elevator, updates states
    MotorDirection direction = elevator->_requestQueue[0]._button;
    int destinationFloor = elevator->_requestQueue[0]._floor;
    if (elevator->_inMotion)
    {
        return;
    }
    // elevator->_motorState = direction;
    // elevator->_movingDirection = direction;
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
    int differenceInFloors = destinationFloor - elevator->_currentFloor;
    if(differenceInFloors > 0)
    {
        elevio_motorDirection(DIRN_UP);
        elevator->_inMotion = true;
    }
    else if(differenceInFloors < 0)
    {
        elevio_motorDirection(DIRN_DOWN);
        elevator->_inMotion = true;
    }
    else
    {
        elevio_motorDirection(DIRN_STOP);
        elevator->_inMotion = false;
    }
    return;
}

void at_right_floor(Elevator *elevator)
{ // as long as door is closed, check if at right floor
    int floor1 = elevio_floorSensor();
    elevator->_currentFloor = floor1;
    if (elevator->_queueSize == 0)
    {
        printf("Queue is empty \n");
        return;
    }
    if (elevator->_currentFloor == elevator->_requestQueue[0]._floor)
    {
        printf("At right floor for reeeaaaal!!! \n");
        // elevator->_motorState = DIRN_STOP;
        elevio_motorDirection(DIRN_STOP);
        elevator->_inMotion = false;
        if (elevator->_currentFloor == 0)
        {
            elevio_buttonLamp(elevator->_currentFloor, 0, 0);
        } else if(elevator->_currentFloor == 3){
            elevio_buttonLamp(elevator->_currentFloor, 1, 0);
        }
        else
        {
            elevio_buttonLamp(elevator->_currentFloor, elevator->_requestQueue[0]._button, 0);
        }
        // elevator->_doorOpen = true;
        elevio_doorOpenLamp(1);
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
        remove_request_from_queue(elevator, elevator->_currentFloor);
        // elevator->_doorOpen = false;
        // Ida: make 'remove from queue' function
    }
    if (elevator->_queueSize == 0)
    {
        printf("Queue is empty \n");
        return;
    }
    moving_elevator(elevator);
}

void move_elevator_to_floor(Elevator *elevator, Request destinationRequest)
{
    MotorDirection direction = button_type_to_motor_direction(destinationRequest._button);
    elevator->_movingDirection = direction;

    int floor = destinationRequest._floor;
    bool stopped = false;

    elevio_motorDirection(direction);

    while (!stopped)
    {
        if (elevio_floorSensor() == floor)
        {
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(floor);
            elevator->_currentFloor = floor;
            stopped = true;
        }
    }
}

void print_elevator(Elevator *elevator)
{
    printf("Current floor: %d\n", elevator->_currentFloor);
    printf("Current moving direction: %s\n", motor_direction_to_string(elevator->_movingDirection));

    if (elevator->_queueSize > 0)
    {
        print_requests(elevator->_requestQueue, elevator->_queueSize);
    }
    else
    {
        printf("No requests in the queue.\n");
    }

    printf("Queue size: %zu\n", elevator->_queueSize);
    printf("Queue capacity: %zu\n", elevator->_queueCapacity);
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

    sort_queue(&elevator);

    printf("\nBefore removing \n");
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

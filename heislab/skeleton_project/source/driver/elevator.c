/**
 * @file
 * @brief The implementation file of elevator.h
 */

#include "elevator.h"

void initialize_elevator(Elevator *elevator, size_t initialCapacity)
{
    // elevator->_currentFloor = 1;
    elevator->_movingDirection = DIRN_UP;
    elevator->_currentFloor = 4; // db, remove later
    // elevator->_movingDirection = DIRN_DOWN;

    // Allocate memory for the queue (initial size of 10, for example)
    elevator->_destinationQueue = (DestinationRequest *)malloc(initialCapacity * sizeof(DestinationRequest));
    elevator->_queueSize = 0;                   // Initialize with no elements
    elevator->_queueCapacity = initialCapacity; // Set the initial capacity

    // Get the elevator to the first floor
    int floor = elevio_floorSensor();
    printf("Floor = %d \n", floor);
    if (floor == 0)
    {
        return;
    }
    else
    {
        elevio_motorDirection(DIRN_DOWN);
        while (floor != 0)
        {
            floor = elevio_floorSensor();
        }
        elevio_motorDirection(DIRN_STOP);
        return;
    }
}

void free_elevator(Elevator *elevator)
{
    free(elevator->_destinationQueue);
    elevator->_destinationQueue = NULL;
}

void sort_queue(Elevator *elevator)
{
    DestinationRequest *elQueue = elevator->_destinationQueue;
    size_t size = elevator->_queueSize;

    MotorDirection mainDir = elevator->_movingDirection;
    MotorDirection otherDir = (mainDir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    DestinationRequest *reqForward = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));
    DestinationRequest *reqBackwards = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));
    DestinationRequest *reqBehind = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));

    if (!reqForward || !reqBackwards || !reqBehind)
    {
        free(reqForward);
        free(reqBackwards);
        free(reqBehind);
        return;
    }

    size_t j = 0;
    size_t k = 0;
    size_t l = 0;

    printf("mainDir: %d \n", mainDir); // db
    for (size_t i = 0; i < size; i++)
    {
        MotorDirection elevatorDir = button_type_to_motor_direction(elQueue[i]._buttonType);
        bool isStop = (elevatorDir == DIRN_STOP);
        bool isMainDir = (elevatorDir == mainDir);
        bool isOtherDir = (elevatorDir == otherDir);

        if (isMainDir || isStop)
        {
            bool isForward = (mainDir == DIRN_DOWN) ? (elQueue[i]._floor <= elevator->_currentFloor) : (elQueue[i]._floor >= elevator->_currentFloor);
            if (isForward || isStop)
            {
                reqForward[j++] = elQueue[i];
            }
            else
            {
                reqBehind[l++] = elQueue[i];
            }
        }
        else if (isOtherDir)
        {
            reqBackwards[k++] = elQueue[i];
        }
    }

    bubble_sort(reqForward, j, mainDir);
    bubble_sort(reqBackwards, k, otherDir);
    bubble_sort(reqBehind, l, mainDir);

    print_queue(reqForward, j);
    print_queue(reqBackwards, k);
    print_queue(reqBehind, l);

    size_t appendIndex = 0;
    memcpy(elQueue + appendIndex, reqForward, j * sizeof(DestinationRequest));
    appendIndex += j;

    memcpy(elQueue + appendIndex, reqBackwards, l * sizeof(DestinationRequest));
    appendIndex += l;

    memcpy(elQueue + appendIndex, reqBehind, k * sizeof(DestinationRequest));
    appendIndex += k;

    free(reqForward);
    free(reqBehind);
    free(reqBackwards);
}

void add_request_to_queue(Elevator *elevator, DestinationRequest destinationRequest)
{
    if (elevator->_queueSize == elevator->_queueCapacity)
    {
        elevator->_queueCapacity *= 2; // Double the capacity
        elevator->_destinationQueue = (DestinationRequest *)realloc(elevator->_destinationQueue, elevator->_queueCapacity * sizeof(DestinationRequest));
    }

    int floor = destinationRequest._floor;
    ButtonType button;
    switch (floor)
    {
    case 0:
        button = BUTTON_HALL_DOWN;
        break;
    case 3:
        button = BUTTON_HALL_UP;
        break;

    default:
        button = destinationRequest._buttonType;
        break;
    }

    int currentIndex = elevator->_queueSize;
    elevator->_destinationQueue[currentIndex]._floor = floor;
    elevator->_destinationQueue[currentIndex]._buttonType = button;
    elevator->_queueSize++;
    sort_queue(elevator);
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t size = elevator->_queueSize;
    DestinationRequest *queue = elevator->_destinationQueue;
    DestinationRequest *tempQueue = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));

    if (!tempQueue)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    int j = 0;

    for (int i = 0; i < size; i++)
    {
        if (queue[i]._floor != floor)
        {
            tempQueue[j++] = queue[i];
        }
    }

    free(elevator->_destinationQueue);
    elevator->_destinationQueue = tempQueue;
    elevator->_queueSize = j;

    if (elevator->_queueSize < elevator->_queueCapacity / 4 && elevator->_queueCapacity > 10)
    {
        elevator->_queueCapacity /= 2;
        elevator->_destinationQueue = (DestinationRequest *)realloc(elevator->_destinationQueue, elevator->_queueCapacity * sizeof(DestinationRequest));
    }
}

void on_button_press(Elevator *elevator)
{
    int buttonPressed;
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            buttonPressed = elevio_callButton(floor, btn);
            printf("buttonPressed = %d \n", buttonPressed);
            elevio_buttonLamp(floor, btn, buttonPressed);

            if (buttonPressed)
            {
                MotorDirection dirReq = int_to_motor_direction(btn);
                DestinationRequest destReq = {floor, dirReq};
                bool inQueue = false;

                if (elevator->_queueSize != 0) // db, there are an error here ... check out
                {
                    inQueue = in_array(elevator->_destinationQueue, elevator->_queueSize, destReq);
                    printf("What");
                }

                if (!inQueue)
                {
                    add_request_to_queue(elevator, destReq);
                    sort_queue(elevator);
                }
            }
        }
    }
}

void moving_elevator(Elevator *elevator)
{ // moves the elevator, updates states
    MotorDirection direction = elevator->_destinationQueue[0]._buttonType;
    if (elevator->_movingDirection == direction)
    {
        return;
    }
    // elevator->_motorState = direction;
    elevator->_movingDirection = direction;
    elevio_motorDirection(direction);
    return;
}

void at_right_floor(Elevator *elevator)
{ // as long as door is closed, check if at right floor
    if (elevator->_queueSize == 0)
    {
        return;
    }
    if (elevator->_currentFloor == elevator->_destinationQueue[0]._floor)
    {
        // elevator->_motorState = DIRN_STOP;
        elevio_motorDirection(DIRN_STOP);
        // elevator->_doorOpen = true;
        elevio_doorOpenLamp(1);
        time_t start_time = time(NULL);
        while (time(NULL) - start_time < 3)
        {
            on_button_press(elevator);
            // if (elevio_obstruction())
            // {
            //     elevio_stopLamp(1);
            // }
            // else
            // {
            //     elevio_stopLamp(0);
            // }

            if (elevio_stopButton())
            {
                elevio_motorDirection(DIRN_STOP);
                kill(getpid(), SIGKILL); // Forcefully stops the program
            }
        }
        elevio_doorOpenLamp(0);
        // elevator->_doorOpen = false;
        // Ida: make 'remove from queue' function
    }
    else
    {
        moving_elevator(elevator);
    }
}

void move_elevator_to_floor(Elevator *elevator, DestinationRequest destinationRequest)
{
    MotorDirection direction = button_type_to_motor_direction(destinationRequest._buttonType);
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

void test_sort_queue()
{
    Elevator elevator;
    initialize_elevator(&elevator, 10); // Initialize with capacity for 10 elements

    DestinationRequest requests[] = {
        {5, BUTTON_HALL_UP},
        {5, BUTTON_CAB},
        {5, BUTTON_HALL_DOWN},
        {3, BUTTON_HALL_UP},
        {8, BUTTON_HALL_DOWN},
        {2, BUTTON_HALL_UP},
        {1, BUTTON_HALL_DOWN},
        {7, BUTTON_HALL_DOWN},
        {3, BUTTON_HALL_DOWN},
        {6, BUTTON_CAB},
        {8, BUTTON_CAB},
        {10, BUTTON_HALL_UP},
        {2, BUTTON_CAB}};

    int numRequests = sizeof(requests) / sizeof(requests[0]);

    for (int i = 0; i < numRequests; i++)
    {
        DestinationRequest request = requests[i];
        add_request_to_queue(&elevator, request);
    }

    size_t queueSize = elevator._queueSize;

    printf("Before sorting:\n");
    print_queue(elevator._destinationQueue, queueSize);

    sort_queue(&elevator);

    printf("After sorting and before removing:\n");
    print_queue(elevator._destinationQueue, queueSize);

    remove_request_from_queue(&elevator, 5);
    printf("After removing:\n");
    print_queue(elevator._destinationQueue, queueSize);

    DestinationRequest desReq = {5, BUTTON_CAB};
    // bubble_sort(elevator._destinationQueue, queueSize, DIRN_UP);
    // add_request_to_queue(&elevator, desReq);
    printf("After joining something again:\n");
    print_queue(elevator._destinationQueue, queueSize);

    // printf("current floor: %d \n", elevator._currentFloor);
    // move_elevator_to_floor(&elevator, elevator._destinationQueue[0]);
    // printf("current floor after moving: %d \n", elevator._currentFloor);
    // print_queue(elevator._destinationQueue, queueSize);
}

// // for testing, db, remove later
int main()
{
    test_sort_queue();
    //     // gcc -o elevator_program source/driver/elevator.c source/driver/elevio.c source/driver/DestinationRequest.c
    //     // ./elevator_program
    //     // rm elevator_program

    //     // does initialize actually make sure that the elevator starts at first floor??
    return 0;
}
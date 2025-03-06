/**
 * @file
 * @brief The implementation file of elevator.h
 */

#include "elevator.h"

void initialize_elevator(Elevator *elevator, size_t initialCapacity)
{
    // elevator->_currentFloor = 1;
    // elevator->_currentFloor = 4; // db, remove later
    elevator->_movingDirection = DIRN_UP;

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
}

void sort_queue(Elevator *elevator)
{
    DestinationRequest *elQueue = elevator->_destinationQueue;
    size_t size = elevator->_queueSize;

    MotorDirection mainDir = elevator->_movingDirection;
    MotorDirection otherDir = (mainDir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    DestinationRequest *requestsInFrontOfCurrentFloor = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));
    DestinationRequest *requestsBehindCurrentFloor = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));
    DestinationRequest *requestsOppositeDirection = (DestinationRequest *)malloc(size * sizeof(DestinationRequest));

    if (!requestsInFrontOfCurrentFloor || !requestsBehindCurrentFloor || !requestsOppositeDirection)
    {
        free(requestsInFrontOfCurrentFloor);
        free(requestsBehindCurrentFloor);
        free(requestsOppositeDirection);
        return;
    }

    size_t countRequestsInFrontOfCurrentFloor = 0;
    size_t countRequestsBehindCurrentFloor = 0;
    size_t countRequestsOppositeDirection = 0;

    for (size_t i = 0; i < size; i++)
    {
        DestinationRequest el = elQueue[i];
        bool isStop = (el._direction == DIRN_STOP);
        bool isMainDir = (el._direction == mainDir);
        bool isOtherDir = (el._direction == otherDir);

        if (isMainDir || isStop)
        {
            bool isForward = (mainDir == DIRN_DOWN) ? (el._floor <= elevator->_currentFloor) : (el._floor >= elevator->_currentFloor);
            if (isForward)
            {
                requestsInFrontOfCurrentFloor[countRequestsInFrontOfCurrentFloor++] = el;
            }
            else
            {
                requestsBehindCurrentFloor[countRequestsBehindCurrentFloor++] = el;
            }
        }
        else if (isOtherDir)
        {
            requestsOppositeDirection[countRequestsOppositeDirection++] = el;
        }
    }

    bubble_sort(requestsInFrontOfCurrentFloor, countRequestsInFrontOfCurrentFloor, mainDir);
    bubble_sort(requestsBehindCurrentFloor, countRequestsBehindCurrentFloor, mainDir);
    bubble_sort(requestsOppositeDirection, countRequestsOppositeDirection, otherDir);

    size_t appendIndex = 0;
    memcpy(elQueue + appendIndex, requestsInFrontOfCurrentFloor, countRequestsInFrontOfCurrentFloor * sizeof(DestinationRequest));
    appendIndex += countRequestsInFrontOfCurrentFloor;

    memcpy(elQueue + appendIndex, requestsOppositeDirection, countRequestsOppositeDirection * sizeof(DestinationRequest));
    appendIndex += countRequestsOppositeDirection;

    memcpy(elQueue + appendIndex, requestsBehindCurrentFloor, countRequestsBehindCurrentFloor * sizeof(DestinationRequest));
    appendIndex += countRequestsBehindCurrentFloor;

    free(requestsInFrontOfCurrentFloor);
    free(requestsBehindCurrentFloor);
    free(requestsOppositeDirection);
}

void add_request_to_queue(Elevator *elevator, DestinationRequest destinationRequest)
{
    if (elevator->_queueSize == elevator->_queueCapacity)
    {
        elevator->_queueCapacity *= 2; // Double the capacity
        elevator->_destinationQueue = (DestinationRequest *)realloc(elevator->_destinationQueue, elevator->_queueCapacity * sizeof(DestinationRequest));
    }

    int currentIndex = elevator->_queueSize;
    elevator->_destinationQueue[currentIndex]._floor = destinationRequest._floor;
    elevator->_destinationQueue[currentIndex]._direction = destinationRequest._direction;
    elevator->_queueSize++;
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t size = elevator->_queueSize;
    DestinationRequest *queue = elevator->_destinationQueue;
    int i = 0, j = 0;

    while (i < size)
    {
        if (queue[i]._floor != floor)
        {
            queue[j] = queue[i];
            j++;
        }
        i++;
    }
    elevator->_queueSize = j;

    if (elevator->_queueSize < elevator->_queueCapacity / 4 && elevator->_queueCapacity > 10)
    {
        elevator->_queueCapacity /= 2;
        elevator->_destinationQueue = (DestinationRequest *)realloc(elevator->_destinationQueue, elevator->_queueCapacity * sizeof(DestinationRequest));
    }
}

void moving_elevator(Elevator *elevator)
{ // moves the elevator, updates states
    MotorDirection direction = elevator->_destinationQueue[0]._direction;
    if (direction == elevator->_movingDirection)
    {
        return;
    }
    else
    {
        elevator->_motorState = direction;
        elevio_motorDirection(direction);
        return;
    }
}

void at_right_floor(Elevator *elevator)
{ // as long as door is closed, check if at right floor
    if (elevator->_queueSize == 0)
    {
        return;
    }
    if (elevator->_currentFloor == elevator->_destinationQueue[0]._floor)
    {
        elevator->_motorState = DIRN_STOP;
        elevio_motorDirection(DIRN_STOP);
        elevator->_doorOpen = true;
        elevio_doorOpenLamp(1);
        time_t start_time = time(NULL);
        while (time(NULL) - start_time < 3)
        {
            button_pressed(elevator);
            if (elevio_obstruction())
            {
                elevio_stopLamp(1);
            }
            else
            {
                elevio_stopLamp(0);
            }

            if (elevio_stopButton())
            {
                elevio_motorDirection(DIRN_STOP);
                kill(getpid(), SIGKILL); // Forcefully stops the program
            }
        }
        elevio_doorOpenLamp(0);
        elevator->_doorOpen = false;
        // Ida: make 'remove from queue' function
    }
    else
    {
        moving_elevator(elevator);
    }
}

void test_sort_queue()
{
    Elevator elevator;
    initialize_elevator(&elevator, 10); // Initialize with capacity for 10 elements

    DestinationRequest requests[] = {
        {5, DIRN_UP},
        {5, DIRN_STOP},
        {5, DIRN_DOWN},
        {3, DIRN_UP},
        {8, DIRN_DOWN},
        {2, DIRN_UP},
        {1, DIRN_DOWN},
        {7, DIRN_DOWN},
        {3, DIRN_DOWN},
        {6, DIRN_STOP},
        {8, DIRN_STOP},
        {10, DIRN_UP},
        {2, DIRN_STOP}};

    int numRequests = sizeof(requests) / sizeof(requests[0]);

    for (int i = 0; i < numRequests; i++)
    {
        DestinationRequest request = requests[i];
        add_request_to_queue(&elevator, request);
    }

    printf("Before sorting:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);

    sort_queue(&elevator);

    printf("After sorting and before removing:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);

    remove_request_from_queue(&elevator, 5);
    printf("After removing:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);
}

// for testing, db, remove later
// int main()
//{
// test_sort_queue();
// gcc -o elevator_program source/driver/elevator.c source/driver/elevio.c source/driver/DestinationRequest.c
// ./elevator_program
// rm elevator_program

// does initialize actually make sure that the elevator starts at first floor??
// return 0;
//}
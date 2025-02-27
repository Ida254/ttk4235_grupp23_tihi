#include "elevator.h"

void initializeElevator(Elevator *elevator, size_t initialCapacity)
{
    elevator->_currentFloor = 4;
    elevator->_movingDirection = DIRN_UP;

    // Allocate memory for the queue (initial size of 10, for example)
    elevator->_destinationQueue = (ButtonRequest *)malloc(initialCapacity * sizeof(ButtonRequest));
    elevator->_queueSize = 0;                   // Initialize with no elements
    elevator->_queueCapacity = initialCapacity; // Set the initial capacity
}

void freeElevator(Elevator *elevator)
{
    free(elevator->_destinationQueue);
}

void addRequestToQueue(Elevator *elevator, int floor, int direction)
{
    if (elevator->_queueSize == elevator->_queueCapacity)
    {
        elevator->_queueCapacity *= 2; // Double the capacity
        elevator->_destinationQueue = (ButtonRequest *)realloc(elevator->_destinationQueue, elevator->_queueCapacity * sizeof(ButtonRequest));
    }

    elevator->_destinationQueue[elevator->_queueSize]._floor = floor;
    elevator->_destinationQueue[elevator->_queueSize]._direction = direction;
    elevator->_queueSize++;
}

void sort_queue(Elevator *elevator)
{
    ButtonRequest *elQueue = elevator->_destinationQueue;
    size_t size = elevator->_queueSize;

    MotorDirection mainDir = elevator->_movingDirection;
    MotorDirection otherDir = (mainDir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    ButtonRequest *requestsInFrontOfCurrentFloor = (ButtonRequest *)malloc(size * sizeof(ButtonRequest));
    ButtonRequest *requestsBehindCurrentFloor = (ButtonRequest *)malloc(size * sizeof(ButtonRequest));
    ButtonRequest *requestsOppositeDirection = (ButtonRequest *)malloc(size * sizeof(ButtonRequest));

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
        ButtonRequest el = elQueue[i];
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
    memcpy(elQueue + appendIndex, requestsInFrontOfCurrentFloor, countRequestsInFrontOfCurrentFloor * sizeof(ButtonRequest));
    appendIndex += countRequestsInFrontOfCurrentFloor;

    memcpy(elQueue + appendIndex, requestsOppositeDirection, countRequestsOppositeDirection * sizeof(ButtonRequest));
    appendIndex += countRequestsOppositeDirection;

    memcpy(elQueue + appendIndex, requestsBehindCurrentFloor, countRequestsBehindCurrentFloor * sizeof(ButtonRequest));
    appendIndex += countRequestsBehindCurrentFloor;

    free(requestsInFrontOfCurrentFloor);
    free(requestsBehindCurrentFloor);
    free(requestsOppositeDirection);
}

void print_queue(ButtonRequest arr[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("Floor: %d, Direction: %d\n", arr[i]._floor, arr[i]._direction);
    }
    printf("\n");
}

void test_sort_queue()
{
    Elevator elevator;
    initializeElevator(&elevator, 10); // Initialize with capacity for 10 elements

    // Adding some requests to the queue
    addRequestToQueue(&elevator, 5, DIRN_UP);
    addRequestToQueue(&elevator, 5, DIRN_STOP);
    addRequestToQueue(&elevator, 3, DIRN_UP);
    addRequestToQueue(&elevator, 8, DIRN_DOWN);
    addRequestToQueue(&elevator, 2, DIRN_UP);
    addRequestToQueue(&elevator, 1, DIRN_DOWN);
    addRequestToQueue(&elevator, 7, DIRN_DOWN);
    addRequestToQueue(&elevator, 3, DIRN_DOWN);
    addRequestToQueue(&elevator, 6, DIRN_STOP);
    addRequestToQueue(&elevator, 8, DIRN_STOP);
    addRequestToQueue(&elevator, 10, DIRN_UP);
    addRequestToQueue(&elevator, 2, DIRN_STOP);

    printf("Before sorting:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);

    sort_queue(&elevator);

    printf("After sorting:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);
}

int main()
{
    test_sort_queue();
    // gcc -o elevator_program source/driver/elevator.c source/driver/elevio.c source/driver/buttonRequest.c
    // ./elevator_program
    return 0;
}
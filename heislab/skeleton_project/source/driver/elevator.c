#include "elevator.h"

void print_queue(ButtonRequest arr[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("Floor: %d, Direction: %d\n", arr[i]._floor, arr[i]._direction);
    }
    printf("\n");
}

void swap(ButtonRequest *a, ButtonRequest *b)
{
    ButtonRequest temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(ButtonRequest arr[], size_t size, MotorDirection dir)
{
    if (size == 0)
        return;

    bool swapped;

    for (size_t i = 0; i < size - 1; i++)
    {
        swapped = false;
        for (size_t j = 0; j < size - i - 1; j++)
        {
            if ((dir == DIRN_UP && arr[j]._floor > arr[j + 1]._floor) ||
                (dir == DIRN_DOWN && arr[j]._floor < arr[j + 1]._floor))
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

int find_split_index(ButtonRequest arr[], size_t size, Elevator *elevator)
{
    int currentFloor = elevator->_currentFloor;
    MotorDirection direction = elevator->_movingDirection;

    for (size_t i = 0; i < size; i++)
    {
        if (direction == DIRN_DOWN && arr[i]._floor > currentFloor)
        {
            return i;
        }
        else if (direction == DIRN_UP && arr[i]._floor < currentFloor)
        {
            return i;
        }
    }

    return size;
}

void split_sort(ButtonRequest arr[], size_t size, Elevator *elevator)
{
    size_t splitIndex = find_split_index(arr, size, elevator);

    printf("splitIndex: %zu \n", splitIndex); // debuging

    if (splitIndex >= size)
        return;

    size_t firstPartSize = splitIndex;
    size_t secondPartSize = size - splitIndex;

    ButtonRequest *tempArr = (ButtonRequest *)malloc(firstPartSize * sizeof(ButtonRequest));
    if (!tempArr)
        return;

    memcpy(tempArr, arr, firstPartSize * sizeof(ButtonRequest));

    // Move the first part of the array to the end
    memmove(arr, &arr[splitIndex], secondPartSize * sizeof(ButtonRequest));

    // Copy tempArr back to the start
    memcpy(&arr[secondPartSize], tempArr, firstPartSize * sizeof(ButtonRequest));

    free(tempArr);
}

void sort_queue(Elevator *elevator)
{
    ButtonRequest *elQueue = elevator->_destinationQueue;
    size_t size = elevator->_queueSize;

    MotorDirection mainDir = elevator->_movingDirection;
    MotorDirection otherDir = (mainDir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    ButtonRequest *tempMainDirQueue = (ButtonRequest *)malloc(size * sizeof(ButtonRequest));
    ButtonRequest *tempOtherDirQueue = (ButtonRequest *)malloc(size * sizeof(ButtonRequest));

    if (!tempMainDirQueue || !tempOtherDirQueue)
    {
        free(tempMainDirQueue);
        free(tempOtherDirQueue);
        return;
    }

    size_t mainCount = 0;
    size_t otherCount = 0;

    for (size_t i = 0; i < size; i++)
    {
        ButtonRequest el = elQueue[i];
        if (el._direction == mainDir)
        {
            tempMainDirQueue[mainCount++] = el;
        }
        else if (el._direction == otherDir)
        {
            tempOtherDirQueue[otherCount++] = el;
        }
    }

    bubble_sort(tempMainDirQueue, mainCount, mainDir); // debug
    printf("temparr before split-sorting:\n");
    print_queue(tempMainDirQueue, elevator->_queueSize);
    split_sort(tempMainDirQueue, mainCount, elevator);
    printf("temparr after split-sorting:\n");
    print_queue(tempMainDirQueue, elevator->_queueSize);

    bubble_sort(tempOtherDirQueue, otherCount, otherDir);

    memcpy(elQueue, tempMainDirQueue, mainCount * sizeof(ButtonRequest));
    memcpy(elQueue + mainCount, tempOtherDirQueue, otherCount * sizeof(ButtonRequest));

    free(tempMainDirQueue);
    free(tempOtherDirQueue);
}

void test_sort_queue()
{
    Elevator elevator = {
        ._currentFloor = 4,
        ._movingDirection = DIRN_UP,
        ._destinationQueue = (ButtonRequest[]){
            {5, DIRN_UP}, {3, DIRN_UP}, {8, DIRN_DOWN}, {2, DIRN_UP}, {1, DIRN_DOWN}, {7, DIRN_DOWN}},
        ._queueSize = 6};

    elevator._movingDirection = DIRN_UP;
    elevator._currentFloor = 3;

    printf("Before sorting:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);

    sort_queue(&elevator);

    printf("After sorting:\n");
    print_queue(elevator._destinationQueue, elevator._queueSize);
}

int main()
{
    test_sort_queue();
    // gcc -o elevator_program source/driver/elevator.c source/driver/elevio.c
    // ./elevator_program
    return 0;
}
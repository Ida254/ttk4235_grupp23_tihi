#include "elevator.h"

void swap(ButtonRequest *a, ButtonRequest *b)
{
    ButtonRequest temp = *a;
    *a = *b;
    *b = temp;
}

int find_floor(ButtonRequest *arr, size_t size, int floor)
{
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i]._floor == floor)
            return i;
    }
    return size;
}

void bubble_sort(ButtonRequest *arr, size_t size, MotorDirection dir)
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

void split_sort(ButtonRequest *arr, size_t size, int threshold)
{
    size_t splitIndex = find_floor(arr, size, threshold);

    if (splitIndex >= size)
        return;

    ButtonRequest tempArr[size - splitIndex];
    memcpy(tempArr, &arr[splitIndex], (size - splitIndex) * sizeof(ButtonRequest));

    // Move the first part of the array to the end
    memmove(&arr[size - splitIndex], arr, splitIndex * sizeof(ButtonRequest));

    // Copy tempArr back to the start
    memcpy(arr, tempArr, (size - splitIndex) * sizeof(ButtonRequest));
}

void sort_queue(Elevator *elevator)
{
    ButtonRequest *elQueue = elevator->_destinationQueue;
    size_t size = elevator->_queueSize;

    MotorDirection mainDir = elevator->_movingDirection;
    MotorDirection otherDir = (mainDir == DIRN_DOWN) ? DIRN_DOWN : DIRN_UP;

    ButtonRequest tempMainDirQueue[size];
    ButtonRequest tempOtherDirQueue[size];

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

    bubble_sort(tempMainDirQueue, mainCount, mainDir);
    split_sort(tempMainDirQueue, mainCount, elevator->_currentFloor);

    bubble_sort(tempOtherDirQueue, otherCount, otherDir);

    memcpy(elQueue, tempMainDirQueue, mainCount * sizeof(ButtonRequest));
    memcpy(elQueue + mainCount, tempOtherDirQueue, otherCount * sizeof(ButtonRequest));
}
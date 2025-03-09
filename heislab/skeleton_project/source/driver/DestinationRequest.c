/**
 * @file
 * @brief The implementation file of DestinationRequest.h
 */

#include "DestinationRequest.h"

void swap(DestinationRequest arr[], size_t i, size_t j)
{
    DestinationRequest temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bubble_sort(DestinationRequest arr[], size_t size, MotorDirection dir)
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
                swap(arr, j, j + 1);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

bool in_array(DestinationRequest arr[], size_t size, DestinationRequest destinationRequest)
{
    for (int i = 0; i < size; i++)
    {
        if ((arr[i]._floor == destinationRequest._floor) && (arr[i]._buttonType == destinationRequest._buttonType))
        {
            return true;
        }
    }
    return false;
}

MotorDirection int_to_motor_direction(int button)
{
    MotorDirection motorDir;
    switch (button)
    {
    case 0:
        motorDir = DIRN_UP;
        break;
    case 1:
        motorDir = DIRN_DOWN;
        break;
    default:
        motorDir = DIRN_STOP;
        break;
    }
    return motorDir;
}

MotorDirection button_type_to_motor_direction(ButtonType btn)
{
    MotorDirection motorDir;
    switch (btn)
    {
    case BUTTON_HALL_UP:
        motorDir = DIRN_UP;
        break;
    case BUTTON_HALL_DOWN:
        motorDir = DIRN_DOWN;
        break;
    default:
        motorDir = DIRN_STOP;
        break;
    }
    return motorDir;
}

void print_queue(DestinationRequest arr[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("Floor: %d, Direction: %d\n", arr[i]._floor, arr[i]._buttonType);
    }
    printf("\n");
}
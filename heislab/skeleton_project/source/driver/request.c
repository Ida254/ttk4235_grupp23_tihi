#include "request.h"

const char *button_to_string(ButtonType button)
{
    switch (button)
    {
    case BUTTON_HALL_UP:
        return "B_UP";
    case BUTTON_HALL_DOWN:
        return "B_DOWN";
    case BUTTON_CAB:
        return "B_CAB";
    default:
        return "UNKNOWN";
    }
}

const char *motor_direction_to_string(MotorDirection direction)
{
    switch (direction)
    {
    case DIRN_UP:
        return "D_UP";
    case DIRN_DOWN:
        return "D_DOWN";
    case DIRN_STOP:
        return "D_STOP";
    default:
        return "UNKNOWN";
    }
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

void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size)
{
    for (size_t i = 0; i < arr2_size; i++)
    {
        arr1[index++] = arr2[i];
    }
}

void bubble_sort(Request *arr, size_t size, MotorDirection direction)
{
    int reverse = (direction == DIRN_DOWN); // Sort descending if moving down
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - i - 1; j++)
        {
            if ((reverse && arr[j]._floor < arr[j + 1]._floor) || (!reverse && arr[j]._floor > arr[j + 1]._floor))
            {
                Request temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sort_requests(Request *arr, size_t req_size, int currentFloor, MotorDirection movingDir)
{
    Request tempArrAlpha[req_size], tempArrBravo[req_size], tempArrCharlie[req_size];
    int alphaCount = 0, bravoCount = 0, charlieCount = 0;

    MotorDirection otherDir = (movingDir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    for (size_t i = 0; i < req_size; i++)
    {
        Request el = arr[i];
        int floor = el._floor;
        int isStop = (el._button == BUTTON_CAB);
        int buttonUp = (el._button == BUTTON_HALL_UP);
        int buttonDown = (el._button == BUTTON_HALL_DOWN);
        int movingUp = (movingDir == DIRN_UP);
        int movingDown = (movingDir == DIRN_DOWN);

        int forward = (movingUp && buttonUp) || (movingDown && buttonDown);
        if (isStop || forward)
        {
            int inFront = (buttonUp && floor >= currentFloor) ||
                          (buttonDown && floor <= currentFloor) ||
                          (movingUp && isStop && floor >= currentFloor) ||
                          (movingDown && isStop && floor <= currentFloor);
            if (!inFront)
            {
                tempArrCharlie[charlieCount++] = el;
            }
            else
            {
                tempArrAlpha[alphaCount++] = el;
            }
        }
        else
        {
            tempArrBravo[bravoCount++] = el;
        }
    }

    // Sorting the three priority lists
    bubble_sort(tempArrAlpha, alphaCount, movingDir);
    bubble_sort(tempArrBravo, bravoCount, otherDir);
    bubble_sort(tempArrCharlie, charlieCount, movingDir);

    // print_requests(tempArrAlpha, alphaCount); // db
    size_t index = 0;
    extend_requests(arr, index, tempArrAlpha, alphaCount);
    index += alphaCount;
    extend_requests(arr, index, tempArrBravo, bravoCount);
    index += bravoCount;
    extend_requests(arr, index, tempArrCharlie, charlieCount);
    index += charlieCount;
}

void add_request(Request **arr, size_t *arr_size, size_t *capacity, Request new_req)
{
    if (*arr_size >= *capacity)
    {
        *capacity = (*capacity == 0) ? 1 : (*capacity * 2);
        // printf("Resizing capacity to: %zu\n", *capacity); // db

        Request *newQueue = realloc(*arr, (*capacity) * sizeof(Request));
        if (!newQueue)
        {
            printf("Memory reallocation failed!\n");
            exit(1);
        }
        *arr = newQueue;
    }

    (*arr)[(*arr_size)++] = new_req;
}

void remove_request_by_floor(Request **arr, size_t *arr_size, size_t *capacity, int floor)
{
    for (size_t i = 0; i < *arr_size; i++)
    {
        if ((*arr)[i]._floor == floor)
        {
            for (size_t j = i; j < (*arr_size) - 1; j++)
            {
                (*arr)[j] = (*arr)[j + 1]; // Shift left
            }
            (*arr_size)--;
            i--;
        }
    }

    if (*capacity > (*arr_size * 2))
    {
        *capacity = *arr_size;
        *arr = realloc(*arr, (*capacity) * sizeof(Request));
        if (!*arr && *capacity > 0)
        {
            printf("Memory reallocation failed!\n");
        }
    }
}

bool in_array(Request arr[], size_t size, Request destinationRequest)
{
    for (int i = 0; i < size; i++)
    {
        if ((arr[i]._floor == destinationRequest._floor) && (arr[i]._button == destinationRequest._button))
        {
            return true;
        }
    }
    return false;
}

void print_requests(Request arr[], size_t size)
{
    if (size == 0)
    {
        printf("No requests in the queue.\n");
        return;
    }

    Request el;
    printf("Requests:\n");
    for (size_t i = 0; i < size; i++)
    {
        el = arr[i];
        printf("%zu: Floor %d, Button %s\n", i, el._floor, button_to_string(el._button));
    }
}
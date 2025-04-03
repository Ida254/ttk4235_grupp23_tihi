/**
 * @file
 * @brief Implementation of elevator request management.
 *
 * This file provides functions for handling elevator requests, such as
 * dynamically managing request arrays, sorting requests, and checking request existence.
 */

#include "request.h"

void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size)
{
    for (size_t i = 0; i < arr2_size; i++)
    {
        arr1[index++] = arr2[i];
    }
}

int compare_requests(const void *a, const void *b)
{
    Request *req_a = (Request *)a;
    Request *req_b = (Request *)b;

    // Compare based on the floor and the desired direction
    if (req_a->floor < req_b->floor)
        return -1;
    if (req_a->floor > req_b->floor)
        return 1;

    return 0;
}

void bubble_sort(Request *arr, size_t size, MotorDirection direction)
{
    if (size <= 1)
    {
        return;
    }

    if (direction == DIRN_DOWN)
    {
        // Reverse order for sorting when going down
        qsort(arr, size, sizeof(Request), compare_requests);
        // Reverse the array to sort in descending order
        for (size_t i = 0; i < size / 2; i++)
        {
            Request temp = arr[i];
            arr[i] = arr[size - i - 1];
            arr[size - i - 1] = temp;
        }
    }
    else
    {
        // Default sorting order for sorting when going up
        qsort(arr, size, sizeof(Request), compare_requests);
    }
}

void sort_requests(Request *arr, size_t arr_size, int curr_floor, MotorDirection moving_dir, bool in_motion)
{
    if (arr_size < 2)
    {
        printf("Abort: arr_size = %zu, no need for sorting \n", arr_size); // db
        return;
    }

    Request tempArrAlpha[arr_size], tempArrBravo[arr_size], tempArrCharlie[arr_size];
    int alphaCount = 0, bravoCount = 0, charlieCount = 0;
    MotorDirection otherDir = (moving_dir == DIRN_DOWN) ? DIRN_UP : DIRN_DOWN;

    for (size_t i = 0; i < arr_size; i++)
    {
        Request el = arr[i];
        int floor = el.floor;
        int isStop = (el.button == BUTTON_CAB);
        int buttonUp = (el.button == BUTTON_HALL_UP);
        int buttonDown = (el.button == BUTTON_HALL_DOWN);
        int movingUp = (moving_dir == DIRN_UP);
        int movingDown = (moving_dir == DIRN_DOWN);

        int forward = (movingUp && buttonUp) || (movingDown && buttonDown);
        if (isStop || forward)
        {
            int inFront = (buttonUp && floor > curr_floor) ||
                          (buttonDown && floor < curr_floor) ||
                          (movingUp && isStop && floor > curr_floor) ||
                          (movingDown && isStop && floor < curr_floor) ||
                          (!(in_motion) && floor == curr_floor);
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

    bubble_sort(tempArrAlpha, alphaCount, moving_dir);
    bubble_sort(tempArrBravo, bravoCount, otherDir);
    bubble_sort(tempArrCharlie, charlieCount, moving_dir);

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
        Request req_el = (*arr)[i];
        if (req_el.floor == floor)
        {
            elevio_buttonLamp(req_el.floor, req_el.button, 0);

            // Shift elements left
            for (size_t j = i; j < (*arr_size) - 1; j++)
            {
                (*arr)[j] = (*arr)[j + 1];
            }
            (*arr_size)--;
            i--;
        }
    }

    // if (*capacity > (*arr_size * 2))
    if (*arr_size < *capacity / 4)
    {
        *capacity = (*arr_size > 0) ? *arr_size : 1;
        Request *new_arr = realloc(*arr, (*capacity) * sizeof(Request));

        if (new_arr)
        {
            *arr = new_arr; // Assign only if realloc is successful
        }
        else if (*capacity > 0)
        {
            // Handle reallocation failure (no memory reallocation)
            printf("Memory reallocation failed!\n");
        }
    }
}

bool in_array(Request arr[], size_t size, Request req)
{
    for (int i = 0; i < size; i++)
    {
        if ((arr[i].floor == req.floor) && (arr[i].button == req.button))
        {
            return true;
        }
    }
    return false;
}

void print_request(Request req)
{
    printf("Request: floor %d, button %s\n", req.floor, button_to_string(req.button));
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
        printf("   %zu. ", i);
        print_request(el);
    }
}
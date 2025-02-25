#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

typedef struct
{
    int _floor;
    ButtonType _direction;
} ButtonRequest;

typedef struct
{
    int _currentFloor;
    MotorDirection _movingDirection;
    ButtonRequest _destinationQueue[8];
    size_t _queueSize;
} Elevator;

void swap(ButtonRequest *a, ButtonRequest *b);

int find_floor(ButtonRequest *arr, size_t size, int floor);
void bubble_sort(ButtonRequest *arr, size_t size, MotorDirection dir);
void split_sort(ButtonRequest *arr, size_t size, int threshold);
void sort_queue(Elevator *elevator);
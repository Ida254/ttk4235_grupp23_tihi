#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "elevio.h"
#include <time.h>

typedef struct
{
    int _floor;
    ButtonType _direction;
} ButtonRequest;

typedef struct
{
    int _currentFloor;
    bool _doorOpen;
    bool _validFloor; // not floor 9 3/4
    bool _emergencyStop;
    MotorDirection _movingDirection;
    MotorDirection _motorState;
    ButtonRequest _destinationQueue[8];
    size_t _queueSize;
} Elevator;

void swap(ButtonRequest *a, ButtonRequest *b);

int find_floor(ButtonRequest *arr, size_t size, int floor);
void bubble_sort(ButtonRequest *arr, size_t size, MotorDirection dir);
void split_sort(ButtonRequest *arr, size_t size, int threshold);
void sort_queue(Elevator *elevator);
void moving_elevator(Elevator *elevator);
void at_right_floor(Elevator *elevator);
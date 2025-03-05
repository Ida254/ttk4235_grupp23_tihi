#pragma once

// #include <stdio.h>
// #include <stdbool.h>
#include <string.h>
#include <stdlib.h> // for testing
#include "DestinationRequest.h"
#include "elevio.h"

typedef struct
{
    int _currentFloor;
    MotorDirection _movingDirection;
    ButtonRequest *_destinationQueue;
    size_t _queueSize;
    size_t _queueCapacity;
} Elevator;

void initializeElevator(Elevator *elevator, size_t initialCapacity);
void freeElevator(Elevator *elevator);

void sort_queue(Elevator *elevator);

void addRequestToQueue(Elevator *elevator, int floor, int direction);
void removeRequestFromQueue(Elevator *elevator, int floor);

void test_sort_queue();
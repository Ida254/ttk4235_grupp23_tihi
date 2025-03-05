#pragma once

#include <string.h>
#include <stdlib.h> // for testing
#include "DestinationRequest.h"
#include "elevio.h"

typedef struct
{
    int _currentFloor;
    MotorDirection _movingDirection;
    DestinationRequest *_destinationQueue;
    size_t _queueSize;
    size_t _queueCapacity;
} Elevator;

void initialize_elevator(Elevator *elevator, size_t initialCapacity);
void free_elevator(Elevator *elevator);

void sort_queue(Elevator *elevator);

void add_request_to_queue(Elevator *elevator, int floor, int direction);
void remove_request_from_queue(Elevator *elevator, int floor);

void test_sort_queue();
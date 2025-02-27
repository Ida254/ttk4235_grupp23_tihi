#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // for testing
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
    ButtonRequest *_destinationQueue;
    size_t _queueSize;
    size_t _queueCapacity;
} Elevator;

void initializeElevator(Elevator *elevator, size_t initialCapacity);
void freeElevator(Elevator *elevator);

void swap(ButtonRequest arr[], size_t i, size_t j);
void bubble_sort(ButtonRequest arr[], size_t size, MotorDirection dir);
void sort_queue(Elevator *elevator);

void addRequestToQueue(Elevator *elevator, int floor, int direction);
// void removeRequestFromQueue(Elevator *elevator, int floor, int direction);

void print_queue(ButtonRequest arr[], size_t size);

void test_sort_queue();
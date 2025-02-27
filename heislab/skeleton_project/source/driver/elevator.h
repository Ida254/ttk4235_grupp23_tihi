#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "elevio.h"
#include <stdlib.h> // for testing

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
} Elevator;

void swap(ButtonRequest *a, ButtonRequest *b);
void bubble_sort(ButtonRequest arr[], size_t size, MotorDirection dir);

int find_split_index(ButtonRequest arr[], size_t size, Elevator *elevator);
void split_sort(ButtonRequest arr[], size_t size, Elevator *elevator);
void sort_queue(Elevator *elevator);

void print_queue(ButtonRequest arr[], size_t size);
void test_sort_queue();
#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

typedef struct
{
    int _floor;
    ButtonType _direction;
} DestinationRequest;

void swap(DestinationRequest arr[], size_t i, size_t j);
void bubble_sort(DestinationRequest arr[], size_t size, MotorDirection dir);

void print_queue(DestinationRequest arr[], size_t size);
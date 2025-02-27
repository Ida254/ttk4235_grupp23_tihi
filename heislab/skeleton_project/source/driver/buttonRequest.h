#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

typedef struct
{
    int _floor;
    ButtonType _direction;
} ButtonRequest;

void swap(ButtonRequest arr[], size_t i, size_t j);
void bubble_sort(ButtonRequest arr[], size_t size, MotorDirection dir);
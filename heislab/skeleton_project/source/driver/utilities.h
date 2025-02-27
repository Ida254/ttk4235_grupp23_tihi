#pragma once
#include "elevio.h"
#include "elevator.h"
#include <stdio.h>
#include <stdbool.h>

bool check_in_array(ButtonRequest *btn, ButtonRequest *arr[]);
void button_pressed(Elevator *elevator);
void remove_elements(ButtonRequest *arr, size_t size, int index);
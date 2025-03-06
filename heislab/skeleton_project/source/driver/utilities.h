#pragma once
#include "elevio.h"
#include "elevator.h"
#include <stdio.h>
#include <stdbool.h>

bool check_in_array(DestinationRequest *btn, DestinationRequest *arr[]);
void button_pressed(Elevator *elevator);
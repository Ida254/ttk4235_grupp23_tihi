#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "elevio.h"

typedef struct
{
    int floor;
    ButtonType button;
} Request;

const char *button_to_string(ButtonType button);
const char *motor_direction_to_string(MotorDirection direction);
const char *bool_to_string(bool true_or_false);
MotorDirection int_to_motor_direction(int button);
MotorDirection button_type_to_motor_direction(ButtonType btn);

void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size);
// void remove_requsts();

void bubble_sort(Request *arr, size_t size, MotorDirection direction);
void sort_requests(Request *arr, size_t arr_size, int curr_floor, MotorDirection moving_dir);

void add_request(Request **arr, size_t *arr_size, size_t *capacity, Request req);
void remove_request_by_floor(Request **arr, size_t *arr_size, size_t *capacity, int floor);
bool in_array(Request arr[], size_t size, Request req);

void print_request(Request req);
void print_requests(Request arr[], size_t size);
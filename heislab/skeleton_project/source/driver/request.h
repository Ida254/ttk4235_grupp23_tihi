/**
 * @file
 * @brief The header file for request.c
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "elevio.h"

/**
 * @brief Represents an elevator request.
 *
 * A request consists of a floor number and a button type indicating
 * whether it is an up, down, or command button.
 */
typedef struct
{
    int floor;         /**< The requested floor number. */
    ButtonType button; /**< The type of button pressed (up, down, or command). */
} Request;

/**
 * @brief Converts a ButtonType to a string representation.
 *
 * @param button The button type to convert.
 * @return A string representing the button type.
 */
const char *button_to_string(ButtonType button);

/**
 * @brief Converts a MotorDirection to a string representation.
 *
 * @param direction The motor direction to convert.
 * @return A string representing the motor direction.
 */
const char *motor_direction_to_string(MotorDirection direction);

/**
 * @brief Converts a boolean value to a string ("true" or "false").
 *
 * @param true_or_false Boolean value to convert.
 * @return A string representing "true" or "false".
 */
const char *bool_to_string(bool true_or_false);

/**
 * @brief Converts an integer to a ButtonType.
 *
 * @param integer The integer representing a button type.
 * @return The corresponding ButtonType.
 */
ButtonType int_to_button_type(int integer);

/**
 * @brief Converts an integer to a MotorDirection.
 *
 * @param integer The integer representing a motor direction.
 * @return The corresponding MotorDirection.
 */
MotorDirection int_to_motor_direction(int integer);

/**
 * @brief Maps a button type to the corresponding motor direction.
 *
 * @param btn The button type.
 * @return The motor direction associated with the button.
 */
MotorDirection button_type_to_motor_direction(ButtonType btn);

/**
 * @brief Extends an array of requests by inserting elements from another array.
 *
 * @param arr1 The destination array.
 * @param index The index at which to insert arr2.
 * @param arr2 The source array.
 * @param arr2_size The size of arr2.
 */
void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size);

/**
 * @brief Sorts an array of requests using the bubble sort algorithm.
 *
 * @param arr The array of requests.
 * @param size The size of the array.
 * @param direction The sorting order based on motor direction.
 */
void bubble_sort(Request *arr, size_t size, MotorDirection direction);

/**
 * @brief Sorts elevator requests based on the current floor and direction.
 *
 * @param arr The array of requests.
 * @param arr_size The size of the request array.
 * @param curr_floor The current floor of the elevator.
 * @param moving_dir The current moving direction of the elevator.
 */
void sort_requests(Request *arr, size_t arr_size, int curr_floor, MotorDirection moving_dir);

/**
 * @brief Adds a request to a dynamically allocated request array.
 *
 * Resizes the array if necessary.
 *
 * @param arr Pointer to the request array.
 * @param arr_size Pointer to the current array size.
 * @param capacity Pointer to the array's capacity.
 * @param req The request to add.
 */
void add_request(Request **arr, size_t *arr_size, size_t *capacity, Request req);

/**
 * @brief Removes all requests associated with a specific floor.
 *
 * @param arr Pointer to the request array.
 * @param arr_size Pointer to the array size.
 * @param capacity Pointer to the array's capacity.
 * @param floor The floor to remove requests for.
 */
void remove_request_by_floor(Request **arr, size_t *arr_size, size_t *capacity, int floor);

/**
 * @brief Checks if a request exists in an array.
 *
 * @param arr The request array.
 * @param size The size of the array.
 * @param req The request to check for.
 * @return True if the request is found, false otherwise.
 */
bool in_array(Request arr[], size_t size, Request req);

/**
 * @brief Prints a single request in a human-readable format.
 *
 * @param req The request to print.
 */
void print_request(Request req);

/**
 * @brief Prints all requests in an array.
 *
 * @param arr The request array.
 * @param size The size of the array.
 */
void print_requests(Request arr[], size_t size);
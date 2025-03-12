/**
 * @file
 * @brief Header file for managing elevator requests.
 *
 * This file defines the Request structure and declares functions for handling
 * elevator requests, including adding, removing, sorting, and checking requests.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "elevio.h"
#include "utilities.h"

/**
 * @brief Represents an elevator request.
 *
 * A request consists of a floor number and a button type indicating
 * whether it is an up, down, or cab (inside elevator) button.
 */
typedef struct
{
    int floor;         /**< The requested floor number. */
    ButtonType button; /**< The type of button pressed (BUTTON_UP, BUTTON_DOWN, or BUTTON_CAB). */
} Request;

/**
 * @brief Extends an array of requests by inserting elements from another array.
 *
 * This function inserts elements from one request array (`arr2`) into another (`arr1`)
 * at a specified index. The destination array must have enough allocated space.
 *
 * @param[out] arr1 The destination array to be extended.
 * @param[in] index The index at which to insert `arr2` into `arr1`.
 * @param[in] arr2 The source array containing requests to be inserted.
 * @param[in] arr2_size The number of elements in `arr2`.
 */
void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size);

/**
 * @brief Compares two requests for sorting purposes.
 *
 * Used as a comparator function in sorting algorithms.
 *
 * @param[in] a Pointer to the first request.
 * @param[in] b Pointer to the second request.
 * @return An integer less than, equal to, or greater than zero if `a` is less than,
 *         equal to, or greater than `b`, respectively.
 */
int compare_requests(const void *a, const void *b);

/**
 * @brief Sorts an array of requests using the bubble sort algorithm.
 *
 * Requests are sorted in ascending or descending order based on
 * the elevator's movement direction.
 *
 * @param[in,out] arr The array of requests to be sorted.
 * @param[in] size The number of requests in the array.
 * @param[in] direction The sorting order based on the motor direction (DIRN_UP or DIRN_DOWN).
 */
void bubble_sort(Request *arr, size_t size, MotorDirection direction);

/**
 * @brief Sorts elevator requests based on the current floor and movement direction.
 *
 * Requests are ordered to optimize elevator travel efficiency.
 *
 * @param[in,out] arr The array of requests.
 * @param[in] arr_size The size of the request array.
 * @param[in] curr_floor The current floor of the elevator.
 * @param[in] moving_dir The current moving direction of the elevator.
 * @param[in] in_motion Whether the elevator is currently moving.
 */
void sort_requests(Request *arr, size_t arr_size, int curr_floor, MotorDirection moving_dir, bool in_motion);

/**
 * @brief Adds a new request to a dynamically allocated request array.
 *
 * The array is resized if necessary to accommodate the new request.
 *
 * @param[in,out] arr Pointer to the request array.
 * @param[in,out] arr_size Pointer to the current number of requests in the array.
 * @param[in,out] capacity Pointer to the array's capacity (may be increased if needed).
 * @param[in] req The request to be added.
 */
void add_request(Request **arr, size_t *arr_size, size_t *capacity, Request req);

/**
 * @brief Removes all requests associated with a specific floor.
 *
 * This function scans the request array and removes any requests for the specified floor.
 * If necessary, the array is resized to free unused memory.
 *
 * @param[in,out] arr Pointer to the request array.
 * @param[in,out] arr_size Pointer to the current number of requests in the array.
 * @param[in,out] capacity Pointer to the array's capacity (may be decreased if needed).
 * @param[in] floor The floor number whose requests should be removed.
 */
void remove_request_by_floor(Request **arr, size_t *arr_size, size_t *capacity, int floor);

/**
 * @brief Checks if a specific request exists in an array.
 *
 * @param[in] arr The request array.
 * @param[in] size The number of requests in the array.
 * @param[in] req The request to search for.
 * @return `true` if the request exists in the array, `false` otherwise.
 */
bool in_array(Request arr[], size_t size, Request req);

/**
 * @brief Prints a single request in a human-readable format.
 *
 * The function displays the requested floor and button type.
 *
 * @param[in] req The request to print.
 */
void print_request(Request req);

/**
 * @brief Prints all requests in an array.
 *
 * Displays a list of all requests in the given array.
 *
 * @param[in] arr The request array.
 * @param[in] size The number of requests in the array.
 */
void print_requests(Request arr[], size_t size);
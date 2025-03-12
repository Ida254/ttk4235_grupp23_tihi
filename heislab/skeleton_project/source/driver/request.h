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
 * whether it is an up, down, or command button.
 */
typedef struct
{
    int floor;         /**< The requested floor number. */
    ButtonType button; /**< The type of button pressed (up, down, or command). */
} Request;

/**
 * @brief Extends an array of requests by inserting elements from another array.
 *
 * This function inserts the elements of `arr2` into `arr1` starting at the given `index`.
 *
 * @param arr1 The destination array.
 * @param index The index at which to insert arr2.
 * @param arr2 The source array.
 * @param arr2_size The size of arr2.
 */
void extend_requests(Request *arr1, size_t index, Request *arr2, size_t arr2_size);

int compare_requests(const void *a, const void *b);

/**
 * @brief Sorts an array of requests using the bubble sort algorithm.
 *
 * This function repeatedly compares adjacent elements and swaps them if they are in the wrong order.
 *
 * @param arr The array of requests.
 * @param size The size of the array.
 * @param direction The sorting order based on motor direction (e.g., up or down).
 */
void bubble_sort(Request *arr, size_t size, MotorDirection direction);

/**
 * @brief Sorts elevator requests based on the current floor and direction.
 *
 * This function orders requests based on the elevator’s current floor and moving direction.
 *
 * @param arr The array of requests.
 * @param arr_size The size of the request array.
 * @param curr_floor The current floor of the elevator.
 * @param moving_dir The current moving direction of the elevator.
 * @param in_motion A flag indicating whether the elevator is in motion.
 */
void sort_requests(Request *arr, size_t arr_size, int curr_floor, MotorDirection moving_dir, bool in_motion);

/**
 * @brief Adds a request to a dynamically allocated request array.
 *
 * Resizes the array if necessary. If reallocation fails, the program exits.
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
 * This function shifts all subsequent requests down after removing the requests for the specified floor.
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
 * This function checks if a given request exists in the provided array.
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
 * This function outputs the details of the request (floor and button type) to the console.
 *
 * @param req The request to print.
 */
void print_request(Request req);

/**
 * @brief Prints all requests in an array.
 *
 * This function prints the details of all requests in the array to the console.
 *
 * @param arr The request array.
 * @param size The size of the array.
 */
void print_requests(Request arr[], size_t size);
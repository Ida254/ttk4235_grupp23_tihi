/**
 * @file
 * @brief Header file for DestinationRequest.c.
 */
#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

/**
 * @brief Represents a destination request in the elevator system.
 *
 * Represents a request for an elevator stop, including the target floor and travel direction.
 */
typedef struct
{
    int _floor;             ///< The requested floor.
    ButtonType _buttonType; ///< The direction of travel (@c DIRN_UP, @c DIRN_DOWN, or @c DIRN_STOP).
} DestinationRequest;

/**
 * @brief Swaps two elements in a @c DestinationRequest array.
 * @param[in,out] arr The array of @c DestinationRequest elements.
 * @param[in] i The index of the first element to swap.
 * @param[in] j The index of the second element to swap.
 */
void swap(DestinationRequest arr[], size_t i, size_t j);

/**
 * @brief Sorts a @c DestinationRequest array in the specified direction.
 *
 * If @p dir is @c DIRN_UP, the array is sorted in ascending order (lowest to highest).
 * If @p dir is @c DIRN_DOWN, the array is sorted in descending order (highest to lowest).
 *
 * @param[in,out] arr The array of @c DestinationRequest elements to be sorted.
 * @param[in] size The number of elements in the array.
 * @param[in] dir The @c MotorDirection that determines the sorting order.
 */
void bubble_sort(DestinationRequest arr[], size_t size, MotorDirection dir);

bool in_array(DestinationRequest arr[], size_t size, DestinationRequest destinationRequest);

MotorDirection int_to_motor_direction(int button);
MotorDirection button_type_to_motor_direction(ButtonType btn);

/**
 * @brief Prints a @c DestinationRequest array for debugging purposes.
 * @param[in] arr The array of @c DestinationRequest elements to be printed.
 * @param[in] size The number of elements in the array.
 */
void print_queue(DestinationRequest arr[], size_t size);
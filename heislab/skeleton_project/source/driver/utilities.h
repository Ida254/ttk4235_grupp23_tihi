/**
 * @file
 * @brief Utility functions for conversions and mappings in the elevator system.
 *
 * This header file provides functions to convert between different data types
 * used in the elevator system, such as buttons, motor directions, and boolean values.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

/**
 * @brief Converts a ButtonType to a string representation.
 *
 * This function takes a ButtonType value and converts it to a corresponding string
 * that represents the button type (e.g., "BUTTON_CAB", "BUTTON_UP").
 *
 * @param[in] button The button type to convert.
 * @return A string representing the button type.
 */
const char *button_to_string(ButtonType button);

/**
 * @brief Converts a MotorDirection to a string representation.
 *
 * This function takes a MotorDirection value and returns a string corresponding
 * to the direction, such as "DIRN_UP" or "DIRN_DOWN".
 *
 * @param[in] direction The motor direction to convert.
 * @return A string representing the motor direction.
 */
const char *motor_direction_to_string(MotorDirection direction);

/**
 * @brief Converts a boolean value to a string ("true" or "false").
 *
 * This function takes a boolean value and returns a string representation of it.
 * It returns either "true" or "false" based on the input value.
 *
 * @param[in] true_or_false The boolean value to convert.
 * @return A string representing "true" or "false".
 */
const char *bool_to_string(bool true_or_false);

/**
 * @brief Converts an integer to a ButtonType.
 *
 * This function converts an integer value into a corresponding ButtonType.
 * The integer should represent one of the predefined button types (e.g., 0 for BUTTON_CAB).
 *
 * @param[in] integer The integer representing a button type.
 * @return The corresponding ButtonType.
 */
ButtonType int_to_button_type(int integer);

/**
 * @brief Converts an integer to a MotorDirection.
 *
 * This function converts an integer value into a corresponding MotorDirection.
 * The integer should represent one of the predefined motor directions (e.g., 0 for DIRN_UP).
 *
 * @param[in] integer The integer representing a motor direction.
 * @return The corresponding MotorDirection.
 */
MotorDirection int_to_motor_direction(int integer);

/**
 * @brief Maps a button type to the corresponding motor direction.
 *
 * This function maps a given button type (up, down, or command) to the motor direction
 * that corresponds to that button type. For example, the "UP" button maps to the DIRN_UP direction.
 *
 * @param[in] btn The button type to map.
 * @return The motor direction associated with the button.
 */
MotorDirection button_type_to_motor_direction(ButtonType btn);
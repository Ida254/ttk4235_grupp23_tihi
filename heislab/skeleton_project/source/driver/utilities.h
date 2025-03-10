#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "elevio.h"

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
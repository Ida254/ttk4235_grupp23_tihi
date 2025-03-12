/**
 * @file
 * @brief Header file for elevator control threads.
 *
 * This file declares thread functions for handling button presses, floor monitoring,
 * and emergency stop detection in the elevator system.
 */

#pragma once

#include <pthread.h>
#include "elevator.h"

/**
 * @brief Listens for button presses and updates the elevator queue accordingly.
 *
 * This function runs in a separate thread, continuously checking for button presses.
 * When a button is pressed, it adds the corresponding request to the elevator's queue.
 *
 * @param[in] arg Pointer to the Elevator structure, which will be updated with button requests.
 * @return Always returns NULL.
 */
void *button_listener(void *arg);

/**
 * @brief Monitors the elevator's current floor and handles stop conditions.
 *
 * This function runs in a separate thread, continuously checking if the elevator
 * has reached the correct floor and handling necessary actions, such as stopping the elevator
 * or opening doors when the right floor is reached.
 *
 * @param[in] arg Pointer to the Elevator structure that needs to be monitored.
 * @return Always returns NULL.
 */
void *floor_listener(void *arg);

/**
 * @brief Continuously checks for an emergency stop condition.
 *
 * This function runs in a separate thread and monitors if an emergency stop has been triggered.
 * If an emergency stop is detected, it handles stopping the elevator safely and initiates
 * appropriate safety measures.
 *
 * @param[in] arg Unused parameter (can be NULL).
 * @return Always returns NULL.
 */
void *emergency_listener(void *arg);

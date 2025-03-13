/**
 * @file
 * @brief Header file for the elevator control system.
 *
 * This file defines the Elevator structure and declares functions to control
 * and manage elevator movement, request handling, and system states.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include <time.h>   // for time
#include <signal.h> // for kill
#include <unistd.h>
#include <pthread.h>
#include "elevio.h"
#include "utilities.h"
#include "request.h"

#define SLEEP_TIME_NS 10 * 1000 * 1000
#define BOTTOM_FLOOR 0
#define TOP_FLOOR 3
#define INITIAL_FLOOR TOP_FLOOR
#define INITIAL_DIRECTION DIRN_DOWN
#define INITIAL_REQUEST \
    (Request) { BOTTOM_FLOOR, BUTTON_CAB }

static pthread_mutex_t elevator_mtx;

/**
 * @brief Structure representing the elevator's state.
 *
 * This struct holds information about the elevator, including its current floor,
 * movement direction, motor state, queue of requested destinations, and various flags.
 */
typedef struct
{
    int current_floor;               /**< The current floor of the elevator. */
    int last_floor;                  /**< The last registered floor of the elevator. */
    MotorDirection moving_direction; /**< The direction the elevator is moving (UP, DOWN, or STOP). */
    MotorDirection motor_state;      /**< The current state of the motor (UP, DOWN, or STOP). */
    bool in_motion;                  /**< Indicates if the elevator is currently moving. */
    Request *request_queue;          /**< The queue storing floor requests. */
    size_t queue_size;               /**< The current number of elements in the request queue. */
    size_t queue_capacity;           /**< The maximum number of elements the queue can hold. */
    bool initialized;                /**< Indicates whether the elevator has been initialized. */
    bool is_stopped;                 /**< Indicates if the elevator is currently stopped. */
} Elevator;

/**
 * @brief Initializes an Elevator struct with default values.
 *
 * This function sets up an elevator by initializing its internal state and setting it to the default floor.
 *
 * @param[in,out] elevator Pointer to the Elevator struct to be initialized.
 */
void initialize_elevator(Elevator *elevator);

/**
 * @brief Frees memory allocated for the Elevator struct.
 *
 * This function releases any dynamically allocated memory used by the elevator structure
 * to prevent memory leaks.
 *
 * @param[in,out] elevator Pointer to the Elevator struct to be freed.
 */
void free_elevator(Elevator *elevator);

/**
 * @brief Turns off all elevator button lamps.
 *
 * This function ensures that all floor and cab call button lamps are turned off.
 */
void turn_off_all_lamps();

/**
 * @brief Sorts the request queue in the Elevator struct.
 *
 * This function organizes the queue of floor requests in a logical order to
 * optimize the elevator's movement.
 *
 * @param[in,out] elevator Pointer to the Elevator struct whose queue needs to be sorted.
 */
void sort_queue(Elevator *elevator);

/**
 * @brief Adds a floor request to the Elevator's destination queue.
 *
 * This function adds a new floor request to the elevator's queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] new_req The new floor request to be added to the queue.
 */
void add_request_to_queue(Elevator *elevator, Request new_req);

/**
 * @brief Removes a floor request from the Elevator's destination queue.
 *
 * This function removes all occurrences of a specified floor from the queue
 * and shifts remaining elements forward to maintain queue structure.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] floor The floor number to remove from the queue.
 */
void remove_request_from_queue(Elevator *elevator, int floor);

/**
 * @brief Clears all requests from the elevator queue.
 *
 * This function removes all pending floor requests, effectively resetting the queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void empty_queue(Elevator *elevator);

/**
 * @brief Handles button press events and updates the elevator queue.
 *
 * This function checks if any button has been pressed and, if so, adds the request
 * to the elevator's queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void on_button_press(Elevator *elevator);

/**
 * @brief Checks if any elevator button has been pressed.
 *
 * Iterates through all floors and button types to detect a pressed button.
 * If a button press is found, it updates the given request structure.
 *
 * @return A pointer to a Request struct if a button was pressed, NULL otherwise.
 */
Request *button_pressed();

/**
 * @brief Checks if the elevator is at the requested floor.
 *
 * If the elevator reaches a requested floor, it performs necessary actions like
 * stopping and opening doors.
 *
 * @param[in] elevator Pointer to the Elevator structure.
 * @return True if the elevator is at the requested floor, false otherwise.
 */
bool at_right_floor(Elevator *elevator);

/**
 * @brief Determines the new motor direction based on requests.
 *
 * This function decides whether the elevator should move up, down, or stop based on the request queue.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 * @return The new motor direction (UP, DOWN, or STOP).
 */
MotorDirection get_new_motor_direction(Elevator *elevator);

/**
 * @brief Resets the elevator to an idle state.
 *
 * This function stops the elevator and clears any pending requests.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void rest_elevator(Elevator *elevator);

/**
 * @brief Prints the current state of the elevator.
 *
 * This function outputs the values of the elevator struct to help with debugging.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 */
void print_elevator(Elevator *elevator);
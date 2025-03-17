/**
 * @file elevator.h
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

/** @brief Sleep time in nanoseconds (10ms). */
#define SLEEP_TIME_NS 10 * 1000 * 1000
/** @brief Defines the lowest floor number. */
#define BOTTOM_FLOOR 0
/** @brief Defines the highest floor number. */
#define TOP_FLOOR 3
/** @brief The initial floor where the elevator starts. */
#define INITIAL_FLOOR TOP_FLOOR
/** @brief The default direction when initializing the elevator. */
#define INITIAL_DIRECTION DIRN_DOWN
/** @brief The default initial request assigned to the elevator. */
#define INITIAL_REQUEST \
    (Request) { BOTTOM_FLOOR, BUTTON_CAB }

/** @brief Mutex for protecting access to the Elevator struct. */
static pthread_mutex_t elevator_mtx;

/**
 * @struct Elevator
 * @brief Represents an elevator and its state.
 *
 * This structure holds information about the elevator, including its
 * current floor, direction, and request queue.
 */
typedef struct
{
    int current_floor;               /**< The current floor of the elevator. */
    int last_floor;                  /**< The last floor the elevator was on. */
    MotorDirection moving_direction; /**< The current moving direction (UP/DOWN). */
    MotorDirection motorState;       /**< The current state of the motor. */
    MotorDirection last_motorState;  /**< The previous state of the motor. */
    bool in_motion;                  /**< Whether the elevator is currently moving. */
    Request *request_queue;          /**< Array storing requested floors. */
    size_t queue_size;               /**< Current number of requests in the queue. */
    size_t queue_capacity;           /**< Maximum number of requests that can be stored. */
    bool initialized;                /**< Whether the elevator has been initialized. */
    bool is_stopped;                 /**< Whether the elevator is currently stopped. */
} Elevator;

/**
 * @brief Initializes an Elevator struct with default values.
 *
 * Moves the elevator to the initial floor and sets default parameters.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void initialize_elevator(Elevator *elevator);

/**
 * @brief Frees memory allocated for the Elevator struct.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void free_elevator(Elevator *elevator);

/** @brief Turns off all lamps in the elevator system. */
void turn_off_all_lamps();

/**
 * @brief Sorts the elevator's request queue in a logical order.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void sort_queue(Elevator *elevator);

/**
 * @brief Adds a floor request to the elevator's queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] new_req The floor request to be added.
 */
void add_request_to_queue(Elevator *elevator, Request new_req);

/**
 * @brief Removes a specific floor request from the queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] floor The floor number to be removed.
 */
void remove_request_from_queue(Elevator *elevator, int floor);

/**
 * @brief Clears all requests from the queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void empty_queue(Elevator *elevator);

/**
 * @brief Handles button press events and updates the elevator queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void on_button_press(Elevator *elevator);

/**
 * @brief Detects if any button has been pressed.
 *
 * @return Pointer to a Request struct if a button was pressed, otherwise NULL.
 */
Request *button_pressed();

/**
 * @brief Moves the elevator based on its queued requests.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void moving_elevator(Elevator *elevator);

/**
 * @brief Checks if the elevator is at a requested floor.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 * @return True if the elevator is at a requested floor, false otherwise.
 */
bool at_right_floor(Elevator *elevator);

/**
 * @brief Switches the direction of the elevator when needed.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void switch_direction(Elevator *elevator);

/**
 * @brief Resets the elevator to an idle state.
 *
 * Stops the elevator and clears any pending requests.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 */
void rest_elevator(Elevator *elevator);

/**
 * @brief Checks if the emergency stop condition is active.
 *
 * If an emergency stop is detected, the elevator will halt immediately.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 * @return True if emergency stop is active, false otherwise.
 */
bool is_emergency_stop(Elevator *elevator);

/**
 * @brief Prints the state of an Elevator struct.
 *
 * This function prints the values of an elevator in a structured format for debugging.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 */
void print_elevator(Elevator *elevator);

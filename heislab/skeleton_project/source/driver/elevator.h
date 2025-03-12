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
 * @brief Defines an Elevator struct to store values.
 *
 * This struct holds information about an elevator, including its current floor,
 * movement direction, and a queue of requested destinations.
 *
 * @param current_floor The current floor of the elevator.
 * @param moving_direction The current movement direction, which can be either
 *        @c DIRN_UP or @c DIRN_DOWN.
 * @param in_motion Tell whether or not the elevator is in motion at the current moment.
 * @param request_queue A queue of destination requests that helps the elevator
 *        determine where to go next.
 * @param queue_size The current number of elements in the destination queue.
//  * @param queue_capacity The maximum number of destination requests the queue can hold.
 */
typedef struct
{
    int current_floor;
    int last_floor;
    MotorDirection moving_direction;
    MotorDirection motor_state;
    bool in_motion;
    Request *request_queue;
    size_t queue_size;
    size_t queue_capacity;
    bool initialized;
    bool is_stopped;
} Elevator;

/**
 * @brief Initializes an Elevator struct with default values.
 *
 * This function sets up an elevator and moves it to the first floor.
 *
 * @param[in,out] elevator Pointer to the Elevator struct to be initialized.
 * @param[in] initialCapacity The initial capacity of the destination queue.
 */
void initialize_elevator(Elevator *elevator);

/**
 * @brief Frees memory allocated for the Elevator struct.
 *
 * This function releases any allocated memory within the elevator structure
 * to prevent memory leaks.
 *
 * @param[in,out] elevator Pointer to the Elevator struct to be freed.
 */
void free_elevator(Elevator *elevator);

void turn_off_all_lamps();

/**
 * @brief Sorts the destination queue in the Elevator struct.
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
 * This function adds a new floor request to the elevator's queue, specifying
 * the direction in which the elevator should move after reaching the floor.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] new_req The direction request to be added to the queue.
 */
void add_request_to_queue(Elevator *elevator, Request new_req);

/**
 * @brief Removes a floor request from the Elevator's destination queue.
 *
 * This function removes all occurrences of a specified floor from the queue
 * and shifts remaining elements forward to fill the gap.
 *
 * @param[in,out] elevator Pointer to the Elevator struct.
 * @param[in] floor The floor number to remove from the queue.
 */
void remove_request_from_queue(Elevator *elevator, int floor);

void empty_queue(Elevator *elevator);

/**
 * @brief Handles button press events and updates the elevator queue.
 *
 * This function checks if any button has been pressed and, if so, adds the request
 * to the elevator's queue.
 *
 * @param elevator Pointer to the Elevator structure.
 */
void on_button_press(Elevator *elevator);

/**
 * @brief Checks if any elevator button has been pressed.
 *
 * Iterates through all floors and button types to detect a pressed button.
 * If a button press is found, it updates the given request structure.
 *
 * @param req Pointer to a Request structure to store the button press details.
 * @return True if a button was pressed, false otherwise.
 */
Request *button_pressed();

/**
 * @brief Moves the elevator according to its queue and direction.
 *
 * This function ensures that the elevator moves towards its intended destination
 * based on the queued requests.
 *
 * @param elevator Pointer to the Elevator structure.
 */
void moving_elevator(Elevator *elevator);

/**
 * @brief Checks if the elevator is at the correct floor and handles stopping logic.
 *
 * If the elevator reaches a requested floor, it performs necessary actions like
 * stopping and opening doors.
 *
 * @param elevator Pointer to the Elevator structure.
 * @return True or false depending on whether you are on the right floor or not.
 */
bool at_right_floor(Elevator *elevator);

/**
 * @brief Switches the moving direction of the elevator when needed.
 *
 * This function updates the elevator's movement direction based on its current
 * position and queued requests.
 *
 * @param elevator Pointer to the Elevator structure.
 */
MotorDirection switch_direction(Elevator *elevator);

/**
 * @brief Resets the elevator to an idle state.
 *
 * Stops the elevator and clears any pending requests.
 *
 * @param elevator Pointer to the Elevator structure.
 */
void rest_elevator(Elevator *elevator);

// /**
//  * @brief Monitors and handles emergency stop conditions.
//  *
//  * If an emergency stop is detected, the elevator stops immediately and takes
//  * appropriate safety measures.
//  */
// void check_emergency_stop(Elevator *elevator);

/**
 * @brief Monitors emergency stop conditions.
 *
 * If an emergency stop is detected, the elevator stops immediately and takes
 * appropriate safety measures.
 */
bool is_emergency_stop(Elevator *elevator);

/**
 * @brief Prints the memebers of the struct Elevator.
 *
 * This function prints the values of an elevator in a nice way that helps with debuging of the code.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 */
void print_elevator(Elevator *elevator);
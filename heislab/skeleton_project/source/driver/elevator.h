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
#include "elevio.h"
#include "utilities.h"
#include "request.h"

#define BOTTOM_FLOOR 0
#define TOP_FLOOR 3
#define INITIAL_FLOOR 0
#define INITIAL_DIRECTION DIRN_UP
// #define INITIAL_REQUEST \
//     (Request) { 0, BUTTON_HALL_UP }

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
    bool in_motion;
    Request *request_queue;
    size_t queue_size;
    size_t queue_capacity;
    // bool initialized;
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
bool button_pressed(Request *req);

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
 */
void at_right_floor(Elevator *elevator);

/**
 * @brief Switches the moving direction of the elevator when needed.
 *
 * This function updates the elevator's movement direction based on its current
 * position and queued requests.
 *
 * @param elevator Pointer to the Elevator structure.
 */
void switch_direction(Elevator *elevator);

/**
 * @brief Resets the elevator to an idle state.
 *
 * Stops the elevator and clears any pending requests.
 *
 * @param elevator Pointer to the Elevator structure.
 */
void rest_elevator(Elevator *elevator);

/**
 * @brief Stops the elevator when it reaches a requested floor.
 *
 * This function handles stopping the elevator and opening its doors.
 *
 * @param elevator Pointer to the Elevator structure.
 * @param floor The floor at which the elevator should stop.
 */
void stop_elevator_at_floor(Elevator *elevator, int floor);

/**
 * @brief Monitors and handles emergency stop conditions.
 *
 * If an emergency stop is detected, the elevator stops immediately and takes
 * appropriate safety measures.
 */
void check_emergency_stop(Elevator *elevator);

/**
 * @brief Prints the memebers of the struct Elevator.
 *
 * This function prints the values of an elevator in a nice way that helps with debuging of the code.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 */
void print_elevator(Elevator *elevator);
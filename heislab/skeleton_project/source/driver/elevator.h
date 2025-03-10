/**
 * @file
 * @brief The header file for elevator.c
 */

#pragma once

#define BOTTOM_FLOOR 0
#define TOP_FLOOR 3 // db, should be 3
#define INITIAL_FLOOR 0
#define INITIAL_DIRECTION DIRN_UP

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // for testing
#include <time.h>
#include <signal.h>
#include <unistd.h>
// #include "DestinationRequest.h"
#include "request.h"
#include "elevio.h"

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
    MotorDirection moving_direction;
    bool in_motion;
    Request *request_queue;
    size_t queue_size;
    size_t queue_capacity;
    int last_floor;
} Elevator;

/**
 * @brief Initializes an Elevator struct with default values.
 *
 * This function sets up an elevator with the specified initial capacity for its destination queue.
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

void on_button_press(Elevator *elevator);

// void move_elevator_to_floor(Elevator *elevator, Request destinationRequest);

void on_button_press(Elevator *elevator);
void moving_elevator(Elevator *elevator);
void at_right_floor(Elevator *elevator);

void switch_direction(Elevator *elevator);
void rest_elevator(Elevator *elevator);
void stop_elevator_at_floor(Elevator *elevator, int floor);
void check_emergency_stop();

/**
 * @brief Prints the memebers of the struct Elevator.
 *
 * This function prints the values of an elevator in a nice way that helps with debuging of the code.
 *
 * @param[in] elevator Pointer to the Elevator struct.
 */
void print_elevator(Elevator *elevator);

/**
 * @brief Runs a test for the sorting function.
 *
 * This function tests whether the elevator's queue sorting mechanism works correctly.
 */
void test_elevator();

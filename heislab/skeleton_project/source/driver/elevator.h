/**
 * @file
 * @brief The header file for elevator.c
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // for testing
#include "DestinationRequest.h"
#include "elevio.h"
#include <time.h>
#include <signal.h>
#include <unistd.h>

/**
 * @brief Defines an Elevator struct to store values.
 *
 * This struct holds information about an elevator, including its current floor,
 * movement direction, and a queue of requested destinations.
 *
 * @param _currentFloor The current floor of the elevator.
 * @param _movingDirection The current movement direction, which can be either
 *        @c DIRN_UP or @c DIRN_DOWN.
 * @param _destinationQueue A queue of destination requests that helps the elevator
 *        determine where to go next.
 * @param _queueSize The current number of elements in the destination queue.
 * @param _queueCapacity The maximum number of destination requests the queue can hold.
 */
typedef struct
{
    int _currentFloor;
    bool _doorOpen;
    bool _validFloor; // not floor 9 3/4
    bool _emergencyStop;
    MotorDirection _movingDirection;
    MotorDirection _motorState;
    DestinationRequest *_destinationQueue;
    size_t _queueSize;
    size_t _queueCapacity;
} Elevator;

/**
 * @brief Initializes an Elevator struct with default values.
 *
 * This function sets up an elevator with the specified initial capacity for its destination queue.
 *
 * @param[in,out] elevator Pointer to the Elevator struct to be initialized.
 * @param[in] initialCapacity The initial capacity of the destination queue.
 */
void initialize_elevator(Elevator *elevator, size_t initialCapacity);

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
 * @param[in] floor The floor number to be added to the queue.
 * @param[in] direction The direction the elevator should move after reaching the floor.
 *                      It can be either @c DIRN_UP, @c DIRN_DOWN, or @c DIRN_STOP.
 */
void add_request_to_queue(Elevator *elevator, int floor, int direction); // make floor and dir into DestinationRequest, Ida

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

void moving_elevator(Elevator *elevator);
void at_right_floor(Elevator *elevator);

/**
 * @brief Runs a test for the sorting function.
 *
 * This function tests whether the elevator's queue sorting mechanism works correctly.
 */
void test_sort_queue();

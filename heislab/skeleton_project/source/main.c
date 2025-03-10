/**
 * @file
 * @brief The main file of the elevator application
 */
#include "driver/elevio.h"
#include "driver/elevator.h"

int main()
{
    Elevator elevator;
    run_elevator_program(&elevator);

    return 0;
}
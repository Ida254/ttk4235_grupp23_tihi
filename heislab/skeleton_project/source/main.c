/**
 * @file
 * @brief The main file of the elevator application
 */
#include "driver/elevio.h"
#include "driver/elevator.h"

void run_elevator_program(Elevator *elevator)
{
    elevio_init();
    printf("=== Cool Program ===\n");
    initialize_elevator(elevator);

    while (1)
    {
        int floor = elevio_floorSensor();
        elevator->current_floor = floor;
        at_right_floor(elevator);

        on_button_press(elevator); // execute if button is pressed, and add to queue

        at_right_floor(elevator);

        check_emergency_stop();

        nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL);
    }

    free_elevator(elevator);
}

int main()
{
    Elevator elevator;
    run_elevator_program(&elevator);

    return 0;
}
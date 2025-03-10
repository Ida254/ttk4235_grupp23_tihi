/**
 * @file
 * @brief The main file of the elevator application
 */
#include "driver/elevio.h"
#include "driver/elevator.h"

void run_elevator_program(Elevator *elevator)
{
    while (1)
    {
        int floor = elevio_floorSensor();
        elevator->current_floor = floor;

        // if (floor1 == 0)
        // {
        //     elevio_motorDirection(DIRN_UP);
        // }

        // if(floor == N_FLOORS-1){
        //     elevio_motorDirection(DIRN_DOWN);
        // }
        at_right_floor(elevator);

        on_button_press(elevator); // execute if button is pressed, and add to queue

        at_right_floor(elevator);

        // if (elevio_obstruction())
        // {
        //     elevio_stopLamp(1);
        // }
        // else
        // {
        //     elevio_stopLamp(0);
        // }

        if (elevio_stopButton())
        {
            elevio_motorDirection(DIRN_STOP);
            kill(getpid(), SIGKILL); // Forcefully stops the program
        }

        nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL);
    }
}

int main()
{
    elevio_init();

    // printf("=== Example Program ===\n");
    // printf("Press the stop button on the elevator panel to exit\n");

    // elevio_motorDirection(DIRN_UP);
    printf("=== Cool Program ===\n");
    Elevator elevator; // Need to define it more
    // size_t queueCapacaty = 10;
    initialize_elevator(&elevator);
    printf("Done with initializing\n");
    print_elevator(&elevator);
    run_elevator_program(&elevator);

    free_elevator(&elevator);
    return 0;
}
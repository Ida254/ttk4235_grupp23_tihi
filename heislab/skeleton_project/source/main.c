/**
 * @file
 * @brief The main file of the elevator application
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/utilities.h"

run_elevator_program(Elevator *elevator);

int main()
{
    elevio_init();

    // printf("=== Example Program ===\n");
    // printf("Press the stop button on the elevator panel to exit\n");

    // elevio_motorDirection(DIRN_UP);

    Elevator elevator; // Need to define it more
    size_t queueCapacaty = 8;
    initialize_elevator(&elevator, queueCapacaty);
    run_elevator_program(&elevator);

    return 0;
}

void run_elevator_program(Elevator *elevator){
    while(1){
        int floor = elevio_floorSensor();
        elevator._currentFloor = floor;

        if (floor == 0)
        {
            elevio_motorDirection(DIRN_UP);
        }

        // if(floor == N_FLOORS-1){
        //     elevio_motorDirection(DIRN_DOWN);
        // }

        button_pressed(&elevator);  //execute if button is pressed, and add to queue

        if(!elevator._doorOpen){
            at_right_floor(&elevator);            
        }

        if (elevio_obstruction())
        {
            elevio_stopLamp(1);
        }
        else
        {
            elevio_stopLamp(0);
        }

        if (elevio_stopButton())
        {
            elevio_motorDirection(DIRN_STOP);
            kill(getpid(), SIGKILL);  // Forcefully stops the program
        }

        nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL);
    }
}
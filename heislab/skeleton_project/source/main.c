/**
 * @file
 * @brief The main file of the elevator application
 */
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/threads.h"

void run_elevator_program(Elevator *elevator)
{
    elevio_init();
    printf("=== Cool Program ===\n");
    initialize_elevator(elevator);

    // while (1)
    // {
    //     elevator->current_floor = elevio_floorSensor();
    //     at_right_floor(elevator);

    //     on_button_press(elevator); // execute if button is pressed, and add to queue

    //     at_right_floor(elevator);

    //     check_emergency_stop();

    //     nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL);
    // }

    pthread_t button_thread, floor_thread, emergency_thread;

    // Create threads for handling different tasks simultaneously
    pthread_create(&button_thread, NULL, button_listener, (void *)elevator);
    pthread_create(&floor_thread, NULL, floor_listener, (void *)elevator);
    pthread_create(&emergency_thread, NULL, emergency_listener, NULL);

    while (1)
    {
        elevator->current_floor = elevio_floorSensor();           // Continuously update the floor sensor
        nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL); // Sleep for 20ms
    }

    pthread_join(button_thread, NULL);
    pthread_join(floor_thread, NULL);
    pthread_join(emergency_thread, NULL);

    free_elevator(elevator);
}

int main()
{
    Elevator elevator;
    run_elevator_program(&elevator);

    return 0;
}
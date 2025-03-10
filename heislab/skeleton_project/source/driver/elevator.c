/**
 * @file
 * @brief The implementation file of elevator.h
 */

#include "elevator.h"

void initialize_elevator(Elevator *elevator)
{
    if (elevator == NULL)
    {
        printf("Elevator pointer is NULL!\n");
        exit(1);
    }

    elevator->current_floor = INITIAL_FLOOR;
    elevator->moving_direction = INITIAL_DIRECTION;
    elevator->in_motion = false;
    elevator->request_queue = NULL;
    elevator->queue_size = 0;
    elevator->queue_capacity = 0;
    elevator->last_floor = 0;

    // move to BOTTOM_FLOOR
    int floor = elevio_floorSensor();
    if (floor == BOTTOM_FLOOR)
    {
        return;
    }

    elevio_motorDirection(DIRN_DOWN);
    while (floor != BOTTOM_FLOOR)
    {
        floor = elevio_floorSensor();
    }
    elevio_motorDirection(DIRN_STOP);
    elevator->current_floor = floor;

    printf("Elevator initialized \n"); // db
    print_elevator(elevator);
}

void free_elevator(Elevator *elevator)
{
    free(elevator->request_queue);
    elevator->request_queue = NULL;
}

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

void *button_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    while (1)
    {
        on_button_press(elevator);                                // Check if a button is pressed and update queue
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *floor_listener(void *arg)
{
    Elevator *elevator = (Elevator *)arg;
    while (1)
    {
        at_right_floor(elevator);                                 // Check if the elevator should stop at a floor
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void *emergency_listener(void *arg)
{
    while (1)
    {
        check_emergency_stop();                                   // Constantly monitor if the emergency stop is triggered
        nanosleep(&(struct timespec){0, 10 * 1000 * 1000}, NULL); // Sleep for 10ms
    }
    return NULL;
}

void sort_queue(Elevator *elevator)
{
    if (!elevator)
    {
        printf("Error: Elevator pointer is NULL!\n");
        return;
    }

    size_t queueSize = elevator->queue_size;
    int currFloor = elevator->current_floor;
    MotorDirection currMovingDir = elevator->moving_direction;
    sort_requests(elevator->request_queue, queueSize, currFloor, currMovingDir);
}

void add_request_to_queue(Elevator *elevator, Request new_req)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    if (in_array(elevator->request_queue, *queueSize, new_req))
    {
        // printf("New req already in the queue \n   ");        // db
        // print_request(new_req);                              // db
        // print_requests(elevator->request_queue, *queueSize); // db
        return;
    }
    add_request(&elevator->request_queue, queueSize, queueCapacity, new_req);
    // print_elevator(elevator);

    sort_queue(elevator);

    // printf("\nAdded to queue and sorted\n"); // db
    // print_elevator(elevator); // db
}

void remove_request_from_queue(Elevator *elevator, int floor)
{
    size_t *queueSize = &elevator->queue_size;
    size_t *queueCapacity = &elevator->queue_capacity;

    remove_request_by_floor(&elevator->request_queue, queueSize, queueCapacity, floor);
}

void on_button_press(Elevator *elevator)
{
    Request new_req;

    if (button_pressed(&new_req))
    {
        elevio_buttonLamp(new_req.floor, new_req.button, 1);

        add_request_to_queue(elevator, new_req);
    }
}

bool button_pressed(Request *req)
{
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int btn = 0; btn < N_BUTTONS; btn++)
        {
            if (elevio_callButton(floor, btn))
            {
                req->floor = floor;
                req->button = btn;
                return true;
            }
        }
    }

    return false;
}

void moving_elevator(Elevator *elevator)
{
    if (elevator->queue_size == 0 || elevator->in_motion)
    {
        return;
    }

    int destinationFloor = elevator->request_queue[0].floor;

    int differenceInFloors = destinationFloor - elevator->current_floor;
    MotorDirection direction = (differenceInFloors > 0) ? DIRN_UP : (differenceInFloors < 0) ? DIRN_DOWN
                                                                                             : DIRN_STOP;

    elevator->in_motion = (direction != DIRN_STOP);
    elevio_motorDirection(direction);
}

void at_right_floor(Elevator *elevator)
{
    if (elevator->queue_size == 0)
    {
        return;
    }

    int currFloor = elevio_floorSensor();

    if (currFloor != -1)
    {
        elevator->current_floor = currFloor;
        elevio_floorIndicator(currFloor);
    }

    if (currFloor == elevator->request_queue[0].floor)
    {
        stop_elevator_at_floor(elevator, currFloor);
        elevio_doorOpenLamp(0);
    }

    moving_elevator(elevator);
}

void switch_direction(Elevator *elevator)
{
    if (elevator->queue_size == 0)
    {
        return;
    }

    MotorDirection *currDir = &elevator->moving_direction;
    MotorDirection newDir = button_type_to_motor_direction(elevator->request_queue[0].button);

    if (*currDir == newDir || newDir == DIRN_STOP)
    {
        return;
    }

    *currDir = newDir;
    sort_queue(elevator);
    printf("\nSwitched direction \n");
}

void rest_elevator(Elevator *elevator)
{
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3)
    {
        on_button_press(elevator);
        check_emergency_stop();
    }
}

void stop_elevator_at_floor(Elevator *elevator, int floor)
{
    elevio_motorDirection(DIRN_STOP);
    elevator->in_motion = false;
    elevio_buttonLamp(floor, elevator->request_queue[0].button, 0);
    switch_direction(elevator);

    remove_request_from_queue(elevator, floor);
    elevio_doorOpenLamp(1);

    rest_elevator(elevator);

    printf("Destination reached and elevator stopped \n");
    print_elevator(elevator); // db
}

void check_emergency_stop()
{
    if (elevio_stopButton())
    {
        elevio_motorDirection(DIRN_STOP);
        kill(getpid(), SIGKILL); // Forcefully stops the program
    }
}

void print_elevator(Elevator *elevator)
{
    printf("Elevator \n");
    printf("Current floor: %d\n", elevator->current_floor);
    printf("Current moving direction: %s\n", motor_direction_to_string(elevator->moving_direction));
    printf("In motion: %s\n", bool_to_string(elevator->in_motion));

    if (elevator->queue_size > 0)
    {
        print_requests(elevator->request_queue, elevator->queue_size);
    }
    else
    {
        printf("No requests in the queue.\n");
    }

    printf("Queue size: %zu\n", elevator->queue_size);
    printf("Queue capacity: %zu\n\n", elevator->queue_capacity);
}

void test_elevator(void)
{
    Elevator elevator;
    initialize_elevator(&elevator);
    printf("Initial elevator \n");
    print_elevator(&elevator);

    // size_t initial_requests_size = 13;
    // Request initial_requests[] = {
    //     {5, BUTTON_HALL_UP},
    //     {5, BUTTON_CAB},
    //     {5, BUTTON_HALL_DOWN},
    //     {3, BUTTON_HALL_UP},
    //     {8, BUTTON_HALL_DOWN},
    //     {2, BUTTON_HALL_UP},
    //     {1, BUTTON_HALL_UP},
    //     {7, BUTTON_HALL_DOWN},
    //     {3, BUTTON_HALL_DOWN},
    //     {6, BUTTON_CAB},
    //     {8, BUTTON_CAB},
    //     {10, BUTTON_HALL_UP},
    //     {2, BUTTON_CAB}};

    size_t initial_requests_size = 3;
    Request initial_requests[] = {{3, BUTTON_HALL_DOWN}, {2, BUTTON_HALL_UP}, {1, BUTTON_HALL_UP}};

    for (size_t i = 0; i < initial_requests_size; i++)
    {
        add_request_to_queue(&elevator, initial_requests[i]);
    }
    printf("\nAfter sorting \n");
    print_elevator(&elevator);

    // sort_queue(&elevator);

    // printf("\nAfter sorting, before removing \n");
    // print_elevator(&elevator);

    // remove_request_from_queue(&elevator, 5);
    // remove_request_from_queue(&elevator, 2);
    // remove_request_from_queue(&elevator, 3);

    // printf("\nAfter removing ");
    // print_elevator(&elevator);
    //     // gcc -o elevator_program source/driver/elevator.c source/driver/request.c source/driver/elevio.c
    //     // ./elevator_program
    //     // rm ./elevator_program
}

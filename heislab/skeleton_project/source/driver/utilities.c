#include "utilities.h"

bool check_in_array(DestinationRequest *btn, DestinationRequest *arr[])
{
    size_t size = sizeof(arr) / sizeof(arr);
    for (int i = 0; i < size; i++)
    {
        if (btn == arr[i])
        {
            return true;
        }
    }
    return false;
}

void button_pressed(Elevator *elevator)
{
    MotorDirection dirRequest;
    DestinationRequest btn;
    int btnPressed;
    bool inQueue = 0;
    for (int f = 0; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            btnPressed = elevio_callButton(f, b);
            printf("BtnPressed = %d \n", btnPressed);
            elevio_buttonLamp(f, b, btnPressed);
            if (btnPressed)
            {
                switch (b)
                {
                case 0:
                    dirRequest = DIRN_UP;
                    break;
                case 1:
                    dirRequest = DIRN_DOWN;
                    break;
                default:
                    dirRequest = DIRN_STOP;
                    break;
                }
                btn._floor = f;
                btn._direction = dirRequest;
                if (elevator->_queueSize != 0) // db, there are an error here ... check out
                {
                    printf("What");
                    inQueue = check_in_array(elevator->_destinationQueue, &btn);
                }
                // bool inQueue = check_in_array(&btn, elevator->_destinationQueue);
                if (!inQueue)
                {
                    add_request_to_queue(elevator, f, btnPressed);
                    remove_request_from_queue(elevator, f);
                }
            }
        }
    }
}
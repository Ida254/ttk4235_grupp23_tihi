#include "utilities.h"


bool check_in_array(DestinationRequest *btn, DestinationRequest *arr[]){
    size_t size = sizeof(arr)/sizeof(arr);
    for (int i = 0; i < size; i++){
        if (btn == arr[i]){
            return true;
        }
    }
    return false;
}

void button_pressed(Elevator *elevator){
    for (int f = 0; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            int btnPressed = elevio_callButton(f, b);
            elevio_buttonLamp(f, b, btnPressed);
            motorDirection dirRequest;
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
            DestinationRequest btn = {f, dirRequest};
            bool inQueue = check_in_array(elevator->_destinationQueue, &btn);
            // bool inQueue = check_in_array(&btn, elevator->_destinationQueue);
            if (!inQueue)
            {
                add_request_to_queue(elevator, f, btnPressed);
                remove_request_from_queue(elevator, f);
            }
        }
    }
}
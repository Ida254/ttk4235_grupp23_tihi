#include "utilities.h"


bool check_in_array(ButtonRequest *btn, ButtonRequest *arr[]){
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
            ButtonRequest btn = {f, btnPressed};
            bool inQueue = check_in_array(&btn, elevator->_destinationQueue);
            if (!inQueue)
            {
                elevator->_destinationQueue[elevator->_queueSize-1] = btn;  //replace this
                sort_queue(elevator);  //Ida fix
            }
        }
    }
}
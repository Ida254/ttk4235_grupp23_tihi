#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "uart.h"
#include <sys/types.h>

#define BUTTON_1 13
#define BUTTON_2 14
#define BUTTON_3 15
#define BUTTON_4 16
#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20

void button_init()
{
    GPIO->PIN_CNF[BUTTON_1] = (3 << 2);
    GPIO->PIN_CNF[BUTTON_2] = (3 << 2);
}

int main()
{
    int sleep = 0;

    button_init();
    uart_init();

    // for (int i = 17; i <= 20; i++)
    // {
    //     GPIO->DIRSET = (1 << i); // LED on
    //     GPIO->OUTCLR = (1 << i);
    // }

    // char letter;

    while (1)
    {
        int buttonOnePressed = !(GPIO->IN & (1 << BUTTON_1));
        int buttonTwoPressed = !(GPIO->IN & (1 << BUTTON_2));

        if (buttonOnePressed)
        {
            iprintf("Halla");
            uart_send('A');
        }
        if (buttonTwoPressed)
        {
            uart_send('B');
        }

        // letter = uart_read();
        // if (letter != '\0'){
        //     if (!GPIO->IN & (1 << ))
        // }

        sleep = 10000;
        while (--sleep)
            ;
    }

    return 0;
}
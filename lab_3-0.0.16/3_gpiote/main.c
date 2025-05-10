#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"
#include <sys/types.h>

#define BUTTON_1 13
#define BUTTON_2 14
#define BUTTON_3 15
#define BUTTON_4 16
#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20

#define DELAY_LOOP_CONSTANT 16000

void leds_init()
{
    for (int i = LED_1; i <= LED_4; i++)
    {
        GPIO->PIN_CNF[i] = 1;
        GPIO->DIRSET = (1 << i);
        GPIO->OUTSET = (1 << i);
    }
}

void button_init()
{
    GPIO->PIN_CNF[BUTTON_1] = (3 << 2);
    GPIO->PIN_CNF[BUTTON_2] = (3 << 2);
}

void gpiote_init(void)
{
    GPIOTE->CONFIG[1] = (3 << 0) | (LED_1 << 8) | (3 << 16) | (0 << 20);
    GPIOTE->CONFIG[2] = (3 << 0) | (LED_2 << 8) | (3 << 16) | (0 << 20);
    GPIOTE->CONFIG[3] = (3 << 0) | (LED_3 << 8) | (3 << 16) | (0 << 20);
    GPIOTE->CONFIG[4] = (3 << 0) | (LED_4 << 8) | (3 << 16) | (0 << 20);
    GPIOTE->CONFIG[0] = (1 << 0) | (BUTTON_1 << 8) | (2 << 16) | (0 << 20);
}

void ppi_int(void)
{
    PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->TASKS_OUT[1]);

    PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->TASKS_OUT[2]);

    PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->TASKS_OUT[3]);

    PPI->PPI_CH[3].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[3].EEP = (uint32_t)&(GPIOTE->TASKS_OUT[4]);

    PPI->CHENSET = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
}

void delay_ms(volatile unsigned int ms)
{
    volatile unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < DELAY_LOOP_CONSTANT; j++)
        {
            __asm__("nop");
        }
    }
}

int main()
{
    leds_init();
    button_init();
    gpiote_init();
    ppi_int();

    while (1)
    {
        __asm__("nop");
    }

    return 0;
}
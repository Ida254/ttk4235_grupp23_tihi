// #include <stdint.h>
// #include <stdio.h>

// #define GPIO ((NRF_GPIO_REGS *)0x50000000)
// #define BUTTON_1 13
// #define BUTTON_2 14
// #define BUTTON_3 15
// #define BUTTON_4 16
// #define LED_1 17
// #define LED_2 18
// #define LED_3 19
// #define LED_4 20

// typedef struct
// {
// 	volatile uint32_t RESERVED0[321];
// 	volatile uint32_t OUT;
// 	volatile uint32_t OUTSET;
// 	volatile uint32_t OUTCLR;
// 	volatile uint32_t IN;
// 	volatile uint32_t DIR;
// 	volatile uint32_t DIRSET;
// 	volatile uint32_t DIRCLR;
// 	volatile uint32_t LATCH;
// 	volatile uint32_t DETECTMODE;
// 	volatile uint32_t RESERVED1[370];
// 	volatile uint32_t PIN_CNF[32];
// } NRF_GPIO_REGS;

// void button_init()
// {
// 	GPIO->PIN_CNF[BUTTON_1] = (3 << 2);
// 	// Fill inn the configuration for the remaining buttons
// 	GPIO->PIN_CNF[BUTTON_2] = (3 << 2);
// 	GPIO->PIN_CNF[BUTTON_3] = (3 << 2);
// 	GPIO->PIN_CNF[BUTTON_4] = (3 << 2);
// }

// int main()
// {
// 	char buffer[50];
// 	// Configure LED Matrix
// 	for (int i = 17; i <= 20; i++)
// 	{
// 		GPIO->DIRSET = (1 << i); // LED on
// 		GPIO->OUTCLR = (1 << i);
// 	}

// 	// Configure buttons -> see button_init()
// 	button_init();

// 	sprintf(buffer, "in main \n");
// 	int sleep = 0;
// 	while (1)
// 	{

// 		/* Check if button 1 is pressed;
// 		 * turn on LED matrix if it is. */
// 		int buttonOnePressed = !(GPIO->IN & (1 << BUTTON_1));
// 		int buttonTwoPressed = !(GPIO->IN & (1 << BUTTON_2));

// 		if (buttonOnePressed)
// 		{
// 			sprintf(buffer, "inside funciton \n");
// 			for (int i = LED_1; i <= LED_4; i++)
// 			{
// 				GPIO->OUTCLR &= ~(1 << i);
// 			}
// 			// GPIO->OUTCLR = (1 << LED_1);
// 		}

// 		if (buttonTwoPressed)
// 		{
// 			sprintf(buffer, "inside funciton \n");
// 			for (int i = LED_1; i <= LED_4; i++)
// 			{
// 				GPIO->OUTSET |= (1 << i);
// 			}
// 		}

// 		/* Check if button 2 is pressed;
// 		 * turn off LED matrix if it is. */

// 		sleep = 10000;
// 		while (--sleep)
// 			; // Delay
// 	}
// 	return 0;
// }
#include <stdint.h>
#include <stdio.h>

#define GPIO ((NRF_GPIO_REGS *)0x50000000)
#define BUTTON_1 14
#define BUTTON_2 15
#define BUTTON_3 16
#define BUTTON_4 17
#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20

typedef struct
{
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t LATCH;
	volatile uint32_t DETECTMODE;
	volatile uint32_t RESERVED1[370];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;

void button_init()
{
	GPIO->PIN_CNF[BUTTON_1] = (1 << 1) | (1 << 2); // Input with pull-up
	GPIO->PIN_CNF[BUTTON_2] = (1 << 1) | (1 << 2); // Input with pull-up
	GPIO->PIN_CNF[BUTTON_3] = (1 << 1) | (1 << 2); // Input with pull-up
	GPIO->PIN_CNF[BUTTON_4] = (1 << 1) | (1 << 2); // Input with pull-up
}

int main()
{
	// Configure LED Matrix
	for (int i = LED_1; i <= LED_4; i++)
	{
		GPIO->DIRSET = (1 << i); // Set as output
		GPIO->OUTCLR = (1 << i); // Turn off LEDs
	}

	// Configure buttons
	button_init();

	int sleep = 0;
	while (1)
	{
		// Check if button 1 is pressed
		int buttonOnePressed = !(GPIO->IN & (1 << BUTTON_1));
		int buttonTwoPressed = !(GPIO->IN & (1 << BUTTON_2));

		if (buttonOnePressed)
		{
			for (int i = LED_1; i <= LED_4; i++)
			{
				GPIO->OUTSET &= ~(1 << i); // Turn on LEDs
			}
		}

		if (buttonTwoPressed)
		{
			for (int i = LED_1; i <= LED_4; i++)
			{
				GPIO->OUTCLR |= (1 << i); // Turn off LEDs
			}
		}

		// Delay
		sleep = 10000;
		while (--sleep)
			;
	}
	return 0;
}
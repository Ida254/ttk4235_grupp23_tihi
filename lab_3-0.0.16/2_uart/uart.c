#include "uart.h"

#define __NOP() __asm__ volatile("nop")

void uart_init()
{
    GPIO->DIRSET = (1 << TX_PIN); // Set TX pin as output
    GPIO->DIRCLR = (1 << RX_PIN); // Set RX pin as input

    UART->PSELTXD = TX_PIN;
    UART->PSELRXD = RX_PIN;

    UART->BAUDRATE = 0x002580; // 9600 baud rate

    UART->PSELCTS = 0xFFFFFFFF; // Disable CTS
    UART->PSELRTS = 0xFFFFFFFF; // Disable RTS
    UART->ENABLE = 0x4;         // Enable UART
    UART->TASK_STARTRX = 1;     // Start RX
}

void uart_send(char letter)
{
    UART->EVENTS_TXDRDY = 0; // Clear TX ready event
    UART->TASK_STARTTX = 1; // Start TX

    UART->TXD = letter;      // Send the letter

    while (UART->EVENTS_TXDRDY == 0){
        __NOP(); // Wait for TX ready
    }

    UART->EVENTS_TXDRDY = 0; // Clear TX ready event
    UART->TASKS_STOPTX = 1; //Stopper TX

    char buffer[50];
    sprintf(buffer, "%c", letter);
    printf("Letter: %s\n", buffer);

    // while (UART->EVENTS_TXDRDY == 0)
    //     ; // Wait for TX ready

    // UART->EVENTS_TXDRDY = 0; // Clear TX ready event
    // UART->TASK_STOPTX = 1;   // Stop TX
}

char uart_read()
{
    while (UART->EVENTS_RXDRDY == 0)
        ;                                  // Wait for RX ready
    UART->EVENTS_RXDRDY = 0;               // Clear RX ready event
    char letter = UART->RXD;               // Read the letter
    return '\0' ? letter == '\0' : letter; // Return the letter
}
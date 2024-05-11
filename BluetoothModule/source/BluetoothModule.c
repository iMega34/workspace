
#include <stdio.h>
#include "board.h"
#include "MKL25Z4.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"

#include "UART.h"
#include "LCD.h"
#include "timer.h"

int main(void) {
    char input[64];

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    TPM0_init();
    LCD_init();
    InitUART2Pins();
    UART2_init();

    LCD_print_1st_line("Bluetooth Module");
    UART_SendString("¡Hola desde FRDM-KL25Z!\r\n");

    while (1) {
        memset(input, 0, sizeof(input));
        UART_ReadString(input, sizeof(input));
        LCD_print_2nd_line(input);
    }

    return 0;
}

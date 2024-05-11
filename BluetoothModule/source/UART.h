
#pragma once

#include <stdio.h>
#include "board.h"
#include "MKL25Z4.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#define UART UART2
#define UART_CLK_FREQ CLOCK_GetFreq(UART2_CLK_SRC)

/*
 * Inicializa el módulo UART2
 * 
 * LA configuración del módulo UART2 es la siguiente:
 * - Baud rate de 9600
 * - 8 bits de datos
 * - 1 bit de parada
 * - Sin paridad
 * - Sin control de flujo
*/
void UART2_init() {
    uart_config_t config;
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 9600;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART, &config, UART_CLK_FREQ);
}

/*
 * Inicializa los pines del módulo UART2
 * 
 * Asigna los pines del puerto E22 y E23 como UART2_TX y UART2_RX, respectivamente.
*/
void InitUART2Pins() {
    // Habilita el reloj para el puerto E
	CLOCK_EnableClock(kCLOCK_PortE);

    // Configura los pines 22 y 23 del puerto E como UART2_TX y UART2_RX
	PORTE->PCR[22] = PORT_PCR_MUX(4);
	PORTE->PCR[23] = PORT_PCR_MUX(4);
}

/*
 * Envía una cadena de caracteres a través del módulo UART
 *
 * @param str Cadena de caracteres a enviar
*/
void UART_SendString(const char *str) {
    // Recorre la cadena de caracteres
    while (*str) {
        // Espera a que el buffer de transmisión esté listo
        UART_WriteBlocking(UART, (uint8_t *)str++, 1);
    }
}

/*
 * Lee una cadena de caracteres a través del módulo UART
 *
 * @param buffer Buffer donde se almacenará la cadena de caracteres leída
*/
void UART_ReadString(char *buffer, size_t buffer_size) {
    uint8_t input;
    int i = 0;

    // Lee los caracteres hasta que se presione Enter
    while (1) {
        // Espera a que haya un dato disponible en el buffer de recepción
        UART_ReadBlocking(UART2, &input, 1);
        // Si se presiona la tecla Enter, se termina la cadena
        if (input == '\r') break;
        // El caracter de nueva línea se ignora
        else if (input == '\n') continue;
        // Si se presiona la tecla Backspace, se elimina el último caracter
        else if (input == '\b') { if (i > 0) i--; }
        // Si se presiona cualquier otra tecla, se almacena en el buffer
        else { if (i < buffer_size - 1) buffer[i++] = input; }
    }

    // Se agrega el caracter nulo al final de la cadena
    buffer[i] = '\0';
}

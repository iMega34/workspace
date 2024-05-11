
#include <MKL25Z4.h>

/*
 * Inicializa el LED RGB de la tarjeta
*/
void RGB_LED_init() {
    // Habilita el reloj para el puerto B
    SIM -> SCGC5 |= 0x0400;
    // Configura los pines 18, 19 y 1 del puerto B como GPIO
    PORTB -> PCR[18] = 0x100;
    PORTB -> PCR[19] = 0x100;
    PORTB -> PCR[1] = 0x100;
    // Configura los pines 18, 19 y 1 del puerto B como salida
    PTB -> PDDR |= 0x40000;
    PTB -> PDDR |= 0x80000;
    PTB -> PDDR |= 0x02;
}

/*
 * Enciende el LED rojo
*/
void RED_LED_on() {
    PTB -> PDOR &= ~0x40000;
    PTB -> PDOR |= 0x80000;
    PTD -> PDOR |= 0x02;
}

/*
 * Enciende el LED verde
*/
void GREEN_LED_on() {
    PTB -> PDOR |= 0x40000;
    PTB -> PDOR &= ~0x80000;
    PTD -> PDOR |= 0x02;
}

/*
 * Enciende el LED azul
*/
void BLUE_LED_on() {
    PTB -> PDOR |= 0x40000;
    PTB -> PDOR |= 0x80000;
    PTD -> PDOR &= ~0x02;
}

/*
 * Apaga el LED RGB
*/
void LED_off() {
    PTB -> PDOR |= 0x40000;
    PTB -> PDOR |= 0x80000;
    PTD -> PDOR |= 0x02;
}

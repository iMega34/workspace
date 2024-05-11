
#include <MKL25Z4.h>

/*
 * Inicializa el TPM0
*/
void TPM0_init() {
    SIM -> SCGC5 |= 0x1000;
    PORTD -> PCR[1] = 0x100;
    PTD -> PDDR |= 0x02;

    SIM -> SCGC6 |= 0x01000000;
    SIM -> SOPT2 |= 0x01000000;
    TPM0 -> SC = 0;
    TPM0 -> MOD = 0xFFFF;
    TPM0 -> SC |= 0x80;
    TPM0 -> SC |= 0x08;
}

/*
 * Delay en base a segundos
*/
void delaySec(uint32_t seconds) {
    for (int i = 0; i < 320 * seconds; i++) {
        while ((TPM0 -> SC & 0x80) == 0);
        TPM0->SC |= 0x80;
    }
}

/*
 * Delay en base a milisegundos
*/
void delayMs(uint32_t ms) {
    // Cálculo aproximado de ciclos de CPU por milisegundo
    uint32_t cycles = (48000000  / 1000) * ms;

    while (cycles > 0) {
        __asm volatile ("NOP"); // Instrucción que no hace nada pero consume tiempo
        cycles--;
    }
}

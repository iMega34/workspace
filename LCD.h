
#include <MKL25Z4.h>

#define RS 0x04 /* PTA2 mask */
#define RW 0x10 /* PTA4 mask */
#define EN 0x20 /* PTA5 mask */

/*
 * Inicializa la pantalla LCD

 * Asigna los pines del puerto D0-D7 como GPIO y salida, al igual que los pines A2, A4 y A5.
 * Realiza la secuencia de inicialización de la pantalla LCD, al habilitar el modo de 8 bits,
 * 2 líneas y 5x7, mover el cursor a la derecha, limpiar la pantalla y encenderla.
*/
void LCD_init() {
    // Habilta el reloj para el puerto D
	SIM -> SCGC5 |= 0x1000;
    // Configura los pines 0-7 del puerto D como GPIO
	PORTD -> PCR[0] = 0x100;
	PORTD -> PCR[1] = 0x100;
	PORTD -> PCR[2] = 0x100;
	PORTD -> PCR[3] = 0x100;
	PORTD -> PCR[4] = 0x100;
	PORTD -> PCR[5] = 0x100;
	PORTD -> PCR[6] = 0x100;
	PORTD -> PCR[7] = 0x100;
    // Configura los pines 0-7 del puerto D como salida
	PTD -> PDDR = 0xFF;
    // Habilta el reloj para el puerto A
	SIM -> SCGC5 |= 0x0200;
    // Configura los pines 2, 4 y 5 del puerto A como GPIO
	PORTA -> PCR[2] = 0x100;
	PORTA -> PCR[4] = 0x100;
	PORTA -> PCR[5] = 0x100;
    // Configura los pines 2, 4 y 5 del puerto A como salida
	PTA -> PDDR |= 0x34;

    // Secuencia de inicialización
	delayMs(20);
	LCD_command_noWait(0x30);
	delayMs(5);
	LCD_command_noWait(0x30);
	delayMs(1);
	LCD_command_noWait(0x30);
    // Modo 8 bits, 2 líneas, 5x7
	LCD_command(0x38);
    // Mueve el cursor a la derecha
	LCD_command(0x06);
    // Limpia la pantalla
	LCD_command(0x01);
    // Enciende la pantalla
	LCD_command(0x0F);
}

/*
 * Espera a que la pantalla LCD esté lista para recibir un comando o dato
*/
void LCD_ready() {
    char status;
    // Configura los pines 0-7 del puerto D como entrada
	PTD -> PDDR = 0;
    // RS = 0, es decir, se envía un comando
	PTA -> PCOR = RS;
    // RW = 1, es decir, una operación de lectura
	PTA -> PSOR = RW;

    // Se queda en el ciclo hasta que la pantalla no esté ocupada
	do {
        // Levanta el pin de habilitación
		PTA -> PSOR = EN;
		delayMs(0);
        // Se guarda el estado del puerto D
		status = PTD -> PDIR;
        // Baja el pin de habilitación
		PTA -> PCOR = EN;
		delayMs(0);
	} while (status & 0x80); // Checa el bit de ocupado

    // RW = 0, es decir, una operación de escritura
	PTA -> PCOR = RW;
    // Configura los pines 0-7 del puerto D como salida
	PTD -> PDDR = 0xFF;
}

/*
 * Envía un comando a la pantalla LCD
*/
void LCD_command(unsigned char command) {
    // Espera a que la pantalla esté lista
	LCD_ready();
    // Asigna a RS el valor de 0 y a RW el valor de 0.
    // Es decir, se envía un comando de escritura
	PTA -> PCOR = RS | RW;
	PTD -> PDOR = command;
    // Habilitación y deshabilitación
	PTA -> PSOR = EN;
	delayMs(0);
	PTA -> PCOR = EN;
}

/*
 * Envía un comando a la pantalla LCD sin esperar a que esté lista
*/
void LCD_command_noWait(unsigned char command) {
    // Asigna a RS el valor de 0 y a RW el valor de 0.
    // Es decir, se envía un comando de escritura
	PTA -> PCOR = RS | RW;
	PTD -> PDOR = command;
	PTA -> PSOR = EN;
	delayMs(0);
	PTA -> PCOR = EN;
}

/*
 * Envía un dato a la pantalla LCD
*/
void LCD_data(unsigned char data) {
    // Espera a que la pantalla esté lista
	LCD_ready();
    // Asigna a RS el valor de 1 y a RW el valor de 0.
    // Es decir, se envía un dato, en este caso un caracter
	PTA -> PSOR = RS;
	PTA -> PCOR = RW;
    // Se envía el caracter
	PTD -> PDOR = data;
	PTA -> PSOR = EN;
	delayMs(0);
	PTA -> PCOR = EN;
}

/*
 * Muestra un texto en la primera línea de la pantalla LCD
*/
void LCD_print_1st_line(char* str) {
    // Mueve el cursor a al inicio de la primera línea
    LCD_command(0x80);
	// Limpia la línea
	for (int i = 0; i < 16; i++) { LCD_data(' '); }
    LCD_command(0x80);
    // Recorre el buffer de caracteres y los muestra en la pantalla
    while (*str) { LCD_data(*str++); }
}

/*
 * Muestra un texto en la segunda línea de la pantalla LCD
*/
void LCD_print_2nd_line(char* str) {
    // Mueve el cursor a al inicio de la segunda línea
    LCD_command(0xC0);
	// Limpia la línea
	for (int i = 0; i < 16; i++) { LCD_data(' '); }
    LCD_command(0xC0);
    // Recorre el buffer de caracteres y los muestra en la pantalla
    while (*str) { LCD_data(*str++); }
}

/*
 * Limpia la pantalla LCD
*/
void LCD_clear() {
    // Envía el comando para limpiar la pantalla
    LCD_command(0x01);
}

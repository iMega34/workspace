
#include <MKL25Z4.h>

char teclas[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

/*
 * Inicializa el teclado matricial
*/
void keypad_init() {
    // Habilita el reloj para el puerto C
	SIM -> SCGC5 |= 0x0800;
    // Configura los pines 0-7 del puerto C como GPIO
	PORTC -> PCR[0] = 0x103;
	PORTC -> PCR[1] = 0x103;
	PORTC -> PCR[2] = 0x103;
	PORTC -> PCR[3] = 0x103;
	PORTC -> PCR[4] = 0x103;
	PORTC -> PCR[5] = 0x103;
	PORTC -> PCR[6] = 0x103;
	PORTC -> PCR[7] = 0x103;
    // Configura los pines 0-7 del puerto C como entrada
	PTD -> PDDR = 0x0F;
}

/*
 * Obtiene la tecla presionada
*/
int keypad_getkey(void) {
	int row, col;
    // Máscara de bits para las filas
    // 0x01 -> 0001 -> Primera fila
    // 0x02 -> 0010 -> Segunda fila
    // 0x04 -> 0100 -> Tercera fila
    // 0x08 -> 1000 -> Cuarta fila
	const char row_select[] = {0x01, 0x02, 0x04, 0x08};

    // Habilita las filas
	PTC -> PDDR |= 0x0F;
    // Asigna las filas en bajo, es decir, las habilita para recibir señal
	PTC -> PCOR = 0x0F;
	delayMs(2);
    // Lee las columnas y guarda el valor de la columna que se presionó.
    // Esto lo logra aplicando una máscara de bits al PDIR del puerto C y obtener el valor de los
    // 4 bits menos significativos, si una columna se presionó, el valor será diferente de 0xF0
	col = PTC -> PDIR & 0xF0;
    // Deshabilita las filas
	PTC -> PDDR = 0;
    // Si no se presionó ninguna tecla, es decir, el valor de las columnas es 0xF0, regresa 0
	if (col == 0xF0)
		return -1;

	for (row = 0; row < 4; row++) {
        // Deshabilita las filas
		PTC -> PDDR = 0;
        // Habilita una fila
		PTC -> PDDR |= row_select[row];
        // Asigna la fila en bajo, es decir, la habilita para recibir señal
		PTC -> PCOR = row_select[row];
		delayMs(2);
        // Lee las columnas y guarda el valor de la columna que se presionó.
        // Esto lo logra aplicando una máscara de bits al PDIR del puerto C y obtener el valor de los
        // 4 bits menos significativos, si una columna se presionó, el valor será diferente de 0xF0
		col = PTC -> PDIR & 0xF0;
        // Si se presionó una tecla, se sale del ciclo
		if (col != 0xF0) break;
	}

    // Deshabilita las filas
	PTC -> PDDR = 0;
    // Si no se presionó ninguna tecla, regresa 0
	if (row == 4)
		return -1;

    // Si se presionó una tecla, se revisa en qué columna fue y se regresa el valor de la tecla al que corresponde
    // al multiplicar la fila por 4 y sumarle la columna
    // Primer columna -> 0xE0
	if (col == 0xE0) return row * 4 + 0;
    // Segunda columna -> 0xD0
	if (col == 0xD0) return row * 4 + 1;
    // Tercera columna -> 0xB0
	if (col == 0xB0) return row * 4 + 2;
    // Cuarta columna -> 0x70
	if (col == 0x70) return row * 4 + 3;
}

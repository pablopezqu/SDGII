#include "teclado.h"

TipoTeclado teclado;
int flags=0;
//int flags_juego=0;
int debounceTime[NUM_ROWS] = {0,0,0,0}; // Timeout to avoid bouncing after pin event

char teclado4x4[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

/**
 * Función que comprueba si se ha activado el flag de timeout. Este tiempo define el máximo tiempo que se excita una columna
 */

int CompruebaColumnTimeout() {
	int result=0;
	piLock(FLAG_KEY);
	result = (flags & FLAG_TMR_TIMEOUT);
	piUnlock(FLAG_KEY);
	return result;
}


void row_1_isr (void){
	if(millis () < debounceTime[ROW_1]){	//millis-> tiempo que lleva corriendo el programa.
		debounceTime[ROW_1] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(FLAG_KEY);

	teclado.teclaPulsada.row = ROW_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;		//Activas el flag de que se ha pulsado una tecla

	piUnlock(FLAG_KEY);

	debounceTime[ROW_1] = millis () + DEBOUNCE_TIME;
}

void row_2_isr (void){
	if(millis () < debounceTime[ROW_2]){	//millis-> tiempo que lleva corriendo el programa.
		debounceTime[ROW_2] = millis () + DEBOUNCE_TIME;
		return;
	}
	piLock(FLAG_KEY);

	teclado.teclaPulsada.row = ROW_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;		//Activas el flag de que se ha pulsado una tecla

	piUnlock(FLAG_KEY);

	debounceTime[ROW_2] = millis () + DEBOUNCE_TIME;
}

void row_3_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_3]) {
		debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_3;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
}

void row_4_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_4]) {
		debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_4;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
}

void col_1 (fsm_t* this){
	TipoTeclado* p_teclado;
	p_teclado = (TipoTeclado*) (this->user_data);

	piLock(FLAG_KEY);

	digitalWrite(GPIO_COL_1,HIGH);	//Excitas columna que interesa
	digitalWrite(GPIO_COL_2,LOW);
	digitalWrite(GPIO_COL_3,LOW);
	digitalWrite(GPIO_COL_4,LOW);

	flags &= (~FLAG_TMR_TIMEOUT);	//Desactivas flag de timeout

	p_teclado->columna_actual = COL_1;

	piUnlock(FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna),COL_REFRESH_TIME);	//Inicias8 el temporizador. Cuando acabe este temporizador, se excitará otra col.
}

void col_2 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_1, LOW);
	digitalWrite (GPIO_COL_2, HIGH);
	digitalWrite (GPIO_COL_3, LOW);
	digitalWrite (GPIO_COL_4, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_2;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_3 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_2, LOW);
	digitalWrite (GPIO_COL_3, HIGH);
	digitalWrite (GPIO_COL_4, LOW);
	digitalWrite (GPIO_COL_1, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_3;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_4 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_3, LOW);
	digitalWrite (GPIO_COL_4, HIGH);
	digitalWrite (GPIO_COL_1, LOW);
	digitalWrite (GPIO_COL_2, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_4;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

/*
 * Esta función comprueba si se ha pulsado una tecla y según eso activa el flag
 */

int key_pressed(fsm_t* this){
	int result = 0;

	piLock (FLAG_KEY);
	result = (flags & FLAG_KEY_PRESSED);
	piUnlock (FLAG_KEY);

	return result;
}

void process_key (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	flags &= (~FLAG_KEY_PRESSED);	//Desactiva el flag de que se ha pulsado una tecla para atender a este evento

	switch(p_teclado->teclaPulsada.col){		//Según la columna que se ha pulsado, planteamos varios casos
		case COL_1:
		case COL_2:
		case COL_3:
		case COL_4:
			if(teclado4x4[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='D'){
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_DISPARO;
				piUnlock(PLAYER_FLAGS_KEY);
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_TRIGGER_BUTTON;
				piUnlock(SYSTEM_FLAGS_KEY);
			}
			else if(teclado4x4[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='5'){
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_IMPACTO;
				piUnlock(PLAYER_FLAGS_KEY);
			}
			else if(teclado4x4[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='0'){
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_DOWN;
				piUnlock(SYSTEM_FLAGS_KEY);
			}

			printf("\nKeypress \"%c\"...\n",
					teclado4x4[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
			fflush(stdout);
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}

	piUnlock (FLAG_KEY);

}

/*
 * Función de atención a la interrupción que produce el temporizador al acabar indicando de que se debe pasar a explorar otra col.
 */
void timer_duracion_columna_isr (union sigval value) {
	piLock (FLAG_KEY);
	flags |= FLAG_TMR_TIMEOUT;
	piUnlock (FLAG_KEY);
}

int initialize(TipoTeclado *p_teclado) {
	if (wiringPiSetupGpio() < 0) {
	    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
	    return 1 ;
	}

	// Comenzamos excitacion por primera columna
	p_teclado->columna_actual = COL_1;

	// Inicialmente no hay tecla pulsada
	p_teclado->teclaPulsada.col = -1;
	p_teclado->teclaPulsada.row = -1;
	pinMode (GPIO_ROW_1, INPUT);
	pullUpDnControl(GPIO_ROW_1, PUD_DOWN);
	wiringPiISR (GPIO_ROW_1, INT_EDGE_RISING, row_1_isr);

	pinMode (GPIO_ROW_2, INPUT);
	pullUpDnControl(GPIO_ROW_2, PUD_DOWN);
	wiringPiISR (GPIO_ROW_2, INT_EDGE_RISING, row_2_isr);

	pinMode (GPIO_ROW_3, INPUT);
	pullUpDnControl(GPIO_ROW_3, PUD_DOWN);
	wiringPiISR (GPIO_ROW_3, INT_EDGE_RISING, row_3_isr);

	pinMode (GPIO_ROW_4, INPUT);
	pullUpDnControl(GPIO_ROW_4, PUD_DOWN);
	wiringPiISR (GPIO_ROW_4, INT_EDGE_RISING, row_4_isr);

	pinMode (GPIO_COL_1, OUTPUT);
	digitalWrite (GPIO_COL_1, HIGH);

	pinMode (GPIO_COL_2, OUTPUT);
	digitalWrite (GPIO_COL_2, LOW);

	pinMode (GPIO_COL_3, OUTPUT);
	digitalWrite (GPIO_COL_3, LOW);

	pinMode (GPIO_COL_4, OUTPUT);
	digitalWrite (GPIO_COL_4, LOW);

	p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);

	printf("\nSystem init complete! keypad ready to process the code!!!\n");
	fflush(stdout);

	return 0;
}

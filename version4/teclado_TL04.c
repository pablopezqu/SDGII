#include "teclado_TL04.h"
#include "piTankGoLib.h"
 TipoTeclado teclado;
 int flags_teclado = 0;
int debounceTime[NUM_ROWS] = {0,0,0,0}; // Timeout to avoid bouncing after pin event

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

int CompruebaColumnTimeout (fsm_t* this) {
	int result = 0;
	piLock (FLAG_KEY);
	result = (flags_teclado & FLAG_TMR_TIMEOUT);
	piUnlock (FLAG_KEY);
	return result;
}

 void row_1_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_1]) {
		debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
}

void row_2_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_2]) {
		debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
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

	flags_teclado |= FLAG_KEY_PRESSED;

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

	flags_teclado |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
}

void col_1 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_4, LOW);
	digitalWrite (GPIO_COL_1, HIGH);
	digitalWrite (GPIO_COL_2, LOW);
	digitalWrite (GPIO_COL_3, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_1;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

 void col_2 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_1, LOW);
	digitalWrite (GPIO_COL_2, HIGH);
	digitalWrite (GPIO_COL_3, LOW);
	digitalWrite (GPIO_COL_4, LOW);

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

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

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_3;

	piUnlock (PLAYER_FLAGS_KEY);

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

	flags_teclado &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_4;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

 int key_pressed (fsm_t* this) {
	int result = 0;

	piLock (FLAG_KEY);
	result = (flags_teclado & FLAG_KEY_PRESSED);
	piUnlock (FLAG_KEY);

	return result;
}

 void process_key (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	flags_teclado &= (~FLAG_KEY_PRESSED);

	piUnlock (FLAG_KEY);


	switch(p_teclado->teclaPulsada.col){
		case COL_1:
			if(p_teclado->teclaPulsada.row==ROW_1){
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_DISPARO;
				piUnlock(PLAYER_FLAGS_KEY);
				printf("Tecla D pulsada!\n");
				fflush(stdout);
//			piLock(PLAYER_FLAGS_KEY);
//			flags_player|=FLAG_START_DISPARO;
//			piUnlock (PLAYER_FLAGS_KEY);
//
//			fflush(stdout);
 }
			break;

		case COL_2:
			if(p_teclado->teclaPulsada.row==ROW_1){
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_IMPACTO;
				piUnlock(PLAYER_FLAGS_KEY);
				printf("Tecla S pulsada!\n");
				fflush(stdout);

//			piLock(PLAYER_FLAGS_KEY);
//			flags_player|=FLAG_START_IMPACTO;
//			piUnlock (PLAYER_FLAGS_KEY);
//			fflush(stdout);
			}
			break;

		case COL_3:
			printf("Hola");
			fflush(stdout);

			break;
		case COL_4:
			printf("\nKeypress \"%c\"...\n",
					tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
			fflush(stdout);
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}


}

 void timer_duracion_columna_isr (union sigval value) {
	piLock (FLAG_KEY);
	flags_teclado |= FLAG_TMR_TIMEOUT;
	piUnlock (FLAG_KEY);
}

int initialize(TipoTeclado *p_teclado) {
//	if (wiringPiSetupGpio() < 0) {
//	    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
//	    return 1 ;
//	}

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

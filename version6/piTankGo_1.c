/** File Name: PiTankGo_1.c
  * Description: Programa principal
 */

/* Includes ------------------------------------------------------------------*/
#include "piTankGo_1.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "fsm.h"
#include "tmr.h"
#include "teclado.h"
#include "joystick.h"

/* Private variables ---------------------------------------------------------*/
int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};

int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

int flags_juego = 0;
int flags_player = 0;


//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

/**
 * @brief	Función de inicialización del sistema. Aquí se inicializa el wiringPi, que permite usar el gpio.
 * @param	puntero tipo sistema
 * @return	result. Variable que indica la inicialización correcta.
 */
int ConfiguraSistema (TipoSistema *p_sistema) {
	int result = 0;
	piLock(STD_IO_BUFFER_KEY);
	wiringPiSetupGpio();
	piUnlock(STD_IO_BUFFER_KEY);

	return result;
}

/**
 * @brief	Función que inicializa los efectos de disparo y de impacto con las frecuencias, tiempos y nombres correspondientes
 * @param	puntero de tipo TipoSistema
 * @return	devuelve un entero que indica si se ha lanzado correctamente la hebra que explora el teclado
 * @note
 */
int InicializaSistema (TipoSistema *p_sistema) {
	int result = 0;
	InicializaEfecto(&p_sistema->player.efecto_disparo,"DISPARO",frecuenciaGOT,tiempoGOT,518);
	p_sistema->player.p_efecto=&p_sistema->player.efecto_disparo;
	InicializaEfecto(&p_sistema->player.efecto_impacto,"IMPACTO",frecuenciaTetris,tiempoTetris,55);
	p_sistema->player.p_efecto=&p_sistema->player.efecto_impacto;
	InicializaPlayer(&p_sistema->player);
	p_sistema->player.tmr=tmr_new(timer_player_duracion_nota_actual_isr);
	// Lanzamos thread para exploracion del teclado convencional del PC
	result = piThreadCreate (thread_explora_teclado_PC);

	if (result != 0) {
		printf ("Thread didn't start!!!\n");
		return -1;
	}

	return result;
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------


/**
 * @brief	aquí se define el comportamiento de la hebra que ecplora el teclado.
 * @note	se ejecuta un bucle infinito en el que cada diez segundos se comprueba que tecla se ha pulsado. Para ello se comprueba si se ha pulsado una tecla
 * llamando a la función kbhit(). Una vez hecho esto, se guarda en teclaPulsada la tecla pulsada usando la función kbread(). Con esto, se plantea un case donde
 * se ejecuta la función correspondiente. Por ejemplo al pulsar la D se activa el flag de disparo que activa el efecto de disparo.
 */
PI_THREAD (thread_explora_teclado_PC) {
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();

			switch(teclaPulsada) {
				case 'p':
					piLock(PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_DISPARO;
					piUnlock(PLAYER_FLAGS_KEY);
					printf("Tecla D pulsada!\n");
					fflush(stdout);
					break;
				case 'i':
					piLock(PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_IMPACTO;
					piUnlock(PLAYER_FLAGS_KEY);
					printf("Tecla S pulsada!\n");
					fflush(stdout);
					break;
				case 'v':
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_SYSTEM_START;
					piUnlock(SYSTEM_FLAGS_KEY);
					printf("Tecla V pulsada!\n");
					fflush(stdout);
					break;
				case 'w':
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_UP;
					piUnlock(SYSTEM_FLAGS_KEY);
					printf("Tecla W pulsada!\n");
					fflush(stdout);
					break;
				case 's':
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_DOWN;
					piUnlock(SYSTEM_FLAGS_KEY);
					printf("Tecla S pulsada!\n");
					fflush(stdout);
					break;
				case 'a':
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_LEFT;
					piUnlock(SYSTEM_FLAGS_KEY);
					printf("Tecla A pulsada!\n");
					fflush(stdout);
					break;
				case 'd':
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_JOYSTICK_RIGHT;
					piUnlock(SYSTEM_FLAGS_KEY);
					printf("Tecla D pulsada!\n");
					fflush(stdout);
					break;

				case 'q':
					exit(0);
					break;

				default:
					printf("INVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}


/**
 * @brief	función que mete un delay al programa
 * @param	next
 */
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

//variable TipoTeclado que identifica al teclado que estamos utilizando. Se hace extern para que la variable sea la misma en todo el programa
extern TipoTeclado teclado;

/**
 * @brief	Hebra principal
 */
int main ()
{
	TipoSistema sistema;	//Se declara la variable que guarda la información del sistema
	unsigned int next;

	ConfiguraSistema (&sistema);	//Se llama a ConfiguraSistema pasando como parámetro la variable sistema declarada antes
	InicializaSistema (&sistema);	//Se inicializa el sistema con la misma variable
	InicializaTorreta(&sistema.torreta);	//Se guarda en el parámetro torreta del struct TipoSistema la torreta que usaremos en el juego

	/*
	 * Declaración de las transiciones y funciones de entrada y salida de las máquinas de estados
	 */
	fsm_trans_t reproductor[] = {
		{ WAIT_START, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
		{ WAIT_START, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
		{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
		{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
		{-1, NULL, -1, NULL },
	};

	fsm_trans_t columns[] = {
		{ KEY_COL_1, CompruebaColumnTimeout, KEY_COL_2, col_2 },
		{ KEY_COL_2, CompruebaColumnTimeout, KEY_COL_3, col_3 },
		{ KEY_COL_3, CompruebaColumnTimeout, KEY_COL_4, col_4 },
		{ KEY_COL_4, CompruebaColumnTimeout, KEY_COL_1, col_1 },
		{-1, NULL, -1, NULL },
	};

	fsm_trans_t keypad[] = {
		{ KEY_WAITING, key_pressed, KEY_WAITING, process_key},
		{-1, NULL, -1, NULL },
	};

	fsm_trans_t torretast[] = {
			{WAIT_START, CompruebaComienzo,WAIT_MOVE, ComienzaSistema},
			{WAIT_MOVE, CompruebaJoystickUp, WAIT_MOVE, MueveTorretaArriba},
			{WAIT_MOVE, CompruebaJoystickDown, WAIT_MOVE, MueveTorretaAbajo},
			{WAIT_MOVE, CompruebaJoystickLeft, WAIT_MOVE, MueveTorretaIzquierda},
			{WAIT_MOVE, CompruebaJoystickRight, WAIT_MOVE, MueveTorretaDerecha},
			{WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR},
			{WAIT_MOVE, CompruebaFinalJuego, TRIGGER_BUTTON, FinalizaJuego},
			{TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR},
			{TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado},
			{-1, NULL, -1, NULL },
	};


	initialize(&teclado);	//Inicializamos el teclado


	/*
	 * se crean las máquinas de estado para el control del reproductor de efectos, la exitación de columnas, el teclado y la torreta
	 */
	fsm_t* player_fsm = fsm_new (WAIT_START, reproductor, &(sistema.player));
	fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
	fsm_t* keypad_fsm = fsm_new (KEY_WAITING, keypad, &teclado);
	fsm_t* torreta_fsm = fsm_new (WAIT_START, torretast, &sistema.torreta);

	next = millis();
	while (1) {
		//Con la función fsm_fire actualizamos nuestras máquinas de estados a los últimos acontecimientos. Internamente se ejecutan las funciones de entradas y
		//según el resultado de estas, se ejecutan las de salida
		fsm_fire (player_fsm);
		fsm_fire (columns_fsm);
		fsm_fire (keypad_fsm);
		fsm_fire(torreta_fsm);

		next += CLK_MS;
		delay_until (next);
	}

	return 0;
}

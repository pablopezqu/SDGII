/** File Name: player.c
  * Description: programa que implementa el reproductor de sonido
 */
#include "player.h"
#include <string.h>
#include <wiringPi.h>
#include <time.h>
#include <softTone.h>
#include "tmr.h"
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

/**
 * @brief	Esta funci�n carga el efecto en el puntero p_efecto que se le pasa como par�metro a trav�s de las carater�sticas que tambi�n se le pasan como par�metro
 * @param	puntero efecto, nombre efecto, array de frecuencias de la melod�a y array de duraciones de la melod�a
 * @return	devuelve el n�mero de notas de la melod�a cargada en el efecto
 */
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
	strcpy(p_efecto->nombre, nombre);
	int i;
	for(i=0; i< num_notas; i++){
			p_efecto->frecuencias[i]=array_frecuencias[i];
	}

	for( i=0; i< num_notas; i++){
		p_efecto->duraciones[i]=array_duraciones[i];
	}

	p_efecto->num_notas=num_notas;
	return p_efecto->num_notas;
}

/**
 * @brief	pone a punto el reproductor
 * @param	puntero al reproductor que se va a utilizar
 */
void InicializaPlayer (TipoPlayer *p_player) {

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

//Las siguientes funciones de entrada tienen como funci�n comprobar si los flags asociados al reproductor de la fsm est�n activados o no
int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

/*
 * En este caso, la funci�n CompruebaNuevaNota no s�lo comprueba si se ha activado o no un flag, sino tambi�n si quedan m�s notas por reproducir en el array de la melod�a
 */
int CompruebaNuevaNota (fsm_t* this){
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;
	if((p_player->posicion_nota_actual+1)<=p_player->p_efecto->num_notas)	//Comprobaci�n de si se ha llegado al final del array
		result=flags_player & FLAG_PLAYER_END;

	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}
/*
 *
 * La funci�n debe comprobar si efectivamente no quedan m�s notas por reproducir en el array
 */
int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;
	if((p_player->posicion_nota_actual+1)>p_player->p_efecto->num_notas) //Aqu� se realiza la comprobaci�n
		result=flags_player & FLAG_PLAYER_END;

	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/*
 * @brief Esta funci�n debe inicializar el sonido de disparo reproduciendo la primera nota
 *
 */
void InicializaPlayDisparo (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;	//Se desactiva el flag de comienzo del disparo para atenderse
	TipoPlayer* p_player;						//Creaci�n de un TipoPlayer para manejar el reproductor
	p_player=(TipoPlayer*) (this->user_data);	//que posteriormente se inicializa
	piUnlock(PLAYER_FLAGS_KEY);

	InicializaPlayer(p_player);
	softToneCreate(21);	//�SE PODR�A QUITAR? PROBAR
	softToneWrite(21,p_player->frecuencia_nota_actual); //Reproduce la nota en el pin 21 del GPIO seg�n la numeraci�n BCM
	ComienzaNuevaNota(this);	//Llama a la funci�n que reproduce una nueva nota nueva nota para reproducir la nota siguiente

	piLock(STD_IO_BUFFER_KEY);
	printf("Inicio play disparo \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

/*
 * @brief Tiene la misma funci�n que InicializaPlayDisparo pero para el efecto de impacto
 */
void InicializaPlayImpacto (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;	//Creaci�n de un TipoPlayer para manejar el reproductor que se inicializa
	piUnlock(PLAYER_FLAGS_KEY);

	InicializaPlayer(p_player);
	softToneCreate(21);
	softToneWrite(21,p_player->frecuencia_nota_actual); //Reproduce la nota en el pin 21 del GPIO seg�n la numeraci�n BCM
	ComienzaNuevaNota(this);	//Llama a la funci�n que reproduce una nueva nota nueva nota para reproducir la nota siguiente

	piLock(STD_IO_BUFFER_KEY);
	printf("Inicializa play impacto \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

/*
 * @brief Esta funci�n hace que se reproduzca una nueva nota y lanza un timer para controlar la duraci�n de esta
 */
void ComienzaNuevaNota (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player= (TipoPlayer*) this->user_data ; //Creaci�n de un TipoPlayer para manejar el reproductor que se inicializa con par�metro que se pasa a la funci�n
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	softToneCreate(21);
	softToneWrite(21, p_player->frecuencia_nota_actual); //Reproduce la nota en el pin 21 del GPIO seg�n la numeraci�n BCM
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual); //Se lanza un timer que provocar� una interrupci�n cuando se haya llegado a la duraci�n de la nota que esta reproduci�ndose
															  //para pasar a reproducir la siguiente

	piLock(STD_IO_BUFFER_KEY);
	printf("Comienza nueva nota \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

/*
 * @brief Esta funci�n tiene como objetivo actualizar el reproductor a la siguiente nota, haciendo que todos los par�metros del TipoPlayer definido se actualicen
 */
void ActualizaPlayer (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	flags_player |= FLAG_PLAYER_END;
	TipoPlayer* p_player;						 //Creaci�n de un TipoPlayer para manejar el reproductor que se inicializa con par�metro que se pasa a la funci�ns
	p_player=(TipoPlayer*) (this->user_data);
	p_player->posicion_nota_actual++;
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	piUnlock(PLAYER_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("Actualiza nueva nota \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

/*
 * @brief Esta funci�n es la que hace que acabe el efecto y deje de sonar la melod�a. Se ejecuta si ya no quedan m�s notas por reproducir
 */
void FinalEfecto (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	softToneWrite(21,0);	//De esta forma hacemos que finalice la melod�a

	piLock(STD_IO_BUFFER_KEY);
	printf("Final efecto \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

//Esta funci�n tiene como objetivo activar el flag de TIMEOUT de una nota para pasar a la siguiente. Atiende a la interrupci�n que genera el temporizador al acabar
void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);
}


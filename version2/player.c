
#include "player.h"
#include <string.h>  //strcpy
#include <wiringPi.h>
#include <time.h>
#include <softTone.h>
#include "tmr.h"
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
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

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	//posible variable num_notas_totales

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

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

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;
	if((p_player->posicion_nota_actual+1)<=p_player->p_efecto->num_notas) //variable notas totales?
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

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;
	if((p_player->posicion_nota_actual+1)>p_player->p_efecto->num_notas) //variable notas totales?
		result=flags_player & FLAG_PLAYER_END;

	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
//	flags_player &= ~FLAG_NOTA_TIMEOUT;
	TipoPlayer* p_player;
	p_player=(TipoPlayer*) (this->user_data);
	piUnlock(PLAYER_FLAGS_KEY);
	InicializaPlayer(p_player);
	softToneCreate(21);
	softToneWrite(21,p_player->frecuencia_nota_actual);
	ComienzaNuevaNota(this);
	piLock(STD_IO_BUFFER_KEY);
	printf("Inicio play disparo \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

void InicializaPlayImpacto (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
//	InicializaPlayer(this->user_data);
	TipoPlayer* p_player=(TipoPlayer*) this->user_data;
	piUnlock(PLAYER_FLAGS_KEY);
	InicializaPlayer(p_player);
	softToneCreate(21);
	softToneWrite(21,p_player->frecuencia_nota_actual);
	ComienzaNuevaNota(this);
	piLock(STD_IO_BUFFER_KEY);
	printf("Inicializa play impacto \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

void ComienzaNuevaNota (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	TipoPlayer* p_player= (TipoPlayer*) this->user_data ;
	flags_player &= ~FLAG_PLAYER_END;
	//int frec=p_player->frecuencia_nota_actual;
	piUnlock(PLAYER_FLAGS_KEY);
	softToneCreate(21);
	softToneWrite(21, p_player->frecuencia_nota_actual);
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
	piLock(STD_IO_BUFFER_KEY);
	printf("Comienza nueva nota \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

void ActualizaPlayer (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	flags_player |= FLAG_PLAYER_END;
	TipoPlayer* p_player;
	p_player=(TipoPlayer*) (this->user_data);
	p_player->posicion_nota_actual++;
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	piUnlock(PLAYER_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("Actualiza nueva nota \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

void FinalEfecto (fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("Final efecto \n");
	piUnlock(STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);
}


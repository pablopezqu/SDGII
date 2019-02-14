
#include "player.h"

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
	strcpy(p_efecto->nombre, nombre);  //con &? crear nuevo efecto??
	p_efecto->frecuencias=array_frecuencias;
	p_efecto->duraciones=array_duraciones;
	p_efecto->num_notas=num_notas;
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {
	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	pilock(PLAYER_FLAGS_KEY);
	result = (flags & FLAG_START_DISPARO);
	piunlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;
	pilock(PLAYER_FLAGS_KEY);
	result = (flags & FLAG_START_IMPACTO);
	piunlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;
	pilock(PLAYER_FLAGS_KEY);
	result = (flags & FLAG_PLAYER_END);
	piunlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;
	pilock(PLAYER_FLAGS_KEY);
	result = (flags & FLAG_NOTA_TIMEOUT);
	piunlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	pilock(PLAYER_FLAGS_KEY);
	result = (flags & FLAG_PLAYER_END);
	piunlock(PLAYER_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {
	pilock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	piunlock(PLAYER_FLAGS_KEY);

	pilock(STD_IO_BUFFER_KEY);
	printf("Inicio play disparo \n");
	piunlock(STD_IO_BUFFER_KEY);
}

void InicializaPlayImpacto (fsm_t* this) {
	pilock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	piunlock(PLAYER_FLAGS_KEY);

	pilock(STD_IO_BUFFER_KEY);
	printf("Inicializa play impacto \n");
	piunlock(STD_IO_BUFFER_KEY);
}

void ComienzaNuevaNota (fsm_t* this) {
	pilock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piunlock(PLAYER_FLAGS_KEY);

	pilock(STD_IO_BUFFER_KEY);
	printf("Comienza nueva nota \n");
	piunlock(STD_IO_BUFFER_KEY);
}

void ActualizaPlayer (fsm_t* this) {
	pilock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piunlock(PLAYER_FLAGS_KEY);

	pilock(STD_IO_BUFFER_KEY);
	printf("Actualiza nueva nota \n");
	piunlock(STD_IO_BUFFER_KEY);
}

void FinalEfecto (fsm_t* this) {
	pilock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piunlock(PLAYER_FLAGS_KEY);

	pilock(STD_IO_BUFFER_KEY);
	printf("Final efecto \n");
	piunlock(STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

/*static void timer_player_duracion_nota_actual_isr (union sigval value) {
	// A completar por el alumno...
	// ...
}*/

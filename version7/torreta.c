#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringSerial.h>
#include <string.h>
#include <errno.h>
#include "torreta.h"
#include "joystick.h"

int fd; //variable entera que identifica el puerto serie. Es compartida con el programa principal.
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {

	p_torreta->servo_x.inicio = SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_y.inicio = SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_x.incremento = SERVO_INCREMENTO;
	p_torreta->servo_y.incremento = SERVO_INCREMENTO;
	p_torreta->servo_x.maximo = SERVO_MAXIMO;
	p_torreta->servo_y.maximo = SERVO_MAXIMO;
	p_torreta->servo_x.minimo = SERVO_MINIMO;
	p_torreta->servo_x.posicion=p_torreta->servo_x.inicio;
	p_torreta->servo_y.posicion=p_torreta->servo_y.inicio;

	if(p_torreta->servo_x.posicion > p_torreta->servo_x.maximo ){		//Posición maxima y minima
		p_torreta->servo_x.posicion = p_torreta->servo_x.maximo;
	}

	if(p_torreta->servo_y.posicion > p_torreta->servo_y.maximo ){		//Posición maxima y minima
		p_torreta->servo_y.posicion = p_torreta->servo_y.maximo;
	}

	if(p_torreta->servo_x.posicion < p_torreta->servo_x.minimo){
		p_torreta->servo_x.posicion = p_torreta->servo_x.minimo;
	}
	if(p_torreta->servo_y.posicion < p_torreta->servo_y.minimo){
		p_torreta->servo_y.posicion = p_torreta->servo_y.minimo;
	}

	softPwmCreate (SERVO_PIN_X, p_torreta->servo_x.inicio , SERVO_PWM_RANGE); //Se inicializa la posición de los servos
	softPwmWrite(SERVO_PIN_X, p_torreta->servo_x.posicion);
	softPwmCreate (SERVO_PIN_Y, p_torreta->servo_y.inicio , SERVO_PWM_RANGE);
	softPwmWrite(SERVO_PIN_X, p_torreta->servo_y.posicion);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SYSTEM_START; 			// Se activa el flag de inicio del sistema
	piUnlock(SYSTEM_FLAGS_KEY);

	inicia();									// Se inicializa el Joystick
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_UP);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SHOOT_TIMEOUT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TARGET_DONE);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TRIGGER_BUTTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_END);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SYSTEM_START;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("¡Torreta inicializada y lista para disparar!\n");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);

}

void MueveTorretaArriba (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_UP;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	serialPrintf(fd, "Moviendo arriba");
	piUnlock(STD_IO_BUFFER_KEY);

	TipoTorreta* p_torreta= (TipoTorreta*) (this->user_data);
	if(p_torreta->servo_y.posicion + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo){
		p_torreta->servo_y.posicion = p_torreta->servo_y.incremento + p_torreta->servo_y.posicion ;
		softPwmWrite(SERVO_PIN_Y,p_torreta->servo_y.posicion );
		printf("posición eje y = %d\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}

}

void MueveTorretaAbajo (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_DOWN;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	serialPrintf(fd, "Moviendo abajo");
	piUnlock(STD_IO_BUFFER_KEY);

	TipoTorreta* p_torreta= (TipoTorreta*) (this->user_data);
	if(p_torreta->servo_y.posicion  - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo){
		p_torreta->servo_y.posicion  = p_torreta->servo_y.posicion  - p_torreta->servo_y.incremento;
		softPwmWrite(SERVO_PIN_Y,p_torreta->servo_y.posicion);
		printf("posición eje y = %d\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_LEFT;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	serialPrintf(fd, "Moviendo a la izquierda");
	piUnlock(STD_IO_BUFFER_KEY);

	TipoTorreta* p_torreta= (TipoTorreta*) (this->user_data);
	if(p_torreta->servo_x.posicion - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo){
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion - p_torreta->servo_x.incremento;
		softPwmWrite(SERVO_PIN_Y,p_torreta->servo_x.posicion);
		printf("posición eje x = %d\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_RIGHT;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	serialPrintf(fd, "Moviendo a la derecha");
	piUnlock(STD_IO_BUFFER_KEY);

	TipoTorreta* p_torreta= (TipoTorreta*) (this->user_data);
	if(p_torreta->servo_x.posicion + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo){
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion + p_torreta->servo_x.incremento;
		softPwmWrite(SERVO_PIN_Y,p_torreta->servo_x.posicion);
		printf("posición eje x = %d\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}

}

void DisparoIR (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_TRIGGER_BUTTON;
	piUnlock(SYSTEM_FLAGS_KEY);
}

void FinalDisparoIR (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_SHOOT_TIMEOUT;
	piUnlock(SYSTEM_FLAGS_KEY);
}

void ImpactoDetectado (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_TARGET_DONE;
	piUnlock(SYSTEM_FLAGS_KEY);
}

void FinalizaJuego (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_SYSTEM_END;
	piUnlock(SYSTEM_FLAGS_KEY);

	TipoTorreta* torreta = (TipoTorreta*) (this->user_data);
	torreta->servo_x.posicion = torreta->servo_x.inicio;
	torreta->servo_y.posicion = torreta->servo_y.inicio;
	softPwmWrite(SERVO_PIN_X,torreta->servo_x.posicion);
	softPwmWrite(SERVO_PIN_Y,torreta->servo_y.posicion);
}

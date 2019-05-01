
#include "torreta.h"
#include "joystick.h"

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

	if(p_torreta->posicion.x != p_torreta->servo_x.inicio){		//Pos maxima y minima
		p_torreta->posicion.x = p_torreta->servo_x.inicio;
	}

	if(p_torreta->posicion.y != p_torreta->servo_y.inicio){
		p_torreta->posicion.y = p_torreta->servo_y.inicio;
	}

	softPwmCreate (SERVO_PIN_X, p_torreta->posicion.x , SERVO_PWM_RANGE);
	softPwmCreate (SERVO_PIN_Y, p_torreta->posicion.y , SERVO_PWM_RANGE);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SYSTEM_START;
	piUnlock(SYSTEM_FLAGS_KEY);

	inicia();
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
	printf("¡Torreta inicializada y lista para disparar!");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);

}

void MueveTorretaArriba (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_UP;
	piUnlock(SYSTEM_FLAGS_KEY);

	TipoTorreta* torreta= (TipoTorreta*) (this->user_data);
	if(torreta->posicion.y < torreta->servo_y.maximo){
		torreta->posicion.y+=torreta->servo_y.incremento;
		softPwmWrite(SERVO_PIN_Y,torreta->posicion.y);
	}

	piLock(STD_IO_BUFFER_KEY);
	printf("Moviendo arriba");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}

void MueveTorretaAbajo (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_DOWN;
	piUnlock(SYSTEM_FLAGS_KEY);

	TipoTorreta* torreta= (TipoTorreta*) (this->user_data);
	if(torreta->posicion.y > torreta->servo_y.minimo){
		torreta->posicion.y-=torreta->servo_y.incremento;
		softPwmWrite(SERVO_PIN_Y,torreta->posicion.y);
	}

	piLock(STD_IO_BUFFER_KEY);
	printf("Moviendo abajo");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}

void MueveTorretaIzquierda (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_LEFT;
	piUnlock(SYSTEM_FLAGS_KEY);

	TipoTorreta* torreta= (TipoTorreta*) (this->user_data);
	if(torreta->posicion.x < torreta->servo_x.maximo){	//max o min
		torreta->posicion.x-=torreta->servo_x.incremento;
		softPwmWrite(SERVO_PIN_Y,torreta->posicion.x);
	}

	piLock(STD_IO_BUFFER_KEY);
	printf("Moviendo a la izquierda");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}

void MueveTorretaDerecha (fsm_t* this) {
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= ~FLAG_JOYSTICK_RIGHT;
	piUnlock(SYSTEM_FLAGS_KEY);

	TipoTorreta* torreta= (TipoTorreta*) (this->user_data);
	if(torreta->posicion.x > torreta->servo_x.minimo){
		torreta->posicion.x+=torreta->servo_x.incremento;
		softPwmWrite(SERVO_PIN_Y,torreta->posicion.x);
	}

	piLock(STD_IO_BUFFER_KEY);
	printf("Moviendo a la derecha");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
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
	torreta->posicion.x = torreta->servo_x.inicio;
	torreta->posicion.y = torreta->servo_y.inicio;
	softPwmWrite(SERVO_PIN_X,torreta->posicion.x);
	softPwmWrite(SERVO_PIN_Y,torreta->posicion.y);
}


#include "joystick.h"
#include "fsm.h"
#include "tmr.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include "torreta.h"


int inicia() {

	pinMode(JS_UP, INPUT);
	pullUpDnControl(JS_UP, PUD_UP);
	wiringPiISR (JS_UP, INT_EDGE_RISING, mvArriba);

	pinMode(JS_DOWN,INPUT);
	pullUpDnControl(JS_DOWN, PUD_DOWN);
	wiringPiISR (JS_DOWN, INT_EDGE_RISING, mvAbajo);

	pinMode(JS_R,INPUT);
	pullUpDnControl(JS_R, PUD_DOWN);
	wiringPiISR (JS_R, INT_EDGE_RISING, mvDerecha);

	pinMode(JS_L,INPUT);
	pullUpDnControl(JS_L, PUD_DOWN);
	wiringPiISR (JS_L, INT_EDGE_RISING, mvIzquierda);

	printf("joystick start");
	fflush(stdout);

//piLock(SYSTEM_FLAGS_KEY);
//flags_juego |= FLAG_SYSTEM_START;
//	piUnlock(SYSTEM_FLAGS_KEY);
return 0;
}

void mvArriba() {

		piLock(SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_UP;
		piUnlock(SYSTEM_FLAGS_KEY);
		printf("ARRIBA");

}

void mvAbajo() {

		piLock(SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_DOWN;
		piUnlock(SYSTEM_FLAGS_KEY);

}

void mvIzquierda() {

		piLock(SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_LEFT;
		piUnlock(SYSTEM_FLAGS_KEY);

}

void mvDerecha() {

		piLock(SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_RIGHT;
		piUnlock(SYSTEM_FLAGS_KEY);

}

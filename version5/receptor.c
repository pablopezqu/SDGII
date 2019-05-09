#include "receptor.h"

void configuraRx() {

	pinMode (IR_RX_PIN, INPUT);
	pullUpDnControl(IR_RX_PIN, PUD_DOWN);
	wiringPiISR (IR_RX_PIN, INT_EDGE_RISING, impacto);

}

void impacto() {

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_TARGET_DONE;
	piUnlock(SYSTEM_FLAGS_KEY);

}

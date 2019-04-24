#ifndef _TECLADO_TL04_H_
#define _TECLADO_TL04_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "fsm.h"
#include "tmr.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include "torreta.h"

#define JS_UP 4
#define JS_DOWN 17
#define JS_L 27
#define JS_R 22

//#define FLAG_JS 5

//#define FLAG_JS_UP	0x01
//#define FLAG_JS_DOWN 0x02
//#define FLAG_JS_L 0x04
//#define FLAG_JS_R 0x08

int inicia();
void mvArriba();
void mvAbajo();
void mvIzquierda();
void mvDerecha();

#endif /* _TECLADO_TL04_H_ */

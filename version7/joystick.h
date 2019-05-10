/** File Name: joystick.c
  * Description: librería del programa que implementa el joystick
 */
#ifndef _TECLADO_TL04_H_
#define _TECLADO_TL04_H_

/* Includes ------------------------------------------------------------------*/
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

/* Defines -------------------------------------------------------------------*/
#define JS_UP 4
#define JS_DOWN 17
#define JS_L 27
#define JS_R 22

/* Private function prototypes -----------------------------------------------*/
int inicia();
void mvArriba();
void mvAbajo();
void mvIzquierda();
void mvDerecha();

#endif /* _TECLADO_TL04_H_ */

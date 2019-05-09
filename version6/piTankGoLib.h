/** File Name: PiTankGo_1.h
  * Description: Librer�a con variables comunes en todo el programa
 */

#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "fsm.h"
#include "tmr.h"

// CLAVES PARA MUTEX
#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	3

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
#define FLAG_SYSTEM_START 		0x01
#define FLAG_JOYSTICK_UP 		0x02
#define FLAG_JOYSTICK_DOWN 		0x04
#define FLAG_JOYSTICK_LEFT		0x08
#define FLAG_JOYSTICK_RIGHT		0x10
#define FLAG_TRIGGER_BUTTON		0x40
#define FLAG_SHOOT_TIMEOUT		0x80
#define FLAG_TARGET_DONE		0x100
#define FLAG_SYSTEM_END			0x200

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
#define FLAG_START_DISPARO 		0x01
#define FLAG_START_IMPACTO 		0x02
#define FLAG_PLAYER_STOP 		0x04
#define FLAG_PLAYER_END			0x08
#define FLAG_NOTA_TIMEOUT		0x10

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
};
/* Private variables ---------------------------------------------------------*/
extern int flags_system;
extern int flags_player;

extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

#endif /* _PITANKGOLIB_H_ */

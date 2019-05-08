
/*
 * lcd.c
 *
 *  Created on: 3 may. 2019
 *      Author: Nacho
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd ;
///dev/serial/by-id/usb-Arduino __www.arduino.cc__0042_55736323639351605071-if00
  wiringPiSetupGpio();
  fd = serialOpen ("/dev/ttyACM0", 115200);
///dev/ttyACM0
// Loop, getting and printing characters

  while(1)
  {
	 delay(2000);
    serialPrintf(fd, "Hola mundo ");
    //serialFlush (fd) ;
    delay(2000);
    serialPrintf(fd, "que tal");

  }

}


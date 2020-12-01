/*
 * UART.h
 *
 *  Created on: 19. 5. 2020
 *      Author: pivov
 */

#ifndef UART_H_
#define UART_H_

#include "includes.h"

extern unsigned char command;
extern unsigned char comL;
extern unsigned char comR;

unsigned char buffer[8];

void UART_Init();

#endif /* UART_H_ */

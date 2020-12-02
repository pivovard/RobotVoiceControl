/*
 * UART.c
 *
 *  Created on: 19. 5. 2020
 *      Author: pivov
 */

#include "UART.h"

int pos;    //buffer position counter

void UART_Init(){
    volatile unsigned int i;
    pos = 0;

    command = 0;
    comL = 0;
    comR = 0;

    P3SEL |= 0x30;                            // P3.4,5 = USART0 option select

    ME1 |= UTXE0 + URXE0;                     // Enable USART1 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL1;                          // UCLK = SMCLK
    UBR00 = 0x68;                             // 1Mhz/9600 = 104
    UBR10 = 0x00;                             //
    UMCTL0 = 0x2C;                            // modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;                            // Enable USART1 RX interrupt

    //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void usart0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) usart0_rx (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG1 & UTXIFG0));                // USART1 TX buffer ready?
  //TXBUF0 = RXBUF0;                          // RXBUF1 to TXBUF1   ECHO

  buffer[pos++] = RXBUF0;

  if(buffer[0] != 0x0A) pos = 0;
  if(pos == 5 && buffer[4] == 0x0E){
      command = buffer[1];
      comL = buffer[2];
      comR = buffer[3];

      buffer[0] = 0x00;
      pos = 0;
  }

}



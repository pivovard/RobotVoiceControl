/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   USCI_A.h                                         */
/*                                                                    */
/*          Procedures for USCI_A settings                            */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */

/* ---------- USCI_SPI_Init ----------------------------------------- */

/* Init USCI_A to SPI mode, SMCLK:1 .                                 */

void UCA0_SPI_Init(void) {

    UCA0CTL1 = 0xC1;     /* SMCLK + SW reset */
    UCA0CTL0 = 0x69;          /* SPI master */
//    UCA0CTL1 = UCSSEL1 | UCSSEL0 | UCSWRST;     /* SMCLK + SW reset */
//    UCA0CTL0 = UCMSB | UCMST | UCSYNC;          /* SPI master */
    UCA0BR0 = 0x01;                             /* low byte */
    UCA0BR1 = 0x00;                             /* high byte */

    UCA0_PORT_SEL |= (MISO_BIT | MOSI_BIT | SCK_BIT); /* select function */
    UCA0_PORT_SEL2 |= (MISO_BIT | MOSI_BIT | SCK_BIT); /* select function */
    UCA0_PORT_DIR |= (MOSI_BIT | SCK_BIT);      /* MOSI + SCK = output */
    UCA0_PORT_DIR &= ~(MISO_BIT);               /* MISO = input */
        
    UCA0CTL1 &= ~UCSWRST;                       /* reset end */
    
}

/* ---------- USCI_SPI_Send_Short ----------------------------------- */

/* Send two bytes via SPI (in raw format).                            */

void UCA0_SPI_Send_Short(unsigned short tx_data) {

    while((IFG2 & UCA0TXIFG) == 0) {;}          /* wait until UCA0 not ready */
    UCA0TXBUF = (tx_data >> 8);
    
    while((IFG2 & UCA0TXIFG) == 0) {;}          /* wait until UCA0 not ready */
    UCA0TXBUF = tx_data;
        
}

/* ---------- END -------------------------------------------------- */


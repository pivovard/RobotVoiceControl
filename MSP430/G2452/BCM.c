/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   BCM.c                                            */
/*                                                                    */
/*          Procedures for Basic Clock Module MSP430x2xx              */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ---------- BCM_Init --------------------------------------------- */

/* DCO(13,4) = 8.192 MHz, SMCLK = DCO:8, ACLK = VLOCLK (14.7 kHz)    */

void BCM_Init(void) {
    
    WDTCTL = WDTPW + WDTHOLD;           /* stop WDT */
    
    DCOCTL = ((0x05 << 5) | 0x02);      /* DCO = 4, MOD = 1 */
//    DCOCTL = ((0x04 << 5) | 0x09);      /* DCO = 4, MOD = 9 */
    BCSCTL1 = (XT2OFF | 13);            /* RSEL = 13 */
    BCSCTL2 = DIVS1 | DIVS0;            /* MCLK = DCO:1, SMCLK = DCO:8 */
    BCSCTL3 = LFXT1S1 | LFXT1OF;        /* ACLK = VLOCLK */
    
}

/* ---------- END -------------------------------------------------- */


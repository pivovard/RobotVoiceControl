/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   BCM.c                                            */
/*                                                                    */
/*          Procedures for Basic Clock Module MSP430x2xx              */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ---------- BCM_Init ---------------------------------------------  */

/* DCO(13,4) = 4.096 MHz, SMCLK = DCO:4, ACLK = stop                  */

void BCM_Init(void) {
    
    WDTCTL = WDTPW + WDTHOLD;           /* stop WDT */
    
    DCOCTL = ((0x05 << 5) | 0x0F);      /* DCO = 5, MOD = 16 */
    BCSCTL1 = (XT2OFF | 0x07);          /* RSEL = 7 */
    BCSCTL2 = DIVS_2;                   /* MCLK = DCO:1, SMCLK = DCO:4 */

    _BIS_SR(OSCOFF);                    /* stop XT1 */
    
    P5SEL |= MCLK_BIT | SMCLK_BIT;      /* MCLK + SMCLK output */
    P5DIR |= MCLK_BIT | SMCLK_BIT;      /* MCLK + SMCLK output */
        
}

/* ---------- END -------------------------------------------------- */


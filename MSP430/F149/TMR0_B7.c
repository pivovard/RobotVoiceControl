/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   TMR0_B7.c                                        */
/*                                                                    */
/*          Procedures for Timer0_V                                  */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */


/* ---------- TMR0_B7_Init ------------------------------------------ */

/* Start counting for PWM. Clocked by SMCLK @1.024 MHz,               */
/* PWM period = 100 Hz.                                               */

void TMR0_B7_PWM_Init(void) {

    TB0CTL = 0x0214;                /* UP mode, clear, int. disable */
    TB0CCR0 = 10240;                /* 1.024 MHz : 10240 = 100 Hz */

    TB0CCR1 = 0;
    TB0CCR2 = 0;
        
    TB0CCTL1 = 0x00E0;              /* compare, mode Reset/Set */    
    TB0CCTL2 = 0x00E0;              /* compare, mode Reset/Set */    
   
}

/* ---------- END -------------------------------------------------- */


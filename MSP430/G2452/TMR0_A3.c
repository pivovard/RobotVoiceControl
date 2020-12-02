/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   TMR0_A3.c                                        */
/*                                                                    */
/*          Procedures for Timer0_A3                                  */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */


/* ---------- TMR0_A3_Init ------------------------------------------ */

/* Start counting of Pin Osc in continuous mode.                      */
/* Returns old value of TAR.                                          */

unsigned short TMR0_A3_Count_Osc(void) {

unsigned short  ta_tmp;

    TA0CTL = 0x0000;                            /* stop counting */
    ta_tmp = TA0R;                              /* read old value */
    
    TA0CTL = TASSEL1 | TASSEL0 | MC1 | TACLR;   /* restart INCLK, continuous, clear */

return(ta_tmp);
    
}

/* ---------- END -------------------------------------------------- */


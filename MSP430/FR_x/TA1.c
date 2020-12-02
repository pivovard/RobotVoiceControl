/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                            TA1.c                                   */
/*                                                                    */
/*                    TA1 control functions                           */
/*                    TA1 is used like beep generator                 */
/*                                                                    */
/* ------------------------------------------------------------------ */


/* ---------- --- -------------------------------------------------- */


#include "includes.h"

#include "globals.c"        /* global variables */

/* ---------- --- -------------------------------------------------- */


/* ---------- variables -------------------------------------------- */


/* ---------- beep ----------------------------------------------- */

/* Generate beep. Length is in 15.625 ms scale.                    */

int8_t beep(uint16_t beep_length) {

    /* start beep length timner */
    beep_timer_start(beep_length);

    /* start beep generator */
    TA1_start();

    return(0);

}

/* ---------- TA1_start ------------------------------------------- */

/* Start TA1 to generate 2525 Hz beep on OUT1 and OUT2.             */
/* !! Suppose P1.3 and P3.3 are set in timer out function !!        */

int8_t TA1_start(void) {

    TA1R = 0x0000;
    TA1CTL = 0x0104;        /* ACLK:1, halted, TAIE disable */

    TA1CCTL0 = 0x0000;      /* compare, CCIE disabled */
    TA1CCTL1 = 0x0040;      /* compare, toggle/reset */
    TA1CCTL2 = 0x00C0;      /* compare, toggle/set */

    TA1CCR0 = 12;           /* 2525 Hz */
    TA1CCR1 = 6;            /* double 2525 Hz */
    TA1CCR2 = 6;            /* double 2525 Hz */

    TA1CTL = 0x0114;        /* ACLK:1, up mode to CCR0, TAIE disable */

    return(0);

}

/* ---------- TA1_stop -------------------------------------------- */

/* Stop & clear TA1.                                                */

int8_t TA1_stop(void) {

    TA1CTL = 0x0104;        /* ACLK:1, stop mode, TAIE disable & clear */
    TA1CCTL0 = 0x0000;      /* compare, CCIE disabled and cleared */

    /* both outputs to 0 */
    TA1CCTL1 = 0x0000;      /* bit output mode, 0 */
    TA1CCTL2 = 0x0000;      /* bit output mode, 0 */

    return(0);

}

/* ---------- END -------------------------------------------------- */


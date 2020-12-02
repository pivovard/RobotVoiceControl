/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                            RTC.c                                   */
/*                                                                    */
/*                    RTC control functions                           */
/*                                                                    */
/*                                                                    */
/* ------------------------------------------------------------------ */


/* ---------- --- -------------------------------------------------- */


#include "includes.h"

#include "globals.c"        /* global variables */

/* ---------- --- -------------------------------------------------- */


/* ---------- variables -------------------------------------------- */

/* ---------- RTC_init --------------------------------------------- */

/* Init RTC to use ACLK, set & disable RTCTEVIFG, RT0PSIFG, RT1PSIFG */

int RTC_init(void) {

    RTCCTL0_H = 0xA5;   /* key to enable write to registers */
    RTCCTL1 = 0x6C;     /* binary, RT1PS, 1min int (disabled), HOLD */

    /* default init values */
    RTCSEC = 0x00;
    RTCMIN = 0x00;
    RTCHOUR = 0x00;
    RTCDOW = 0x00;
    RTCDAY = 0x01;
    RTCMON = 0x01;
    RTCYEAR = 0x0000;

    RTCPS0CTL = 0x3818; /* :256, RT0PSIFG = 3.9 ms, disabled */
    RTCPS1CTL = 0xF000; /* :128, RT1PSIFG = 15.625 ms, disabled */

    RTCCTL1 &= ~0x40;   /* clear HOLD bit */

    RTCCTL0_L |= 0x40;  /* set RTCTEVIE = enable RTCTEV */

    /* read RTC, time just changed -> no need to care about consistency */
    now.dow = RTCDOW;
    now.hour = RTCHOUR;
    now.min = RTCMIN;
    now.sec = RTCSEC;
    now.signum = 0;         /* time is positive */

    return(0);

}

/* ---------- RTC_time_addsub ------------------------------------- */

/* Add two time values in RTC_strx format (a + b -> c).             */
/* Wrap in 24 hour format. Returns value 0, 1 or -1 if the operands */
/* a == b, a > b or a < b respectively. For comparision only c can  */
/* be NULL.                                                         */

int8_t RTC_time_math(struct RTC_strx *a,
                    struct RTC_strx *b,
                    struct RTC_strx *c,
                    uint8_t operation) {

int32_t     aa, bb, cc;

TRACE_ON(0x01);

    aa = (int32_t)a->hour * 3600 + (int32_t)a->min * 60 + (int32_t)a->sec;
    bb = (int32_t)b->hour * 3600 + (int32_t)b->min * 60 + (int32_t)b->sec;

    if(a->signum != 0) {        /* a is negative */
        aa = -aa;
    }

    if(b->signum != 0) {        /* b is negative */
        bb = -bb;
    }

    switch(operation) {
        case RTC_ADD:
            cc = aa + bb;
            break;
        case RTC_SUB:
            cc = aa - bb;
            break;
        case RTC_CMP:
            break;              /* do nothing here */
        default:
            cc = aa;
            break;
    }

    /* result in c */
    if((c != NULL) && (operation != RTC_CMP)) {
        if(cc < 0) {                /* result is negative */
            c->signum = 1;
            cc = -cc;
        }
        else {
            c->signum = 0;
        }

        c->sec = cc % 60;
        cc = cc / 60;
        c->min = cc % 60;
        cc = cc / 60;
        c->hour = cc % 24;
    }

    /* return value */
    cc = aa - bb;

TRACE_OFF(0x01);

    if(cc == 0) {
        return(0);
    }
    else {
        if(cc > 0) {
            return(1);
        }
        else {
            return(-1);
        }
    }

}

/* ---------- RTC_sum ---------------------------------------------- */

/* Sum all items in RTC_strx as.                                     */

uint16_t RTC_sum(struct RTC_strx *as) {

uint16_t    chsum;

    chsum = 0;

        chsum = chsum + as->signum;
        chsum = chsum + as->hour;
        chsum = chsum + as->min;
        chsum = chsum + as->sec;

return(chsum);

}

/* ---------- RTC_1min_isr ---------------------------------------- */

/* Procedure to service 1 min RTC interrupt.                        */

#pragma vector=RTC_VECTOR         /* vector 0xFFC8 */
__interrupt void RTC_1min_isr(void) {

uint16_t    iv;
uint8_t     cancel_LPM_mode;

    iv = RTCIV;
    cancel_LPM_mode = 0;

    switch (iv) {
        case 0x0006:                /* RTCTEVIFG (1 min. interrupt */
            cancel_LPM_mode = 1;    /* we need to service timer in main loop */
            RTC_1min_flag = 1;      /* indication to main loop */

            /* read RTC, time just changed -> no need to care about consistency */
            now.dow = RTCDOW;
            now.hour = RTCHOUR;
            now.min = RTCMIN;
            now.sec = RTCSEC;
            now.signum = 0;         /* time is positive */

            break;

        default:                    /* unexpected RTC interrupt */
            break;
    }

    /* check if LPM should be canceled */
    if(cancel_LPM_mode != 0) {
        /* clear LPM bits in stacked SR */
//      __bic_SR_register_on_exit(unsigned short mask); /* BIC mask, saved_SR */
        __low_power_mode_off_on_exit();                 /* BIC.W #0xF0, saved_SR */
    }

}

/* ---------- END -------------------------------------------------- */


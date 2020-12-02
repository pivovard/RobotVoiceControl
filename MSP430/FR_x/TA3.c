/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                            TA3.c                                   */
/*                                                                    */
/*                    TA3 control functions                           */
/*                    TA3 is used for SW timers                       */
/*                                                                    */
/* ------------------------------------------------------------------ */


/* ---------- --- -------------------------------------------------- */


#include "includes.h"

#include "globals.c"        /* global variables */

/* ---------- --- -------------------------------------------------- */


/* ---------- variables -------------------------------------------- */

//#pragma NOINIT(timer_required)
//#pragma DATA_SECTION(timer_required, ".TI.persistent")
#pragma PERSISTENT(timer_required)
uint8_t     timer_required = 5;     /* variable indicating that timing operation is required */

uint8_t     cancel_LPM_mode;    /* variable indicating that isr should cancel LPM */

uint16_t    LCD_string_timer_count;  /* counting variable */
uint16_t    LCD_string_timer_period; /* counting variable */

uint16_t    beep_timer_count;       /* counting variable in 15.625 ms multiply*/

uint16_t    P1_debouncing_timer_count;    /* in 15.625 ms multiply */

/* ---------- LCD_string_timer_start ----------------------------- */

/* Start timer for LCD scrolling. Start TA3 if it is not running.  */

int8_t LCD_string_timer_start(uint16_t scroll_speed) {

    __disable_interrupt();                     /* do not disturb by TA3 interrupt */

    LCD_string_timer_count = scroll_speed;      /* in 15.625 ms multiply */
    LCD_string_timer_period = scroll_speed;     /* in 15.625 ms multiply */

    /* start timer if it is not running */
    if(timer_required == 0) {
        TA3_start();
    }
    timer_required |= LCD_STRING_TIMER_REQUEST; /* TA3 required */

    __enable_interrupt();

    return(0);

}

/* ---------- LCD_string_timer_stop ------------------------------ */

/* Start timer for LCD scrolling. Start TA3 if it is not running.  */

int8_t LCD_string_timer_stop(void) {

    __disable_interrupt();                     /* do not disturb by TA3 interrupt */

    LCD_string_timer_count = 0;                 /* this stops counting */

    timer_required &= ~LCD_STRING_TIMER_REQUEST;    /* TA3 is not required */

    /* stop timer and clear CCIF0 if it is not required */
    if(timer_required == 0) {
        TA3_stop();
    }

    __enable_interrupt();

    return(0);

}

/* ---------- P1_debouncing_timer_start -------------------------- */

/* Start timer for P1 debouncing. Start TA3 if it is not running.  */
/* Called inside P1 isr.                                           */

int8_t P1_debouncing_timer_start(void) {

TRACE_ON(0x02);

P1_debouncing_timer_count = P1_DEBOUNCING_DELAY;    /* in 15.625 ms multiply */

    /* start timer if it is not running */
    if(timer_required == 0) {
        TA3_start();
    }

    timer_required |= P1_DEBOUNCING_TIMER_REQUEST;      /* TA3 required */

TRACE_OFF(0x02);

    return(0);

}

/* ---------- P1_debouncing_timer_stop --------------------------- */

/* Stop P1 debouncing timer after 1 timer period.                  */

int8_t P1_debouncing_timer_stop(void) {

    __disable_interrupt();                      /* do not disturb by TA3 interrupt */

    P1_debouncing_timer_count = 0;              /* this stops counting */

    timer_required &= ~P1_DEBOUNCING_TIMER_REQUEST; /* TA3 is not required */

    /* stop timer and clear CCIF0 if it is not required */
    if(timer_required == 0) {
        TA3_stop();
    }

    __enable_interrupt();

    return(0);

}

/* ---------- beep_timer_start ----------------------------------- */

/* Start timer for beep length measurement.                        */

int8_t beep_timer_start(uint16_t beep_length) {

    __disable_interrupt();                  /* do not disturb by TA3 interrupt */

    beep_timer_count = beep_length;         /* in 15.625 ms multiply */

    /* start timer if it is not running */
    if(timer_required == 0) {
        TA3_start();
    }
    timer_required |= BEEP_TIMER_REQUEST;   /* TA3 required */

    __enable_interrupt();

    return(0);

}

/* ---------- beep_timer_stop ------------------------------------ */

/* Stop timer for beep length measurement. Normally, stop is       */
/* done automatically in isr.                                      */

int8_t beep_timer_stop(void) {

    __disable_interrupt();              /* do not disturb by TA3 interrupt */

    beep_timer_count = 0;               /* this stops counting */

    timer_required &= ~BEEP_TIMER_REQUEST;    /* TA3 is not required */

    /* stop timer and clear CCIF0 if it is not required */
    if(timer_required == 0) {
        TA3_stop();
    }

    __enable_interrupt();

    return(0);

}

/* ---------- TA3_init ------------------------------------------- */

/* Init TA3. Does not start the timer.                             */

int8_t TA3_init(void) {

    timer_required = 0;         /* no timing operation required */

    return(0);

}

/* ---------- TA3_start ------------------------------------------- */

/* Start TA3 to generate 15.625 ms periodic interrupts.             */

int8_t TA3_start(void) {

    TA3R = 0x0000;
    TA3CTL = 0x01D4;        /* ACLK:8, up mode to CCR0, TAIE disable */
    TA3CCTL0 = 0x0010;      /* compare, CCIE enabled and cleared */
    TA3CCR0 = 63;           /* period 15.625 ms */
    TA3EX0 = 0x0000;

    return(0);

}

/* ---------- TA3_stop -------------------------------------------- */

/* Stop & clear TA3.                                                */

int8_t TA3_stop(void) {

    TA3CTL = 0x01C4;        /* ACLK:8, stop mode, TAIE disable & clear */
    TA3CCTL0 = 0x0000;      /* compare, CCIE disabled and cleared */

    return(0);

}

/* ---------- TA3_CCIR0_isr --------------------------------------- */

/* Procedure to service TA3CCIR0 interrupt. If TA3 is active, the   */
/* routine is called every 15.625 ms.                               */

#pragma vector=TIMER3_A0_VECTOR         /* vector 0xFFD2 */
__interrupt void TA3_CCIR0_isr(void) {

    cancel_LPM_mode = 0;

    /* --- service LCD_string timer --- */
    if(LCD_string_timer_count > 0) {
        LCD_string_timer_count -= 1;
        if(LCD_string_timer_count == 0) {
            LCD_string_timer_flag = 1;      /* set flag to service end counting */
            cancel_LPM_mode = 1;            /* modify PSW to exit from isr without LPM */
            LCD_string_timer_count = LCD_string_timer_period;   /* restart for new period */
        }
    }

    /* --- service P1_debouncing timer --- */
    if(P1_debouncing_timer_count > 0) {
        P1_debouncing_timer_count -= 1;

        if(P1_debouncing_timer_count == 0) {
            /* check P1 */
            P1_new_state = P1IN;            /* read P1 */
            P1_switch_xor = ((P1_state ^ P1_new_state) & P1_SW_MASK);
            if(P1_switch_xor != 0) {
                /* some switch changed */
                P1_state = P1_new_state;
                cancel_LPM_mode = 1;    /* modify PSW to exit from isr without LPM */
            }
            else {
                /* perhaps some glitch or what? */
                P1IFG = 0x00;           /* clear old flags */
                P1IE = P1_SW_MASK;      /* enable SW1 and SW2 interrupts */
            }
            timer_required &= ~P1_DEBOUNCING_TIMER_REQUEST; /* TA3 is not required */
        }
    }

    /* --- service beep timer --- */
    if(beep_timer_count > 0) {
        beep_timer_count -= 1;

        if(beep_timer_count == 0) {
            TA1_stop();               /* stop beep from here requires less energy */
            timer_required &= ~BEEP_TIMER_REQUEST;  /* TA3 is not required for beep */
        }
    }

    /* check if TA3 is required */
    if(timer_required == 0) {
        TA3_stop();                         /* TA3 is not required -> stop it */
    }

    /* check if LPM should be canceled */
    if(cancel_LPM_mode != 0) {
        /* clear LPM bits in stacked SR */
//      __bic_SR_register_on_exit(unsigned short mask); /* BIC mask, saved_SR */
        __low_power_mode_off_on_exit();                 /* BIC.W #0xF0, saved_SR */
    }

}

/* ---------- END -------------------------------------------------- */


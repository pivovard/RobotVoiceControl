/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                            IO_port.c                               */
/*                                                                    */
/*                    Functions for IO port manipulation              */
/*                                                                    */
/* ------------------------------------------------------------------ */


/* ---------- --- -------------------------------------------------- */


#include "includes.h"

#include "globals.c"        /* global variables */

/* ---------- --- -------------------------------------------------- */


/* ---------- variables -------------------------------------------- */


/* ---------- IO_port_init ----------------------------------------- */

/* Init all IO ports.                                                */

/*
 * Port assignment:
 * P1.0     red LED         out active H
 * P1.1     SW1             in  active L
 * P1.2     SW2             in  active L
 *
 * P9.7     green LED       out active H
 *
 */

int8_t IO_port_init(void) {

    PM5CTL0 &= ~LOCKLPM5;           /* Disable the GPIO power-on default high-impedance mode */
                                    /* to activate previously configured port settings */
    /* init P1 */
    P1SEL0 &= ~0x07;    /* P1.0 - P1.2 = IO */
    P1SEL1 &= ~0x07;    /* P1.0 - P1.2 = IO */

    P1DIR |= 0x01;                      /* P1.0 = out */
    P1OUT &= ~0x01;                     /* LED off */

    P1OUT |= P1_SW_MASK;                /* P1.1, P1.2 pullup */
    P1REN |= P1_SW_MASK;                /* P1.1, P1.2 pullup enable */

    /* set P1.3 for TA1 pulse output */
    P1SEL0 |= 0x08;
    P1SEL1 &= ~0x08;
    P1DIR |= 0x08;

    /* set P3.3 for TA1 pulse output */
    P3SEL0 &= ~0x08;
    P3SEL1 |= 0x08;
    P3DIR |= 0x08;

    /* init P9 */
    P9SEL0 &= ~0x80;                    /* P9.7 = IO */
    P9SEL1 &= ~0x80;                    /* P9.7 = IO */

    P9DIR |= 0x80;                      /* P9.7 = out */
    P9OUT &= ~0x80;                     /* LED off */

    /* P4.0 = MCLK, P4.1 = ACLK */
    P4DIR |=0x03;                       /* P4.0, P4.1 output */
    P4SEL0 |= 0x03;
    P4SEL1 |= 0x03;

    /* enable iterrupt on P1 */
    P1IES = P1_SW_MASK;                 /* interrupt on falling edge */
    P1IFG = 0x00;                       /* clear all interrupts */
    P1IE = P1_SW_MASK;                  /* enable SW1 and SW2 interrupts */

    /* enable pullup R for unused pins */
    P1REN |= 0xF0;
    P2REN |= 0x00;
    P3REN |= 0xF7;
    P4REN |= 0x8F;
    P6REN |= 0x07;
    P8REN |= 0xF0;
    P9REN |= 0x7F;
    PJREN |= 0xCF;

    /* read P1 inputs */
    P1_state = P1IN & P1_SW_MASK;       /* read state of P1 inputs */

    return(0);

}

/* ---------- P1_switch_service ----------------------------------- */

/* Called from main programm loop if sw1 or sw2 changed.            */

int8_t P1_switch_service(void) {

    /* service SW1 change */
    if((P1_switch_xor & SW1_MASK) != 0) {
        if((P1_new_state & SW1_MASK) == 0) {
            /* SW1 down */
            P1IES &= ~SW1_MASK;                 /* set interrupt to rising edge */
            cancel_highest_priority_alarm();    /* cancel the highest priority alarm */
        }
        else {
            /* SW1 up */
            P1IES |= SW1_MASK;                  /* set interrupt to falling edge */
        }
    }

    /* service SW2 */
    if((P1_switch_xor & SW2_MASK) != 0) {
        if((P1_new_state & SW2_MASK) == 0) {
            /* SW1 down */
            P1IES &= ~SW2_MASK;                 /* set interrupt to rising edge */
        }
        else {
            /* SW1 up */
            P1IES |= SW2_MASK;                  /* set interrupt to falling edge */
        }
    }

    P1_switch_xor = 0;              /* service done */

    P1IFG = 0x00;                   /* clear old flags */
    P1IE = P1_SW_MASK;              /* enable SW1 and SW2 interrupts */

    return(0);

}

/* ---------- P1_isr ---------------------------------------------- */

/* Procedure to service P1 interrupt. Starts TA3 to read port after */
/* some delay for debouncing.                                       */

#pragma vector=PORT1_VECTOR             /* vector 0xFFDA */
__interrupt void P1_isr(void) {

    /* disable interrupts for debouncing time */
    P1IE &= ~P1_SW_MASK;                /* disable SW1 and SW2 interrupts */

    /* clear interrupt indication in IV */
    P1IFG = 0x00;

    /* start timer to delay P1 reading */
    P1_debouncing_timer_start();

}

/* ---------- END -------------------------------------------------- */


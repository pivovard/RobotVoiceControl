/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   WDT.c                                            */
/*                                                                    */
/*          Procedures for Watchdog Timer settings                    */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */

unsigned short  wdt_counter;
unsigned short  sec1_counter;

extern unsigned char   timer_tick;
extern unsigned char   sec1_tick;

/* ---------- WDT_Set_Timer ---------------------------------------- */

/* Set WDT to timer mode with interrupt.                             */
/* If SMCLK = 1.024 MHz => IRQ freq = 125 Hz                         */

void WDT_Set_Timer(void) {

    wdt_counter = 0;
    sec1_counter = 0;
        
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTIS0;     /* timer mode SMCLK:8192 */
    IFG1 &= ~WDTIFG;                                   /* clear flag */
    IE1 |= WDTIE;                                      /* enable WDT int */

}

/* ---------- WDT_ISR ---------------------------------------------- */

/* WDT timer mode interrupt service routine.                         */

#pragma vector = WDT_VECTOR             /* vector for watchdog timer */
interrupt void WDT_ISR() {

    wdt_counter += 1;
    timer_tick += 1;
    
    sec1_counter += 1;
    if(sec1_counter == 125) {
        sec1_counter = 0;
        sec1_tick += 1;
    }
                        
}

/* ---------- END -------------------------------------------------- */


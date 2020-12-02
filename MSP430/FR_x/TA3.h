/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                         TA3.h                                      */
/*                                                                    */
/*                 Include file for TA3                               */
/*                                                                    */
/* ------------------------------------------------------------------ */

#ifndef __TA3__

#define __TA3__

/* ---------- --- -------------------------------------------------- */

#define LCD_STRING_TIMER_REQUEST    0x01    /* bit indicating timer rquest by LCD */
#define P1_DEBOUNCING_TIMER_REQUEST 0x02    /* bit indicating timer rquest by P1 debouncing */
#define BEEP_TIMER_REQUEST 0x04             /* bit indicating timer rquest by beep */

/* ---------- --- -------------------------------------------------- */


/* ---------- prototypes ------------------------------------------- */

int8_t LCD_string_timer_start(uint16_t scroll_speed) ;

int8_t LCD_string_timer_stop(void) ;

int8_t P1_debouncing_timer_start(void) ;

int8_t P1_debouncing_timer_stop(void) ;

int8_t beep_timer_start(uint16_t beep_length) ;

int8_t beep_timer_stop(void) ;

int8_t TA3_init(void) ;

int8_t TA3_start(void) ;

int8_t TA3_stop(void) ;

__interrupt void TA3_CCIR0_isr(void) ;

#endif

/* ---------- END -------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                         IO_port.h                                  */
/*                                                                    */
/*                 Include file for IO_port.c                         */
/*                                                                    */
/* ------------------------------------------------------------------ */

#ifndef __IO_PORT__

#define __IO_PORT__

/* ---------- --- -------------------------------------------------- */

#define P1_SW_MASK      0x06        /* mask for SW1 and SW2 */
#define SW1_MASK        0x02        /* mask for SW1 */
#define SW2_MASK        0x04        /* mask for SW2 */

/* ---------- --- -------------------------------------------------- */

#define RED_LED_ON      P1OUT |= 0x01
#define RED_LED_OFF     P1OUT &= ~0x01

#define GREEN_LED_ON    P9OUT |= 0x80
#define GREEN_LED_OFF   P9OUT &= ~0x80

/* ---------- prototypes ------------------------------------------- */

int8_t IO_port_init(void) ;

int8_t P1_switch_service(void) ;

__interrupt void P1_isr(void) ;

#endif

/* ---------- END -------------------------------------------------- */


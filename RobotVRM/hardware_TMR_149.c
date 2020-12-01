/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   hardware.c                                       */
/*                                                                    */
/*          Basic TMR robot hardware functions                        */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ---------- Hardware_Init ---------------------------------------- */

void Hardware_Init(void) {

    /* LED */    
    LED_DIR |= 0xFF;                    /* all LED output */
    LED_OUT |= 0xFF;                    /* all LED OFF */

    /* SMI & SMO */    
    SMO1_DIR &= ~SMO1_MASK;             /* input */      
    SMO2_DIR &= ~SMO2_MASK;             /* input */      
    SMO3_DIR &= ~SMO3_MASK;             /* input */
    SMI_DIR |= SMI_MASK;                /* output */
    SMI_OUT |= SMI_MASK;                /* H = not active */
    
    /* confog switch */
    CONFIG_DIR &= CONFIG_MASK;          /* input */

    /* Motor port */
    MOT_DIR |= MOT_IN_BIT_ALL | L_MOT_PWM_BIT | R_MOT_PWM_BIT;
    MOT_SEL |= L_MOT_PWM_BIT | R_MOT_PWM_BIT;
    MOT_OUT &= ~MOT_IN_BIT_ALL;
    
}

/* ---------- Motor_Init ------------------------------------------- */

void Motor_Init(void) {
    
    MOT_DIR |= MOT_IN_BIT_ALL | L_MOT_PWM_BIT | R_MOT_PWM_BIT;
    MOT_SEL |= L_MOT_PWM_BIT | R_MOT_PWM_BIT;
    MOT_OUT &= ~MOT_IN_BIT_ALL;
    
}

/* ---------- END -------------------------------------------------- */


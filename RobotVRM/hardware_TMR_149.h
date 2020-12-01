/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   hardware.h                                       */
/*                                                                    */
/*          Basic LaunchPad hardware definitions                      */
/*                                                                    */
/* ------------------------------------------------------------------ */


#define     LED_DIR         P2DIR    
#define     LED_OUT         P2OUT    

#define     R1_LED_BIT      0x80            /* P1.0 */    
#define     G1_LED_BIT      0x08            /* P1.6 */
#define     R2_LED_BIT      0x40            /* P1.0 */    
#define     G2_LED_BIT      0x04            /* P1.6 */
#define     R3_LED_BIT      0x20            /* P1.0 */    
#define     G3_LED_BIT      0x02            /* P1.6 */
#define     R4_LED_BIT      0x10            /* P1.0 */    
#define     G4_LED_BIT      0x01            /* P1.6 */

#define     R1_LED_OFF      LED_OUT |= R1_LED_BIT       /* R led OFF */
#define     R1_LED_ON       LED_OUT &= ~R1_LED_BIT      /* R led ON */
#define     R1_LED_TOGGLE   LED_OUT ^= R1_LED_BIT       /* R led Toggle */
#define     G1_LED_OFF      LED_OUT |= G1_LED_BIT       /* G led OFF */
#define     G1_LED_ON       LED_OUT &= ~G1_LED_BIT      /* G led ON */
#define     G1_LED_TOGGLE   LED_OUT ^= G1_LED_BIT       /* G led Toggle */
    
#define     R2_LED_OFF      LED_OUT |= R2_LED_BIT       /* R led OFF */
#define     R2_LED_ON       LED_OUT &= ~R2_LED_BIT      /* R led ON */
#define     R2_LED_TOGGLE   LED_OUT ^= R2_LED_BIT       /* R led Toggle */
#define     G2_LED_OFF      LED_OUT |= G2_LED_BIT       /* G led OFF */
#define     G2_LED_ON       LED_OUT &= ~G2_LED_BIT      /* G led ON */
#define     G2_LED_TOGGLE   LED_OUT ^= G2_LED_BIT       /* G led Toggle */
    
#define     R3_LED_OFF      LED_OUT |= R3_LED_BIT       /* R led OFF */
#define     R3_LED_ON       LED_OUT &= ~R3_LED_BIT      /* R led ON */
#define     R3_LED_TOGGLE   LED_OUT ^= R3_LED_BIT       /* R led Toggle */
#define     G3_LED_OFF      LED_OUT |= G3_LED_BIT       /* G led OFF */
#define     G3_LED_ON       LED_OUT &= ~G3_LED_BIT      /* G led ON */
#define     G3_LED_TOGGLE   LED_OUT ^= G3_LED_BIT       /* G led Toggle */
    
#define     R4_LED_OFF      LED_OUT |= R4_LED_BIT       /* R led OFF */
#define     R4_LED_ON       LED_OUT &= ~R4_LED_BIT      /* R led ON */
#define     R4_LED_TOGGLE   LED_OUT ^= R4_LED_BIT       /* R led Toggle */
#define     G4_LED_OFF      LED_OUT |= G4_LED_BIT       /* G led OFF */
#define     G4_LED_ON       LED_OUT &= ~G4_LED_BIT      /* G led ON */
#define     G4_LED_TOGGLE   LED_OUT ^= G4_LED_BIT       /* G led Toggle */
    
#define     G_LED_OFF(n)    LED_OUT |= (0x08 >> (n-1))  /* G led OFF */
#define     R_LED_OFF(n)    LED_OUT |= (0x80 >> (n-1))  /* G led OFF */
    
#define     G_LED_ON(n)     LED_OUT &= ~(0x08 >> (n-1))  /* G led ON */
#define     R_LED_ON(n)     LED_OUT &= ~(0x80 >> (n-1))  /* G led ON */
    
#define     G_LED_TOGGLE(n) LED_OUT ^= (0x08 >> (n-1))  /* G led TOGGLE */
#define     R_LED_TOGGLE(n) LED_OUT ^= (0x80 >> (n-1))  /* G led TOGGLE */

/* ------------------------------------------------------------------ */
    
#define     MOT_DIR         P4DIR    
#define     MOT_OUT         P4OUT    
#define     MOT_SEL         P4SEL    

#define     L_MOT_PWM_BIT   0x02    
#define     R_MOT_PWM_BIT   0x04

#define     L_MOT_IN1_BIT   0x08    
#define     L_MOT_IN2_BIT   0x10    

#define     R_MOT_IN3_BIT   0x20    
#define     R_MOT_IN4_BIT   0x40    
    
#define     MOT_IN_BIT_ALL  0x78    

#define     L_MOT_FORWARD   MOT_OUT |= L_MOT_IN2_BIT; MOT_OUT &= ~L_MOT_IN1_BIT    
#define     L_MOT_BACK      MOT_OUT |= L_MOT_IN1_BIT; MOT_OUT &= ~L_MOT_IN2_BIT    
#define     L_MOT_BREAK     MOT_OUT |= L_MOT_IN1_BIT | L_MOT_IN2_BIT    

#define     R_MOT_BACK      MOT_OUT |= R_MOT_IN4_BIT; MOT_OUT &= ~R_MOT_IN3_BIT    
#define     R_MOT_FORWARD   MOT_OUT |= R_MOT_IN3_BIT; MOT_OUT &= ~R_MOT_IN4_BIT    
#define     R_MOT_BREAK     MOT_OUT |= R_MOT_IN3_BIT | R_MOT_IN4_BIT    

#define     MOT_SPEED_MAX   10240   /* value for TB0CCRx - maximal speed */
#define     MOT_SPEED_FULL  2300    /* value for TB0CCRx - full speed */
#define     MOT_SPEED_SLOW  1300     /* value for TB0CCRx - half speed */
#define     MOT_SPEED_MIN   500     /* value for TB0CCRx - stop */
#define     MOT_SPEED_STOP  0       /* value for TB0CCRx - stop */

#define     L_MOT_MAX       TB0CCR1 = MOT_SPEED_MAX     /* value for TB0CCRx */
#define     R_MOT_MAX       TB0CCR2 = MOT_SPEED_MAX     /* value for TB0CCRx */
#define     L_MOT_FULL      TB0CCR1 = MOT_SPEED_FULL    /* value for TB0CCRx */
#define     R_MOT_FULL      TB0CCR2 = MOT_SPEED_FULL    /* value for TB0CCRx */
#define     L_MOT_SLOW      TB0CCR1 = MOT_SPEED_SLOW    /* value for TB0CCRx */
#define     R_MOT_SLOW      TB0CCR2 = MOT_SPEED_SLOW    /* value for TB0CCRx */
#define     L_MOT_MIN       TB0CCR1 = MOT_SPEED_MIN     /* value for TB0CCRx */
#define     R_MOT_MIN       TB0CCR2 = MOT_SPEED_MIN     /* value for TB0CCRx */
#define     L_MOT_STOP      TB0CCR1 = MOT_SPEED_STOP    /* value for TB0CCRx */
#define     R_MOT_STOP      TB0CCR2 = MOT_SPEED_STOP    /* value for TB0CCRx */
    
/* ------------------------------------------------------------------ */
    
#define     SMO1_DIR        P6DIR   /* SubModule 1 Out Port */    
#define     SMO1_OUT        P6OUT    
#define     SMO1_IN         P6IN    
#define     SMO1_MASK       0x07    

#define     SMO2_DIR        P1DIR   /* SubModule 2 Out Port */     
#define     SMO2_OUT        P1OUT    
#define     SMO2_IN         P1IN    
#define     SMO2_MASK       0x07    

#define     SMO3_DIR        P1DIR   /* SubModule 3 Out Port */     
#define     SMO3_OUT        P1OUT    
#define     SMO3_IN         P1IN    
#define     SMO3_MASK       0x07    

#define     SMI_DIR         P3DIR   /* All SubModules In Port */     
#define     SMI_OUT         P3OUT    
#define     SMI_IN          P3IN    
#define     SMI_MASK        0x07    

#define     CONFIG_DIR      P5DIR   /* Configuration switches */     
#define     CONFIG_OUT      P5OUT    
#define     CONFIG_IN       P5IN    
#define     CONFIG_MASK     0x07    

/* ------------------------------------------------------------------ */
    
void Hardware_Init(void) ;

void Motor_Init(void) ;

/* ---------- END -------------------------------------------------- */


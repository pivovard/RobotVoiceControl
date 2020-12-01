/* ----------------------------------------------------------------- */
/*                                                                   */
/*                                                                   */
/*           Touch_01.c                                              */
/*                                                                   */
/*      Experiments with LaunchPad touch module                      */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/* ----------------------------------------------------------------- */

/* ---------- includes --------------------------------------------- */

#include    "includes.h"
#include    "commands.h"
#include    "UART.h"

/* ---------- variables -------------------------------------------- */

unsigned char   TMR_configured;     /* configuration map of TMR: 1 = submodule ON */
unsigned char   TMR_active;         /* map of active submodules */
unsigned char   lock_state;         /* map of diff. > DIFF_LIMIT */
unsigned char   diff_state;         /* map of diff. > 1 */
unsigned char   TMR_diff_mask;      /* mask for valid differences */

unsigned char   timer_tick;
unsigned char   sec1_tick;
unsigned short  diff_timer;
unsigned short  VCC_voltage;

unsigned char   SM1_cmd;            /* 3bit command read from submodule */
unsigned char   SM2_cmd;            /* 3bit command read from submodule */
unsigned char   SM3_cmd;            /* 3bit command read from submodule */

unsigned char   SM1_cmd_old;        /* 3bit command read from submodule */
unsigned char   SM2_cmd_old;        /* 3bit command read from submodule */
unsigned char   SM3_cmd_old;        /* 3bit command read from submodule */

unsigned char   active_cmd;

unsigned short  dif12_count;        /* difference counter */
unsigned short  dif23_count;        /* difference counter */
unsigned short  dif13_count;        /* difference counter */

unsigned char command;
unsigned char comL;
unsigned char comR;

/* ---------- main ------------------------------------------------- */

void main(void) { 

    BCM_Init();                         /* stop WDT & init clock module */
    Hardware_Init();                    /* init hw */
    Motor_Init();
    TMR0_B7_PWM_Init();
    WDT_Set_Timer();
    ADC12_Single_Init(0x03);            /* init ADC for Vcc measurement */    
    timer_tick = 0;
    sec1_tick = 0;
    
    UART_Init();
    command = 0;
    comL = 0;
    comR = 0;

    dif12_count = 0;
    dif23_count = 0;
    dif13_count = 0;
    diff_timer = 0;
    
    int d_count = 0;

    P3DIR |= 0x30;              /* P3.5, 3.6 = out */
    P3OUT &= ~0x30;
                    
    _enable_interrupts();
    

    while(1) {
        if(timer_tick != 0) {
            timer_tick -= 1;
            execute(command, comL, comR);
        }
        
        if(sec1_tick != 0) {
            sec1_tick -= 1;
            
            d_count++;
            if(d_count>8) d_count = 0;

            VCC_voltage = ADC12_Single_Convert();

            if(VCC_voltage > VCC_7_0) {            
                G4_LED_TOGGLE;
                R4_LED_OFF;
            }
            else {
                if(VCC_voltage > VCC_6_8) {
                    G4_LED_TOGGLE;
                    if((LED_OUT & G4_LED_BIT) == 0) {
                        LED_OUT |= R4_LED_BIT;          /* R4 = ~G4 */
                    }
                    else {
                        LED_OUT &= ~R4_LED_BIT;         /* R4 = ~G4 */
                    }
                }
                else {
                    if(VCC_voltage > VCC_6_7) {
                        R4_LED_TOGGLE;
                        G4_LED_OFF;
                    }
                    else {
                        G4_LED_OFF;
                        R4_LED_ON;
                    }
                }
            }
                    
            if(diff_timer > 0) {
                diff_timer -= 1;
            }
        }
        
    }

}

/* ---------- Get_Config_Switch ------------------------------------ */

/* Read TMR configuration.                                           */

void Get_Config_Switch(void) { 

    TMR_configured = (~CONFIG_IN) & CONFIG_MASK;
    TMR_configured = (TMR_configured & 0x02) |
                     ((TMR_configured >> 2) & 0x01) |
                     ((TMR_configured << 2) & 0x04);    /* swap bits 0-2 */
    
    /* ONLY FOR TESTING: */
//    TMR_configured = 0x01;                  /* submodule 1 configured */
    
    TMR_diff_mask = 0x07;
    
    if((TMR_configured & 0x01) == 0) {      /* submodule 1 = OFF      */
        TMR_diff_mask &= ~0x05;             /* diff12, diff13 exclude */
    } 
    if((TMR_configured & 0x02) == 0) {      /* submodule 2 = OFF      */
        TMR_diff_mask &= ~0x03;             /* diff12, diff23 exclude */
    } 
    if((TMR_configured & 0x04) == 0) {      /* submodule 3 = OFF      */
        TMR_diff_mask &= ~0x06;             /* diff23, diff13 exclude */
    }
       
}

/* ---------- Get_Submodule_CMD ------------------------------------ */

/* Read commands from submodules.                                    */

void Get_Submodule_CMD(void) { 

    SM1_cmd = (SMO1_IN & SMO1_MASK);        /* read submodule commands */
    SM2_cmd = (SMO2_IN & SMO2_MASK);
    SM3_cmd = ((SMO3_IN >> 4) & SMO3_MASK);
    
    /* synchronize submodules: In = LOW pulse */
    SMI_OUT &= ~SMI_MASK;                   /* all In -> L */
    _nop();                                 /* delay */
    _nop();                                 /* delay */
    _nop();                                 /* delay */
    _nop();                                 /* delay */
    _nop();                                 /* delay */
    SMI_OUT |= SMI_MASK;                    /* all In -> H */
    
}

/* ---------- Evaluate_Active_Submodules --------------------------- */

/*                                                                   */

void Evaluate_Active_Submodules(void) {

    lock_state = 0;                 /* evaluate state for every round */
        
    /* --- compare SM1 <-> SM2 --- */
    if(SM1_cmd == SM2_cmd) {        /* SM1 == SM2 */
        if( ((diff_timer == 0) && (dif12_count >= DIFF_LIMIT)) |
            ((dif12_count > 0) && (dif12_count < DIFF_LIMIT))   ) {
           dif12_count -= 1;        
        }
    }
    else {                          /* SM1 != SM2 */
        diff_state |= DIFF12;
        if(dif12_count < DIFF_LIMIT) {   /* count diff. until limit */
            dif12_count += 1;
        }
        if(dif12_count >= DIFF_LIMIT) {
            lock_state |= DIFF12;
            diff_timer = DIFF_TIMER_DELAY;
        }
    }

    /* --- compare SM2 <-> SM3 --- */
    if(SM2_cmd == SM3_cmd) {        /* SM2 == SM3 */
        if( ((diff_timer == 0) && (dif23_count >= DIFF_LIMIT)) |
            ((dif23_count > 0) && (dif23_count < DIFF_LIMIT))   ) {
           dif23_count -= 1;        
        }
    }
    else {                          /* SM2 != SM3 */
        diff_state |= DIFF23;
        if(dif23_count < DIFF_LIMIT) {   /* count diff. until limit */
            dif23_count += 1;
        }
        if(dif23_count >= DIFF_LIMIT) {
            lock_state |= DIFF23;
            diff_timer = DIFF_TIMER_DELAY;
        }
    }

    /* --- compare SM1 <-> SM3 --- */
    if(SM1_cmd == SM3_cmd) {        /* SM1 == SM3 */
        if( ((diff_timer == 0) && (dif13_count >= DIFF_LIMIT)) |
            ((dif13_count > 0) && (dif13_count < DIFF_LIMIT))   ) {
           dif13_count -= 1;        
        }
    }
    else {                          /* SM1 != SM3 */
        diff_state |= DIFF13;
        if(dif13_count < DIFF_LIMIT) {   /* count diff. until limit */
            dif13_count += 1;
        }
        if(dif13_count >= DIFF_LIMIT) {
            lock_state |= DIFF13;
            diff_timer = DIFF_TIMER_DELAY;
        }
    }
    
    if(dif12_count >= DIFF_LIMIT) {
        lock_state |= DIFF12;
    }
    if(dif23_count >= DIFF_LIMIT) {
        lock_state |= DIFF23;
    }
    if(dif13_count >= DIFF_LIMIT) {
        lock_state |= DIFF13;
    }

    lock_state = lock_state & TMR_diff_mask;    /* consider only configured submod. */

    /* --- evaluate active submodules --- */
    
    switch(lock_state) {                /* check comparision results */
        case 0x00:                      /* all submodules OK */
                    TMR_active = 0x07;
                    break;
        case DIFF12:                    /* possible only when 2 modules configured */
                    TMR_active = 0x00;  /* both are bad */                   
                    break;
        case DIFF23:                    /* possible only when 2 modules configured */
                    TMR_active = 0x00;  /* both are bad */                   
                    break;
        case DIFF13:                    /* possible only when 2 modules configured */
                    TMR_active = 0x00;  /* both are bad */                   
                    break;
        case (DIFF12+DIFF13):           /* submodule 1 bad */
                    TMR_active = 0x06;  /* submodule 1 out */                                       
                    break;
        case (DIFF12+DIFF23):           /* submodule 2 bad */
                    TMR_active = 0x05;  /* submodule 2 out */                                       
                    break;
        case (DIFF13+DIFF23):           /* submodule 3 bad */
                    TMR_active = 0x03;  /* submodule 3 out */                                       
                    break;
        case (DIFF12+DIFF23+DIFF13):    /* all bad ? */
                    TMR_active = 0x00;  /* submodule 1 out */                                       
                    break;
        default:                        /* ??? */
                    TMR_active = 0x00;  /* ??? */                                       
                    break;
    }
    
    /* consider only configured submodules */
    TMR_active = TMR_active & TMR_configured;
    
    /* LED indication */
    G1_LED_OFF;
    R1_LED_OFF;
    if((TMR_configured & 0x01) != 0) {  /* submodule 1 configured ON */
        if((TMR_active & 0x01) != 0) {  /* submodule OK */
            G1_LED_ON;
        }
        else {                          /* submodule bad */
            R1_LED_ON;
        }
    }
        
    G2_LED_OFF;
    R2_LED_OFF;
    if((TMR_configured & 0x02) != 0) {  /* submodule 2 configured ON */
        if((TMR_active & 0x02) != 0) {  /* submodule OK */
            G2_LED_ON;
        }
        else {                          /* submodule bad */
            R2_LED_ON;
        }
    }
        
    G3_LED_OFF;
    R3_LED_OFF;
    if((TMR_configured & 0x04) != 0) {  /* submodule 3 configured ON */
        if((TMR_active & 0x04) != 0) {  /* submodule OK */
            G3_LED_ON;
        }
        else {                          /* submodule bad */
            R3_LED_ON;
        }
    }
        
        
}

/* ---------- Exec_Submodule_CMD ----------------------------------- */

/* Execute.                                                          */

void Exec_Submodule_CMD(void) {

    switch(TMR_active) {

        case 0x01:              /* only submodule 1 */
        case 0x03:              /* submodules 1 + 2 */
        case 0x05:              /* submodules 1 + 3 */
        case 0x07:              /* submodules 1 + 2 + 3 */
            active_cmd = SM1_cmd;
            break;

        case 0x02:              /* only submodule 2 */
        case 0x06:              /* submodules 2 + 3 */
            active_cmd = SM2_cmd;
            break;
            
        case 0x04:              /* only submodule 3 */
            active_cmd = SM3_cmd;
            break;
            
        case 0x00:              /* no submodule */
            active_cmd = STOP_BOTH;  /* STOP */
            break;
    }        
        
    switch(active_cmd) {
        
        case 0x00:              /* both stop */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_STOP;
            R_MOT_STOP;
            break;

        case 0x01:              /* left 1, right 0 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_BACK;
            L_MOT_FULL;
//            R_MOT_STOP;
            R_MOT_MIN;
            break;
               
        case 0x02:              /* left 0, right 1 */
            L_MOT_BACK;
            R_MOT_FORWARD;
            L_MOT_MIN;
//            L_MOT_STOP;
            R_MOT_FULL;
            break;
               
        case 0x03:              /* left 1, right 1 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_FULL;
            R_MOT_FULL;
            break;
               
        case 0x05:              /* left 1, right 0.5 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_FULL;
            R_MOT_SLOW;
            break;
               
        case 0x06:              /* left 0.5, right 1 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_SLOW;
            R_MOT_FULL;
            break;
               
        case 0x07:              /* left 1, right 1 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_FULL;
            R_MOT_FULL;
            break;
               
        case 0x04:              /* left 0.5, right 0.5 */
            L_MOT_FORWARD;      /* run only forward */
            R_MOT_FORWARD;
            L_MOT_SLOW;
            R_MOT_SLOW;
            break;
            
        default:                /* ??? */
            break;
    }          
               
}

/* ---------- END -------------------------------------------------- */

/*
 * commands.c
 *
 *  Created on: 19. 5. 2020
 *      Author: pivov
 */

#include "commands.h"

void execute(unsigned char com, unsigned char comL, unsigned char comR){
    switch(com){
    case 0x00:
        //L_MOT_BREAK;
        //R_MOT_BREAK;
        L_MOT_STOP;
        R_MOT_STOP;
        break;
    case 0x01:
        L_MOT_FORWARD;
        R_MOT_FORWARD;
        TBCCR1 = comL * 100;
        TBCCR2 = comR * 100 + r_offset;
        break;
    case 0x02:
        L_MOT_BACK;
        R_MOT_BACK;
        TBCCR1 = comL * 100;
        TBCCR2 = comR * 100 + r_offset;
        break;
    case 0x03:
        follow();
        break;
    }
}

void follow(){
    P3OUT |= 0x10;
    Get_Config_Switch();            /* read TMR configuration */
    Get_Submodule_CMD();            /* read data from submodules */
    Evaluate_Active_Submodules();   /* evaluate which submod. are active */
    Exec_Submodule_CMD();           /* execute CMD */
    P3OUT &= ~0x10;
}

/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   main.h                                           */
/*                                                                    */
/*          Procedures for Basic Clock Module MSP430x2xx              */
/*                                                                    */
/* ------------------------------------------------------------------ */

#define     DIFF12       0x01
#define     DIFF23       0x02
#define     DIFF13       0x04

#define     DIFF_LIMIT  20          /* diff. limit for submodule locking */
#define     DIFF_TIMER_DELAY  20    /* reset differences after 20 sec. */

#define     STOP_BOTH   0x00        /* command STOP_BOTH from submodules */

#define     VCC_7_0     2050        /* VCC = 7.00 V */
#define     VCC_6_8     1993        /* VCC = 6.80 V */
#define     VCC_6_7     1957        /* VCC = 6.70 V */

/* ------------------------------------------------------------------ */

void Hardware_Init(void) ;
   
void Get_Config_Switch(void) ; 

void Get_Submodule_CMD(void) ; 

void Evaluate_Active_Submodules(void) ;

void Exec_Submodule_CMD(void) ; 

/* ---------- END -------------------------------------------------- */


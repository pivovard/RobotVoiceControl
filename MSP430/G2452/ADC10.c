/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   ADC10.h                                          */
/*                                                                    */
/*          Procedures for 10 bit ADC                                 */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */


/* ---------- ADC_Start_Scan --------------------------------------- */

void ADC10_Start_Scan(unsigned short data_addr) {

    ADC10CTL0 &= ~ENC;                                  /* clear ENC */
    
    ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;             /* 16x CLK, multiple, ADC ON */
    ADC10AE0 |= 0x07;                                   /* analog input enable */
    ADC10CTL1 = (0x02 << 12) | ADC10SSEL_3 | CONSEQ_1;  /* SMCLK : 1, sequence 1x */
    
    ADC10DTC0 = 0x00;                                   /* one block 1x */
    ADC10DTC1 = 0x03;                                   /* number of transfers */
    ADC10SA = data_addr;                                /* start address */
    
    ADC10CTL0 |= ENC;                                   /* set ENC */
    ADC10CTL0 |= ADC10SC;                               /* start conversion */
    
    while((ADC10CTL0 & ADC10IFG) == 0) {;}              /* wait until block end */

}

/* ---------- END -------------------------------------------------- */


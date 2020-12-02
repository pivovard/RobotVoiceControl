/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   ADC12.h                                          */
/*                                                                    */
/*          Procedures for 12 bit ADC                                 */
/*                                                                    */
/* ------------------------------------------------------------------ */

#include    "includes.h"

/* ------------------------------------------------------------------ */


/* ---------- ADC_Single_Init ------------------ -------------------- */

/* Init ADC12 for single conversion using AVCC as reference.          */

void ADC12_Single_Init(unsigned char channel) {
    
    P6SEL |= (0x01 << channel);                                      /* PORT = FUNCTION */
    P6DIR &= ~(0x01 << channel);                                     /* input */

    ADC12CTL0 &= ~ENC;                                  /* clear ENC */
    
    ADC12MCTL0 = EOS | SREF_0 | (channel & 0x0F);       /* memory control */

    ADC12CTL0 = SHT1_8 | SHT0_8 | ADC12ON ;             /* 256x CLK, ADC ON */
    ADC12CTL1 = CSTARTADD_0 | SHS_0 | SHP | ADC12DIV_0 | ADC12SSEL_3; /* memory 0, sw start, :1, SMCLK */
    ADC12IFG = 0x00;
        
}

/* ---------- ADC_Single_Init_2 --------------------------------------- */

/* Init ADC12 for single conversion using VREF as reference.            */

void ADC12_Single_Init_2(unsigned char channel) {
    
    P6SEL |= (0x01 << channel);                                      /* PORT = FUNCTION */
    P6DIR &= ~(0x01 << channel);                                     /* input */

    ADC12CTL0 &= ~ENC;                                  /* clear ENC */
    
    ADC12MCTL0 = EOS | SREF_1 | (channel & 0x0F);       /* memory control */

    ADC12CTL0 = SHT1_8 | SHT0_8 | ADC12ON | REFON | REF2_5V;          /* 256x CLK, ADC ON, VREF 2.5V */
    ADC12CTL1 = CSTARTADD_0 | SHS_0 | SHP | ADC12DIV_0 | ADC12SSEL_3; /* memory 0, sw start, :1, SMCLK */
    ADC12IFG = 0x00;
        
}

/* ---------- ADC_Single_Convert -------------------------------------- */

unsigned short ADC12_Single_Convert(void) {
    
unsigned long   adc_tmp;

    ADC12IFG &= 0x0001;                                 /* clear IFG */    
    ADC12CTL0 |= ENC;                                   /* set ENC */
    ADC12CTL0 |= ADC12SC;                               /* start conversion */
    
    while((ADC12IFG & 0x0001) == 0) {;}                 /* wait until conversion end */

    adc_tmp = ((unsigned long)(ADC12MEM0) * VREF) / ADCREF;
//    adc_tmp = (unsigned long)(ADC12MEM0);
        
    return((unsigned short)adc_tmp);
    
}

/* ---------- END -------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   ADC12.h                                          */
/*                                                                    */
/*          Procedures for 12 bit ADC                                 */
/*                                                                    */
/* ------------------------------------------------------------------ */

#define         VREF    3142            /* ref.voltage in mV */
#define         ADCREF  4080            /* ADC value for ref.voltage */
 
/* ------------------------------------------------------------------ */

void ADC12_Single_Init(unsigned char channel) ;

void ADC12_Single_Init_2(unsigned char channel) ;

unsigned short ADC12_Single_Convert(void) ;
    
/* ---------- END -------------------------------------------------- */


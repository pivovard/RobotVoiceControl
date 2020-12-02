/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   USCI_A.h                                         */
/*                                                                    */
/*          Procedures for USCI_A settings                            */
/*                                                                    */
/* ------------------------------------------------------------------ */


#define     UCA0_PORT_SEL   P1SEL
#define     UCA0_PORT_SEL2  P1SEL2
#define     UCA0_PORT_DIR   P1DIR

#define     MISO_BIT        0x02
#define     MOSI_BIT        0x04
#define     SCK_BIT         0x10

    
void UCA0_SPI_Init(void) ;

void UCA0_SPI_Send_Short(unsigned short tx_data) ;

/* ---------- END -------------------------------------------------- */


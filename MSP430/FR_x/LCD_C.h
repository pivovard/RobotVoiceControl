/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                   LCD_C.h                                          */
/*                                                                    */
/*          Include file for LCD_C.c                                  */
/*                                                                    */
/* ------------------------------------------------------------------ */

#ifndef __LCD_C__

#define __LCD_C__

/* ---------- --- -------------------------------------------------- */
#define LCD_SCROLL_SPEED    20      /* scroll speed in 15.625 ms multiply */
/* ---------- --- -------------------------------------------------- */

/* define segment position for 6 LCD digits */
#define POS1 			9   /* Digit A1 begins at S18 */
#define POS2 			5   /* Digit A2 begins at S10 */
#define POS3 			3   /* Digit A3 begins at S6  */
#define POS4 			18  /* Digit A4 begins at S36 */
#define POS5 			14  /* Digit A5 begins at S28 */
#define POS6 			7   /* Digit A6 begins at S14 */

/* define segment segment mask for first and second memory line to mask LCD digits */
#define DIGIT_MASK_1 	0xFF   /* the first line */
#define DIGIT_MASK_2 	0xFA   /* the second line */

/* define LCD mark codes */
#define TMR 			0    /* timer mark */
#define HRT 			1    /* heart mark */
#define REC 			2    /* record mark */
#define ANT 			3    /* antenne mark */
#define TX 				4    /* TX mark */
#define RX 				5    /* RX mark */
#define BATT 			6    /* battery mark */
#define BAT1 			7    /* leftmost batt.segment mark */
#define BAT2 			8    /* batt.segment mark */
#define BAT3 			9    /* batt.segment mark */
#define BAT4 			10   /* batt.segment mark */
#define BAT5 			11   /* batt.segment mark */
#define BAT6 			12   /* rightmost batt.segment mark */
#define BRACKET 		13   /* battery bracket mark */
#define A1DP 			14   /* dec. point mark */
#define A2DP 			15   /* dec. point mark */
#define A3DP 			16   /* dec. point mark */
#define A4DP 			17   /* dec. point mark */
#define A5DP 			18   /* dec. point mark */
#define NEG 			19   /* minus mark */
#define A2COL 			20   /* collon mark */
#define A4COL 			21   /* collon mark */
#define DEG 			22   /* DEG mark */
#define EXCL 			23   /* ! mark */

/* ---------- prototypes ------------------------------------------- */

int8_t LCD_C_init(void) ;

int8_t LCD_C_disp_char(char c, uint8_t pos) ;

int8_t LCD_C_disp_char_blink(char c, uint8_t pos) ;

int8_t LCD_C_clr_char(uint8_t pos) ;

int8_t LCD_C_mark_set(uint8_t mark) ;

int8_t LCD_C_mark_set_blink(uint8_t mark) ;

int8_t LCD_C_mark_clr(uint8_t mark) ;

void LCD_C_display_time_hms(struct RTC_strx *tm) ;

void LCD_C_display_time_hm(struct RTC_strx *tm) ;

int8_t LCD_C_disp_string(char *str) ;

int8_t LCD_C_disp_string_stop(void) ;

int8_t LCD_C_disp_string_callback(void) ;

/* ---------- --- -------------------------------------------------- */

#endif

/* ---------- END -------------------------------------------------- */


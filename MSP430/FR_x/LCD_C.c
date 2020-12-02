/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                            LCD_C.c                                 */
/*                                                                    */
/*                    LCD_C control functions                         */
/*                                                                    */
/* ------------------------------------------------------------------ */


/* ---------- --- -------------------------------------------------- */


#include <string.h>
#include "includes.h"

#include "globals.c"        /* global variables */

/* ---------- LCD character definitions ---------------------------- */

/* Position of segment digits in LCD memory                          */

const uint8_t seg_pos[6] =
{
    9,  	/* digit A1 */
    5,  	/* digit A2 */
    3,  	/* digit A3 */
    18,  	/* digit A4 */
    14,  	/* digit A5 */
    7,  	/* digit A6 */
};

/* LCD memory map for numeric digits                                 */

const uint8_t digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

const uint8_t extra[2][2] =
{
    {0x03, 0x00},  /* "-" big - on digit position */
    {0x03, 0x50},  /* "+" big + on digit position */
};

/* LCD memory map for uppercase letters                              */

const uint8_t alphabet_up[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

/* LCD memory map for marks                                          */

const uint8_t mark_map[24] =
{
    0x08 ,  		/* "TMR" */
    0x04 ,  		/* "HRT" */
    0x02 ,  		/* "REC" */
    0x04 ,  		/* "ANT" */
    0x04 ,  		/* "TX" */
    0x01 ,  		/* "RX" */
    0x10 ,  		/* "BATT" */
    0x20 ,  		/* "B1" */
    0x20 ,  		/* "B2" */
    0x40 ,  		/* "B3" */
    0x40 ,  		/* "B4" */
    0x80 ,  		/* "B5" */
    0x80 ,  		/* "B6" */
    0x10 ,  		/* "BRACKET" */
    0x01 ,  		/* "A1DP" */
    0x01 ,  		/* "A2DP" */
    0x01 ,  		/* "A3DP" */
    0x01 ,  		/* "A4DP" */
    0x01 ,  		/* "A5DP" */
    0x04 ,  		/* "NEG" */
    0x04 ,  		/* "A2COL" */
    0x04 ,  		/* "A4COL" */
    0x04 ,  		/* "DEG" */
    0x01   		    /* "EXCL" */
};

/* LCD memory offset for marks                                       */

const uint8_t mark_offset[24] =
{
     2 ,  		/* "TMR" */
     2 ,  		/* "HRT" */
     2 ,  		/* "REC" */
     4 ,  		/* "ANT" */
     8 ,  		/* "TX" */
     8 ,  		/* "RX" */
     13 ,  		/* "BATT" */
     17 ,  		/* "B1" */
     13 ,  		/* "B2" */
     17 ,  		/* "B3" */
     13 ,  		/* "B4" */
     17 ,  		/* "B5" */
     13 ,  		/* "B6" */
     17 ,  		/* "BRACKET" */
     10 ,  		/* "A1DP" */
     6 ,  		/* "A2DP" */
     4 ,  		/* "A3DP" */
     19 ,  		/* "A4DP" */
     15 ,  		/* "A5DP" */
     10 ,  		/* "NEG" */
     6 ,  		/* "A2COL" */
     19 ,  		/* "A4COL" */
     15 ,  		/* "DEG" */
     2   		/* "EXCL" */
};

/* ---------- variables -------------------------------------------- */

uint8_t			str_len;		/* length of displayed string */
uint8_t 	    str_ptr;		/* pointer to the first not displayed char */
char        *LCD_string;    /* pointer to string to be displayed */

/* ---------- LCD_C_init ------------------------------------------- */

/* Init LCD_C module.                                                */

int8_t LCD_C_init(void) {

	LCDCCTL0 = 0x041F;		/* div 1, pre 16, ACLK, 4mux, low power */
	LCDCCTL1 = 0x0000;		/* int. disable */

	LCDCBLKCTL = 0x0019;	/* div 1, pre 32768, blink enabled */
	LCDCMEMCTL = 0x0006;	/* clear both mem */
	LCDCVCTL = 0x1008;		/* internal, Vlcd = 3.02 V */

	LCDCPCTL0 = 0xFFD0;
	LCDCPCTL1 = 0xF83F;
	LCDCPCTL2 = 0x00F8;

	LCDCCPCTL = 0x8000;		/* ch. pump = synchronous, no module disable ch. p. */

    return(0);
}

/* ---------- LCD_C_disp_char -------------------------------------- */

/* Display char on LCD position pos (in range 0..5)                  */

int8_t LCD_C_disp_char(char c, uint8_t pos) {

int8_t      rc;     /* return code */
uint8_t     sp;

    rc = 1;
    sp = seg_pos[pos];

	if(('0' <= c) && ('9' >= c)) {
		/* write to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
        		                | (digit[c - '0'][0] & DIGIT_MASK_1);
        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
        		                | (digit[c - '0'][1] & DIGIT_MASK_2);
        rc = 0;
	}

    if(('A' <= c) && ('Z' >= c)) {
        /* write to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
                                | (alphabet_up[c - 'A'][0] & DIGIT_MASK_1);
        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                | (alphabet_up[c - 'A'][1] & DIGIT_MASK_2);
        rc = 0;
    }

    /* display some special chars */
    switch(c) {
        case ' ':               /* whitespace */
            /* write to memory, preserve mark bits */
            LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1));
            LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2));
            rc = 0;
            break;

        case '-':               /* big - */
            /* write to memory, preserve mark bits */
            LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
                                    | (extra[0][0] & DIGIT_MASK_1);
            LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                    | (extra[0][1] & DIGIT_MASK_2);
            rc = 0;
            break;

        case '+':               /* big + */
            /* write to memory, preserve mark bits */
            LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
                                    | (extra[1][0] & DIGIT_MASK_1);
            LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                    | (extra[1][1] & DIGIT_MASK_2);
            rc = 0;
            break;

        default:
            break;
    }

    /* clear blink memory, do not change mark bits */
    LCDBMEM[sp] = (LCDBMEM[sp] & (~DIGIT_MASK_1));
    LCDBMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2));

    return(rc);
}

/* ---------- LCD_C_disp_char_blink -------------------------------- */

/* Display char on LCD position pos (in range 0..5) in blinking mode */

int8_t LCD_C_disp_char_blink(char c, uint8_t pos) {

int8_t      rc;     /* return code */
uint8_t     sp;

    rc = 1;
    sp = seg_pos[pos];

    if(('0' <= c) && ('9' >= c)) {
        /* write to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
                                | (digit[c - '0'][0] & DIGIT_MASK_1);
        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                | (digit[c - '0'][1] & DIGIT_MASK_2);
        rc = 0;
    }

    if(('A' <= c) && ('Z' >= c)) {
        /* write to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1))
                                | (alphabet_up[c - 'A'][0] & DIGIT_MASK_1);
        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                | (alphabet_up[c - 'A'][1] & DIGIT_MASK_2);
        rc = 0;
    }

    if(' ' == c) {
        /* write to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1));
        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2));
        rc = 0;
    }

    if(rc == 0) {
        /* write to blink memory, do not change mark bits */
        LCDBMEM[sp] = (LCDBMEM[sp] & (~DIGIT_MASK_1))
                                | (LCDMEM[sp] & DIGIT_MASK_1);
        LCDBMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2))
                                | (LCDMEM[sp + 1] & DIGIT_MASK_2);
    }

    return(rc);

}

/* ---------- LCD_C_clr_char --------------------------------------- */

/* Clear LCD char on position pos (in range 0..5)                    */

int8_t LCD_C_clr_char(uint8_t pos) {

uint8_t     sp;

	if(5 >= pos) {

        sp = seg_pos[pos];

        /* write 0 to memory, preserve mark bits */
        LCDMEM[sp] = (LCDMEM[sp] & (~DIGIT_MASK_1));
        LCDBMEM[sp] = (LCDBMEM[sp] & (~DIGIT_MASK_1));

        LCDMEM[sp + 1] = (LCDMEM[sp + 1] & (~DIGIT_MASK_2));
        LCDBMEM[sp + 1] = (LCDBMEM[sp + 1] & (~DIGIT_MASK_2));

        return(0);
	}
	else {
		return(1);
	}

}

/* ---------- LCD_C_mark_set --------------------------------------- */

/* Set picture mark on LCD.                                          */

int8_t LCD_C_mark_set(uint8_t mark) {

	if(23 >= mark) {
		LCDMEM[mark_offset[mark]] |= mark_map[mark];
        LCDBMEM[mark_offset[mark]] &= ~(mark_map[mark]);  /* clear blinking */
	}
	else {
		return(1);
	}

	return(0);

}

/* ---------- LCD_C_mark_set_blink --------------------------------- */

/* Set LCD mark in blink mode.                                       */

int8_t LCD_C_mark_set_blink(uint8_t mark) {

	if(23 >= mark) {
		LCDMEM[mark_offset[mark]] |= mark_map[mark];
		LCDBMEM[mark_offset[mark]] |= mark_map[mark];
	}
	else {
		return(1);
	}

	return(0);

}

/* ---------- LCD_C_mark_clr --------------------------------------- */

/* Clear picture mark on LCD.                                        */

int8_t LCD_C_mark_clr(uint8_t mark) {

	if(23 >= mark) {
		LCDMEM[mark_offset[mark]] &= ~(mark_map[mark]);
		LCDBMEM[mark_offset[mark]] &= ~(mark_map[mark]);
	}
	else {
		return(1);
	}

	return(0);

}

/* ---------- LCD_C_disp_string ------------------------------------ */

/* Init string displaying. If strlen > 6, callback shifts LCD text   */
/* one char to the left. String display have to be stopped by        */
/* LCD_C_displ_string_stop().                                        */

int8_t LCD_C_disp_string(char *str) {

uint8_t		tmp;
uint8_t		i;

    LCD_string = str;               /* for callback function */
	str_len = strlen(str);			/* lenght of displayed string */
	str_ptr = 0;					/* pointer to the first char */

	if(str_len <= 6) {
		tmp = str_len;
	}
	else {
		tmp = 6;
	}

	/* display max. 6 first chars */
	for(i = 0; i < tmp; i++) {
	    LCD_C_clr_char(i);                  /* clear previous char */
	    LCD_C_disp_char(str[str_ptr], i);
	    str_ptr += 1;
	}

	/* start timer to tick LCD shift */
    if(str_len > 6) {
        LCD_string_timer_start(LCD_SCROLL_SPEED);
    }

    return(0);

}

/* ---------- LCD_C_disp_string_stop ------------------------------- */

/* Init string displaying. If strlen > 6, callback shifts LCD text   */
/* one char to the left. String display have to be stopped by        */
/* LCD_C_displ_string_stop().                                        */

int8_t LCD_C_disp_string_stop(void) {

    LCD_string_timer_stop();    /* stop LCD shifting timer */

    /* ??? clear LCD here ? */

    return(0);
}

/* ---------- LCD_C_disp_string_callback --------------------------- */

/* Shift LCD text one position to the left. Called from main() after */
/* LCD timer interrupt.                                              */

int8_t LCD_C_disp_string_callback(void) {

uint8_t     i;

    /* shift LCD chars left */
    for(i = 0; i < 5; i++) {
        /* copy only char segments */
        LCDMEM[seg_pos[i]] = LCDMEM[seg_pos[i]] & ~DIGIT_MASK_1
                             | LCDMEM[seg_pos[i + 1]] & DIGIT_MASK_1;
        LCDMEM[seg_pos[i]+1] = LCDMEM[seg_pos[i]+1] & ~DIGIT_MASK_2
                             | LCDMEM[seg_pos[i + 1]+1] & DIGIT_MASK_2;
    }

    /* check string roll */
    if(str_ptr >= str_len) {
        str_ptr = 0;
    }

    LCD_C_disp_char(LCD_string[str_ptr], i);
    str_ptr += 1;

    return(0);
}

/* ---------- LCD_C_display_time_hms ------------------------------- */

/* Display time in format HH:MM:SS on LCD.                           */

void LCD_C_display_time_hms(struct RTC_strx *tm) {

uint8_t     ts[6];
uint8_t     i;

    /* convert hour */
    ts[1] = (tm->hour % 10) + '0';
    ts[0] = (tm->hour / 10) + '0';

    /* convert min */
    ts[3] = ((tm->min % 10) + 0x30);
    ts[2] = ((tm->min / 10) + 0x30);

    /* convert sec */
    ts[5] = ((tm->sec % 10) + '0');
    ts[4] = ((tm->sec / 10) + '0');

    for(i = 0; i < 6; i++) {
        LCD_C_disp_char(ts[i], i);
    }

    if(tm->signum == 0) {
        LCD_C_mark_clr(NEG);
    }
    else {
        LCD_C_mark_set(NEG);
    }

    LCD_C_mark_set(A2COL);
    LCD_C_mark_set(A4COL);

//    return(0);

}

/* ---------- LCD_C_display_time_hm -------------------------------- */

/* Display time in format HH:MM on LCD.                              */

void LCD_C_display_time_hm(struct RTC_strx *tm) {

uint8_t         ts[4];
uint8_t         i;

    /* convert hour */
    ts[1] = (tm->hour % 10) + '0';
    ts[0] = (tm->hour / 10) + '0';

    /* convert min */
    ts[3] = (tm->min % 10) + '0';
    ts[2] = (tm->min / 10) + '0';

    for(i = 0; i < 4; i++) {
        LCD_C_disp_char(ts[i], i + 2);
    }

    if(tm->signum == 0) {
        LCD_C_mark_clr(NEG);
    }
    else {
        LCD_C_mark_set(NEG);
    }

    LCD_C_mark_set_blink(A4COL);

//    return(0);

}

/* ---------- END -------------------------------------------------- */


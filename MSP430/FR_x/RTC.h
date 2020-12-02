/* ------------------------------------------------------------------ */
/*                                                                    */
/*                                                                    */
/*                         RTC.h                                      */
/*                                                                    */
/*                 Include file for RTC                               */
/*                                                                    */
/* ------------------------------------------------------------------ */

#ifndef __RTC__

#define __RTC__

/* ---------- --- -------------------------------------------------- */

#define RTC_ADD         0       /* operation code for RTC_time_addsub */
#define RTC_SUB         1       /* operation code for RTC_time_addsub */
#define RTC_CMP         2       /* operation code for RTC_time_addsub */

/* ---------- --- -------------------------------------------------- */

struct  RTC_strx_full {
    uint8_t     signum;     /* time is negative if signum != 0 */
    uint16_t    year;
    uint8_t     month;
    uint8_t     day;
    uint8_t     dow;
    uint8_t     hour;
    uint8_t     min;
    uint8_t     sec;        /* MSB can be 1 to indicate negative date */
};

struct  RTC_strx {
    uint8_t     signum;     /* time is negative if signum != 0 */
    uint8_t     dow;
    uint8_t     hour;
    uint8_t     min;
    uint8_t     sec;        /* MSB can be 1 to indicate negative date */
};

/* ---------- prototypes ------------------------------------------- */

int RTC_init(void) ;

int8_t RTC_time_math(struct RTC_strx *a,
                    struct RTC_strx *b,
                    struct RTC_strx *c,
                    uint8_t operation)      ;

uint16_t RTC_sum(struct RTC_strx *as) ;

__interrupt void RTC_1min_isr(void) ;

/* ---------- --- -------------------------------------------------- */

#endif

/* ---------- END -------------------------------------------------- */


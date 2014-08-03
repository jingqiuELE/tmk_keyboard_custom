#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
#include "softpwm_led.h"
#include "debug.h"

#define SOFTPWM_LED_FREQ 64
#define SOFTPWM_LED_TIMER_TOP F_CPU / (256 * SOFTPWM_LED_FREQ)

static uint8_t softpwm_led_state = 0;
static uint8_t softpwm_led_ocr[LED_COUNT] = {0};
static uint8_t softpwm_led_ocr_buff[LED_COUNT] = {0};

void softpwm_init(void)
{
#ifdef SOFTPWM_LED_TIMER3
    /* Timer3 setup */
    /* CTC mode */
    TCCR3B |= (1<<WGM32);
    /* Clock selelct: clk/8 */
    TCCR3B |= (1<<CS30);
    /* Set TOP value */
    uint8_t sreg = SREG;
    cli();
    OCR3AH = (SOFTPWM_LED_TIMER_TOP >> 8) & 0xff;
    OCR3AL = SOFTPWM_LED_TIMER_TOP & 0xff;
    SREG = sreg;
#else
    /* Timer1 setup */
    /* CTC mode */
    TCCR1B |= (1<<WGM12);
    /* Clock selelct: clk/8 */
    TCCR1B |= (1<<CS10);
    /* Set TOP value */
    uint8_t sreg = SREG;
    cli();
    OCR1AH = (SOFTPWM_LED_TIMER_TOP >> 8) & 0xff;
    OCR1AL = SOFTPWM_LED_TIMER_TOP & 0xff;
    SREG = sreg;
#endif
    softpwm_led_init();
}

void softpwm_led_enable(void)
{
    /* Enable Compare Match Interrupt */
#ifdef SOFTPWM_LED_TIMER3
    TIMSK3 |= (1<<OCIE3A);
    //dprintf("softpwm led on: %u\n", TIMSK3 & (1<<OCIE3A));
#else
    TIMSK1 |= (1<<OCIE1A);
    //dprintf("softpwm led on: %u\n", TIMSK1 & (1<<OCIE1A));
#endif
    softpwm_led_state = 1;
#ifdef LEDMAP_ENABLE
    softpwm_led_state_change(softpwm_led_state);
#endif
}

void softpwm_led_disable(void)
{
    /* Disable Compare Match Interrupt */
#ifdef SOFTPWM_LED_TIMER3
    TIMSK3 &= ~(1<<OCIE3A);
    //dprintf("softpwm led off: %u\n", TIMSK3 & (1<<OCIE3A));
#else
    TIMSK1 &= ~(1<<OCIE1A);
    //dprintf("softpwm led off: %u\n", TIMSK1 & (1<<OCIE1A));
#endif
    softpwm_led_state = 0;
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        softpwm_led_off(i);
    }
#ifdef LEDMAP_ENABLE
    softpwm_led_state_change(softpwm_led_state);
#endif
}

void softpwm_led_toggle(void)
{
    if (softpwm_led_state) {
        softpwm_led_disable();
    }
    else {
        softpwm_led_enable();
    }
}

void softpwm_led_set(uint8_t index, uint8_t val)
{
    softpwm_led_ocr_buff[index] = val;
}

void softpwm_led_set_all(uint8_t val)
{
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        softpwm_led_ocr_buff[i] = val;
    }
}

inline uint8_t softpwm_led_get_state(void)
{
    return softpwm_led_state;
}

#ifdef BREATHING_LED_ENABLE

/* Breathing LED brighness(PWM On period) table
 *
 * http://www.wolframalpha.com/input/?i=Table%5Bfloor%28%28exp%28sin%28x%2F256*2*pi%2B3%2F2*pi%29%29-1%2Fe%29*%28256%2F%28e-1%2Fe%29%29%29%2C+%7Bx%2C0%2C255%2C1%7D%5D
 * Table[floor((exp(sin(x/256*2*pi+3/2*pi))-1/e)*(256/(e-1/e))), {x,0,255,1}]
 * (0..255).each {|x| print ((exp(sin(x/256.0*2*PI+3.0/2*PI))-1/E)*(256/(E-1/E))).to_i, ', ' }
 */
static const uint8_t breathing_table[256] PROGMEM = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 10, 11, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 32, 34, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 56, 58, 61, 63, 66, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 98, 102, 105, 108, 112, 116, 119, 123, 126, 130, 134, 138, 142, 145, 149, 153, 157, 161, 165, 169, 173, 176, 180, 184, 188, 192, 195, 199, 203, 206, 210, 213, 216, 219, 223, 226, 228, 231, 234, 236, 239, 241, 243, 245, 247, 248, 250, 251, 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 254, 253, 252, 251, 250, 248, 247, 245, 243, 241, 239, 236, 234, 231, 228, 226, 223, 219, 216, 213, 210, 206, 203, 199, 195, 192, 188, 184, 180, 176, 173, 169, 165, 161, 157, 153, 149, 145, 142, 138, 134, 130, 126, 123, 119, 116, 112, 108, 105, 102, 98, 95, 92, 89, 86, 83, 80, 77, 74, 71, 68, 66, 63, 61, 58, 56, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 34, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 11, 10, 9, 8, 8, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static led_state_t breathing_led_state = 0;
static uint8_t breathing_led_duration[LED_COUNT] = {0};

void breathing_led_enable(uint8_t index)
{
    LED_BIT_SET(breathing_led_state, index);
}

void breathing_led_enable_all(void)
{
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        LED_BIT_SET(breathing_led_state, i);
    }
}

void breathing_led_disable(uint8_t index)
{
    LED_BIT_CLEAR(breathing_led_state, index);
}

void breathing_led_disable_all(void)
{
    breathing_led_state = 0;
}

void breathing_led_toggle(uint8_t index)
{
    LED_BIT_XOR(breathing_led_state, index);
}

void breathing_led_toggle_all(void)
{
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        LED_BIT_XOR(breathing_led_state, i);
    }
}

void breathing_led_set_duration(uint8_t index, uint8_t dur)
{
    breathing_led_duration[index] = dur;
    //dprintf("breathing led set duration: %u\n", breathing_led_duration);
}

void breathing_led_set_duration_all(uint8_t dur)
{
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        breathing_led_duration[i] = dur;
    }
}

#endif

#ifdef SOFTPWM_LED_TIMER3
ISR(TIMER3_COMPA_vect)
#else
ISR(TIMER1_COMPA_vect)
#endif
{
    static uint8_t pwm = 0;
    pwm++;
    // LED on
    if (pwm == 0) {
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            softpwm_led_on(i);
            softpwm_led_ocr[i] = softpwm_led_ocr_buff[i];
        }
    }
    // LED off
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        if (pwm == softpwm_led_ocr[i]) {
            softpwm_led_off(i);
        }
    }

#ifdef BREATHING_LED_ENABLE
    static uint8_t count = 0;
    static uint8_t index[LED_COUNT] = {0};
    static uint8_t step[LED_COUNT] = {0};
    if (breathing_led_state) {
        if (++count > SOFTPWM_LED_FREQ) {
            count = 0;
            for (uint8_t i = 0; i < LED_COUNT; i++) {
                if (breathing_led_state & LED_BIT(i)) {
                    if (++step[i] > breathing_led_duration[i]) {
                        step[i] = 0;
                        softpwm_led_ocr_buff[i] = pgm_read_byte(&breathing_table[index[i]]);
                        index[i]++;
                    }
                }
            }
        }
    }
#endif
}

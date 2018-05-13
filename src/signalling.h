#ifndef __SIGNALLING_H__
#define __SIGNALLING_H__

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t signalling_init(void);

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
// time in milliseconds
void buzzer_beep(int time);
void led_init(void);
void led_on(void);
void led_off(void);
// time in milliseconds
void led_blink(int time);

#ifdef __cplusplus
}
#endif


#endif //__SIGNALLING_H__
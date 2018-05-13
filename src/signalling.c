
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "config.h"
#include "signalling.h"
#include <sys/unistd.h>


static const char *TAG = "signalling";

esp_err_t signalling_init(void)
{
    buzzer_init();
    buzzer_beep(100);
    led_init();
    led_blink(500);
    return ESP_OK;
}

void buzzer_init(void)
{
    gpio_pad_select_gpio(PIN_BUZZER);
    buzzer_off();
    gpio_set_direction(PIN_BUZZER, GPIO_MODE_OUTPUT);
}

inline void buzzer_on(void)
{
    gpio_set_level(PIN_BUZZER, 1);
}

inline void buzzer_off(void)
{
    gpio_set_level(PIN_BUZZER, 0);
}

// time in milliseconds
void buzzer_beep(int time)
{
    if (time > BUZZER_MAX_BEEP_TIME)
    {
        return;
    }
    buzzer_on();
    usleep(time * 1000);
    buzzer_off();
}

void led_init(void)
{
    gpio_pad_select_gpio(PIN_EXT_LED);
    led_off();
    gpio_set_direction(PIN_EXT_LED, GPIO_MODE_OUTPUT);
    gpio_set_drive_capability(PIN_EXT_LED, PIN_EXT_LED_DRIVE_STRENGTH);
}


inline void led_on(void)
{
    gpio_set_level(PIN_EXT_LED, (EXT_LED_ACTIVE_LOW != 0) ? 0 : 1);
}

inline void led_off(void)
{
    gpio_set_level(PIN_EXT_LED, (EXT_LED_ACTIVE_LOW != 0) ? 1 : 0);
}


// time in milliseconds
void led_blink(int time)
{
    led_on();
    usleep(time * 1000);
    led_off();
}

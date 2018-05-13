/*
    Nano Flight Computer
*/

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config.h"
#include "signalling.h"
#include "storage.h"
#include "sensors.h"

static const char *TAG = "main";

extern "C" {
	void app_main(void);
}

void app_main(void)
{
  signalling_init();
  if (storage_init() != ESP_OK)
  {
    ESP_LOGE(TAG, "Storage initialization failed");
    while(true)
    {
      buzzer_beep(200);
      usleep(200 * 1000);
    }
  }
  if (sensors_init() != ESP_OK)
  {
    ESP_LOGE(TAG, "Sensors initialization failed");
    while(true)
    {
      buzzer_beep(200);
      usleep(500 * 1000);
    }
  }
  while(true)
  {
    buzzer_beep(500);
    usleep(3000 * 1000);
  }
}

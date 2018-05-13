#ifndef __STORAGE_H__
#define __STORAGE_H_

#include "esp_err.h"
#include "structs.h"

esp_err_t storage_init(void);
esp_err_t sd_card_init(void);
void sd_card_unmount(void);
void storeMeasurement(Measurement& meas);

#endif //__STORAGE_H__
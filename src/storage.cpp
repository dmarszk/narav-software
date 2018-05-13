#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_err.h"

#include "config.h"
#include "storage.h"
#include "structs.h"
#include <string.h>

#undef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

static const char *TAG = "storage";

static FILE* samples_fd;
static uint8_t buffer[STORAGE_BUFFER_SIZE];
static uint16_t bufPos = 0;


uint8_t dataSizeLUT[16] =
{
  sizeof(DataStart),
  sizeof(DataAhrs),
  sizeof(DataAccel),
  sizeof(DataGyro),
  sizeof(DataMag),
  sizeof(DataBarom),
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

esp_err_t storage_init(void)
{
    return sd_card_init();
}

int file_exist(const char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

esp_err_t print_file_info(const char *filename)
{
  struct stat   buffer;   
  if (stat (filename, &buffer) == 0)
  {
    ESP_LOGI(TAG, "file %s, size = %u", filename, (uint32_t)buffer.st_size);
    return ESP_OK;
  }
  return ESP_FAIL;
  
}

void dataWrite(uint8_t* data, uint16_t size)
{
  static bool error = false;
  ESP_LOGV(TAG, "write: %d", size);
  if (samples_fd) {
    fwrite(data, size, 1, samples_fd);
    fsync(fileno(samples_fd));
  }
  else {
    if (!error)
    {
      ESP_LOGE(TAG, "SD write error");
      error = true;
    }
  }
}

void flushBuffer()
{
  dataWrite(buffer, bufPos);
  bufPos = 0;
}

void storeMeasurement(Measurement& meas)
{
  uint8_t measType = (meas.header >> MEAS_TYPE_SHIFT) & 0xF;
  uint8_t measSize = dataSizeLUT[measType] + 4; // header has 4 bytes
  ESP_LOGV(TAG, "Storing: %u, size: %u", measType, measSize);
  if (bufPos + measSize > STORAGE_BUFFER_SIZE)
    flushBuffer();
  memcpy((void*)(buffer + bufPos), (void*)&meas, measSize);
  bufPos += measSize;
}

esp_err_t prepare_samples_file(void)
{
  char fname[64];
  uint32_t num = 0;
  ESP_LOGI(TAG, "Preparing samples file...");
  while (samples_fd == NULL && num <= MAX_SAMPLE_FILE_INDEX)
  {
    sprintf(fname, "/sdcard/data_%d.dat", num++);
    if (print_file_info(fname) == ESP_OK)
    {
      // File exists
      continue;
    }
    samples_fd = fopen(fname, "w");
    ESP_LOGI(TAG, "Opened %s", fname);
    break;
  }
  if (num > MAX_SAMPLE_FILE_INDEX)
  {
    ESP_LOGE(TAG, "Cannot create file!");
    return ESP_FAIL;
  }
  return ESP_OK;
}

esp_err_t sd_card_init(void)
{
    // To use card in SDMMC mode, refer to SD Storage example in ESP IDF:
    // https://github.com/espressif/esp-idf/tree/59859fa53c03c598118529e61ce54ef87d25fe7c/examples/storage/sd_card
    ESP_LOGI(TAG, "Initializing SD card in SPI mode");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_SD_MISO;
    slot_config.gpio_mosi = PIN_SD_MOSI;
    slot_config.gpio_sck  = PIN_SD_CLK;
    slot_config.gpio_cs   = PIN_SD_CS;

    // Options for mounting the filesystem.
    // format_if_mount_failed is false, so in case of filesystem corruption,
    // the card does not get reformatted.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%d).", ret);
        }
        return ret;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    return prepare_samples_file();
}

void sd_card_unmount(void)
{
    // Unmount partition and disable SDMMC or SPI peripheral
    esp_vfs_fat_sdmmc_unmount();
}

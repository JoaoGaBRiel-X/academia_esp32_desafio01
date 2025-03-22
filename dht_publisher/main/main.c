#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include "mqtt_app.h"
#include "wifi.h"
#include "dht.h"
#include "ssd1306.h"

// function declaration
void display_init(SSD1306_t *dev, int sda, int scl, int reset);
void update_dht();

// OLED GPIO
#define OLED_SDA_GPIO 8
#define OLED_SCL_GPIO 9
#define OLED_RESET_GPIO -1

static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio = 15;

SSD1306_t oled;
char lineChar[20] = {0};
int16_t temperature = 0;
int16_t humidity = 0;

char temperature_str[10];
char humidity_str[10];

static const char *TAG = "Desafio_01";

void app_main(void)
{
    // initialize NVS
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA iniciado...");

    mqtt_app_start();
    ESP_LOGI(TAG, "MQTT iniciado...");

    // vTaskDelay(pdMS_TO_TICKS(2000));

    display_init(&oled, OLED_SDA_GPIO, OLED_SCL_GPIO, OLED_RESET_GPIO);

    // mqtt_app_subscribe("desafio01_jg/humidity", 0);
    // mqtt_app_subscribe("desafio01_jg/temperature", 0);
    // mqtt_app_subscribe("desafio01_jg/alert", 0);

    bool alert = false;
    bool inAlert = true;

    while (1)
    {
        update_dht();
        sprintf(temperature_str, "%d", temperature / 10);
        sprintf(humidity_str, "%d", humidity / 10);
        if (mqtt_is_connected())
        {
            mqtt_app_publish("desafio01_jg/temperature", temperature_str, 0, 0);
            mqtt_app_publish("desafio01_jg/humidity", humidity_str, 0, 0);
        }

        if ((temperature / 10) > 30) alert = true;
        else alert = false;


        if (alert && !inAlert)
        {
            inAlert = true;
            if (mqtt_is_connected())
            {
                mqtt_app_publish("desafio01_jg/alert", "alert", 0, 1);
            }
        }
        else if (!alert && inAlert)
        {
            inAlert = false;
            if (mqtt_is_connected())
            {
                mqtt_app_publish("desafio01_jg/alert", "no_alert", 0, 1);
            }
        }

        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}

void display_init(SSD1306_t *dev, int sda, int scl, int reset)
{
    ESP_LOGI(TAG, "Panel is 128x64");
    ESP_LOGI(TAG, "Interface is I2C");
    ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d", OLED_SDA_GPIO);
    ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d", OLED_SCL_GPIO);
    ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d", OLED_RESET_GPIO);

    i2c_master_init(&oled, OLED_SDA_GPIO, OLED_SCL_GPIO, OLED_RESET_GPIO);

    ssd1306_init(&oled, 128, 64);
    ssd1306_clear_screen(&oled, true);
    ssd1306_contrast(&oled, 0xff);
    ssd1306_display_text(&oled, 3, "   DESAFIO 01    ", 18, true);
    vTaskDelay(pdMS_TO_TICKS(3000));

    ssd1306_clear_screen(&oled, false);
    ssd1306_contrast(&oled, 0xff);
}

void update_dht()
{
    if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature) == ESP_OK)
    {
        ESP_LOGI(TAG, "Humidity: %d%%", humidity / 10);
        ESP_LOGI(TAG, "Temperature: %dC", temperature / 10);

        sprintf(lineChar, "Temp: %dC", temperature / 10);
        ssd1306_display_text(&oled, 3, lineChar, sizeof(lineChar), false);
        sprintf(lineChar, "Humi: %d%%", humidity / 10);
        ssd1306_display_text(&oled, 4, lineChar, sizeof(lineChar), false);
    }
    else
    {
        ESP_LOGE(TAG, "Could not read data from sensor");
    }
}
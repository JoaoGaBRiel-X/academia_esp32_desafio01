#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "mqtt_app.h"
#include "wifi.h"

#define LED_GPIO 2

static const char *TAG = "Desafio_01";

void app_main(void)
{
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

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    mqtt_app_subscribe("desafio01_jg/alert", 0);

    // initialize LED
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while(1)
    {
        if(mqtt_is_connected())
        {
            if (mqtt_app_get_alert_event())
            {
                ESP_LOGI(TAG, "Alerta recebido!");
                gpio_set_level(LED_GPIO, 1);            
            }
            else
            {
                gpio_set_level(LED_GPIO, 0);
            }
        }
        else
        {
            ESP_LOGI(TAG, "MQTT desconectado!");
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }


}

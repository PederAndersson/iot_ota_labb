#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "ota_update.h"
#include "wifi_connect.h"

#define LED_PIN_RED GPIO_NUM_23
#define LED_PIN_BLUE GPIO_NUM_22
#define LED_PIN_GREEN GPIO_NUM_21
#define ARR_SIZE sizeof(arr)/sizeof(arr[0])

void app_main(void)
{
    //Rör inte detta!!!!
    esp_err_t wifi_result = wifi_connect_start();
    if (wifi_result != ESP_OK) {
        ESP_LOGW("main", "Wi-Fi connection failed: %s", esp_err_to_name(wifi_result));
    }
    
    
    if (wifi_result == ESP_OK || wifi_result == ESP_FAIL) {
        esp_err_t ota_result = ota_update_start_polling();
        if (ota_result != ESP_OK) {
            ESP_LOGW("main", "Could not start OTA polling: %s", esp_err_to_name(ota_result));
        }
    }
    //Till hit. Viktigt glöm inte att sätta SSID och lösenord för skolans wifi
    //med idf.py menuconfig innan ni laddar upp uppdateringen!

    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN_GREEN));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN_GREEN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN_RED));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN_RED, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN_BLUE));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN_BLUE, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN_GREEN, 1));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN_RED, 0));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN_BLUE, 0));
}

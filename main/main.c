#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "ota_update.h"
#include "wifi_connect.h"

#define LED_PIN GPIO_NUM_23

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
    //Till hit.

    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 1));
}

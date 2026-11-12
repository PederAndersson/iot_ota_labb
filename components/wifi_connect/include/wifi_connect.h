#pragma once

#include <stdbool.h>

#include "esp_err.h"

// Starts Wi-Fi in station mode and waits until an IP address is assigned.
// Call once during startup. Returns ESP_FAIL after five failed attempts.
esp_err_t wifi_connect_start(void);

// True after the station has received an IP address.
bool wifi_connect_has_ip(void);

// Starts a new connection attempt after the initial retries were exhausted.
void wifi_connect_retry_if_needed(void);

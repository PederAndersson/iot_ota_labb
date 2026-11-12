#pragma once

#include "esp_err.h"

// Starts periodic HTTP update checks. An empty URL enables LAN discovery.
// Call after Wi-Fi has been initialized.
esp_err_t ota_update_start_polling(void);

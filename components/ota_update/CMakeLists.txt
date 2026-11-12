idf_component_register(
    SRCS "ota_update.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_https_ota esp_http_client esp_app_format esp_system wifi_connect lwip
)

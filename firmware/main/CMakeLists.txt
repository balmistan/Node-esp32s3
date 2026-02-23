idf_component_register(
    SRCS "arduino_wrapper.c"
         "main.c"
         "mqtt_utils.c"
         "timer_interrupt.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio
             esp_driver_ledc
             esp_driver_gptimer
    PRIV_REQUIRES nvs_flash esp_wifi esp_netif esp_event
)
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "mqtt_client.h"
#include "mqtt_utils.h"
#include "timer_interrupt.h"


static const char *TAG = "MQTT5_EXAMPLE";


// Parametri letti da menuconfig
#define WIFI_SSID      CONFIG_MY_WIFI_SSID
#define WIFI_PASS      CONFIG_MY_WIFI_PASS
#define MQTT_BROKER    CONFIG_MY_MQTT_BROKER_URI
#define MQTT_USER      CONFIG_MY_MQTT_USERNAME
#define MQTT_PASSWORD  CONFIG_MY_MQTT_PASSWORD
#define DEVICE_NAME  CONFIG_MY_DEVICE_NAME

// Event group per sincronizzazione Wi-Fi
static EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0

// Avanti dichiarazione
static void mqtt_app_start(void);

// Gestione eventi Wi-Fi
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Wi-Fi STA started, connecting...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Wi-Fi disconnected, retrying...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// Inizializzazione Wi-Fi
static void wifi_init(void)
{
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi init finished.");
}

// Gestione eventi MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED: {
        ESP_LOGI(TAG, "MQTT connected");

        char topic_sub[64];
        char topic_pub[64];

        snprintf(topic_sub, sizeof(topic_sub), "%s/in", DEVICE_NAME);
        snprintf(topic_pub, sizeof(topic_pub), "%s/out", DEVICE_NAME);

        esp_mqtt_client_subscribe(client, topic_sub, 1);
        mqtt_publish_message(client, topic_pub, "MQTT5 test message");
        break;
    }

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT disconnected");
            break;

        case MQTT_EVENT_DATA:
            handle_incoming_mqtt_data(event);
            break;

        default:
            break;
    }
}

// Avvio client MQTT
static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
    .broker.address.uri = MQTT_BROKER,
    .credentials.username = MQTT_USER,
    .credentials.authentication.password = MQTT_PASSWORD,
    .network.disable_auto_reconnect = false,
    .session.keepalive = 60,
    .buffer.size = 1024,
    .task.stack_size = 6144,
};

#if CONFIG_MY_MQTT_PROTOCOL_V5
    mqtt_cfg.session.protocol_ver = MQTT_PROTOCOL_V_5;
#else
    mqtt_cfg.session.protocol_ver = MQTT_PROTOCOL_3_1_1;
#endif
    ESP_LOGI(TAG, "MQTT config:");
    ESP_LOGI(TAG, "  Protocol version: %d", mqtt_cfg.session.protocol_ver);
    ESP_LOGI(TAG, "MQTT Broker URI: %s", mqtt_cfg.broker.address.uri);
    ESP_LOGI(TAG, "  Username: %s", mqtt_cfg.credentials.username ? mqtt_cfg.credentials.username : "(null)");
    ESP_LOGI(TAG, "  Password: %s", mqtt_cfg.credentials.authentication.password ? mqtt_cfg.credentials.authentication.password : "(null)");

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

// Main principale
void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();

    ESP_LOGI(TAG, "Waiting for IP...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    mqtt_app_start();
    init_timer_interrupt_1ms();

}

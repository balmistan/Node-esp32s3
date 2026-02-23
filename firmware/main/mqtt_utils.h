#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include "mqtt_client.h"


void commands(char receved_msg[]);

// Funzione da chiamare per gestire messaggi MQTT ricevuti
void handle_incoming_mqtt_data(esp_mqtt_event_handle_t event);

// Funzione per pubblicare messaggi su topic MQTT
void mqtt_publish_message(esp_mqtt_client_handle_t client, const char *topic, const char *message);

#endif // MQTT_UTILS_H

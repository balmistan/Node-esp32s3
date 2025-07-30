#include "arduino_wrapper.h"
#include "mqtt_utils.h"
#include "esp_log.h"
#include "esp_system.h"
#include <string.h>

static const char *TAG = "MQTT_UTILS";
#define RST 0 // reset software
#define PM 1 // pinMode
#define DR 2 // digitalRead
#define DW 3 // digitalWrite
#define AW 4 // analogWrite
#define AI 5 // attachInterrupt
#define DI 6 // detacheInzerrupt
#define DT 7 // digitalToggle
#define PING 8 // mqtt ping
#define ADI 9 // attachdebounceInterrupt

uint8_t offset = 48;



void handle_incoming_mqtt_data(esp_mqtt_event_handle_t event)
{
    char topic[event->topic_len + 1];
    char data[event->data_len + 1];

    memcpy(topic, event->topic, event->topic_len);
    topic[event->topic_len] = '\0';

    memcpy(data, event->data, event->data_len);
    data[event->data_len] = '\0';

    commands(data);

}

void mqtt_publish_message(esp_mqtt_client_handle_t client, const char *topic, const char *message)
{
    int msg_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    if (msg_id >= 0) {
        ESP_LOGI(TAG, "Messaggio pubblicato su topic '%s': %s", topic, message);
    } else {
        ESP_LOGE(TAG, "Errore pubblicazione messaggio su topic '%s'", topic);
    }
}


void commands(char *receved_msg)
{
   ESP_LOGI(TAG, "   receved message: %s", receved_msg);

  uint8_t i=0;
  uint8_t cmd=0;
  gpio_num_t pin=0;
  uint8_t value=0;
  uint16_t value2=0;
  uint8_t mode=0;
  

  if(strlen(receved_msg)>0){
    bool exit_on_error=false;
    do{
      cmd = (uint8_t)receved_msg[i] - offset;
      //ESP_LOGI(TAG, "   > command: %d ", cmd);
      switch(cmd){
        case RST:
         ESP_LOGI(TAG, "   > restart!");
          esp_restart();  // There is no need to do anything else, because the microcontroller will be restarted.
          break;  
        case PM:  //pinMode(pin, mode);
          pin = ((gpio_num_t)receved_msg[i+1]) - offset;                                                                                
          mode = ((uint8_t)receved_msg[i+2] - offset) >> 2;
          pinMode(pin, mode);
          i+=3;
          break;
        case DW:  //digitalWrite(pin, value);
          pin = ((gpio_num_t)receved_msg[i+1]) - offset;                                                                                
          value = ((uint8_t)receved_msg[i+2] - offset) & 0x01;
          digitalWrite(pin, value);
          i+=3;
          break;
          case AW:  //analogWrite(pin, value);
          pin = ((gpio_num_t)receved_msg[i+1]) - offset;                                                                                
          value = (receved_msg[i+2] - offset) & 0x03;
          value = value | (((uint8_t)receved_msg[i+3] - offset) << 2);

value2 =  (((uint16_t)receved_msg[i+4] - offset) << 8);
value2 = value2 | value;
          analogWrite(pin,value2);
          i+=5;
          break;
    /*    case AI:  //attachInterrupt(pin, mode);                                                                                                                    
          pin = ((uint8_t)receved_msg[i+1]) - offset;            
          mode = (((uint8_t)receved_msg[i+2]) - offset)>>2;
          interrupt_enable(pin, mode, 0);
          snprintf(debug_msg, sizeof(debug_msg), "> attachInterrupt(%d, %d)", pin, mode);
          i+=3;
          break;
        case DI: //detachInterrupt(pin, mode);                                                                                                                     
          pin = ((uint8_t)receved_msg[i+1]) - offset;             
          interrupt_disable(pin);
          snprintf(debug_msg, sizeof(debug_msg), "> detachInterrupt(%d)", pin);
          i+=2;
          break;
        case PING:
          mqtt_send("ping", "pong");
          i+=1;
          break;
        case ADI: //attachDebounceInterrupt(pin, mode);                                                                                                                      
          pin = ((uint8_t)receved_msg[i+1]) - offset;            
          mode = (((uint8_t)receved_msg[i+2]) - offset)>>2;
          interrupt_enable(pin, mode, 20);
          snprintf(debug_msg, sizeof(debug_msg), "> attachDebounceInterrupt(%d, %d)", pin, mode);
        i+=3;
        break;
        */
        default:
         ESP_LOGI(TAG, "   > command %d not found!", cmd);
         exit_on_error=true;
          break;
      }
     
    }while(i<strlen(receved_msg) && !exit_on_error);
  }
}

#include "arduino_wrapper.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "ARDUINO_WRAPPER";

// for analogWrite ///////////////
bool is_analog_initialized = false;
uint8_t used_channels = 0;
gpio_num_t configured_gpios[8];

/////////////////////////////////

void pinMode(gpio_num_t pin, uint8_t mode)
{
    ESP_LOGI(TAG, "   > pinMode(%d, %d)", pin, mode);
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .mode = GPIO_MODE_DISABLE};

    // Set direction and pull-up if needed
    if (mode == INPUT)
    {
        io_conf.mode = GPIO_MODE_INPUT;
    }
    else if (mode == OUTPUT)
    {
        io_conf.mode = GPIO_MODE_OUTPUT;
    }
    else if (mode == INPUT_PULLUP)
    {
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    }

    gpio_config(&io_conf);
}

void digitalWrite(gpio_num_t pin, uint8_t level)
{
    pinMode(pin, OUTPUT);
    gpio_set_level(pin, level);
    ESP_LOGI(TAG, "   > digitalWrite(%d, %d)", pin, level);
}

int digitalRead(gpio_num_t pin)
{
    return gpio_get_level(pin);
}

void analogWrite(gpio_num_t pin, uint16_t value)
{
uint8_t ch_index=0;

    ESP_LOGI(TAG, "   > analogWrite(%d, %d)", pin, value);

    if (!is_analog_initialized)
    {
        // PWM resolution: 13 bits -> values from 0 to 8191. Duty cicle step: ca 0,0122%

        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .duty_resolution = LEDC_TIMER_13_BIT,
            .freq_hz = 10000,
            .clk_cfg = LEDC_AUTO_CLK};
        ledc_timer_config(&ledc_timer);
        is_analog_initialized = true;
    }

    if (used_channels == 4)
    {
        ESP_LOGE(TAG, "No analog  outputs available. Maximum 8 already used!");
    }
    else
    {
        // I check if the GPIO is already configured as analog
        bool is_configured = false;
        
        for (ch_index = 0; ch_index < used_channels; ch_index++)
        {
            if (configured_gpios[ch_index] == pin)
            {
                is_configured = true;
                break;
            }
        } // close for
        if (!is_configured)
        {
            configured_gpios[ch_index] = pin;
            used_channels++;
        }
    }
    // Configure channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode =  LEDC_LOW_SPEED_MODE,
        .channel = ch_index,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = value, 
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);

    // Update duty cycle
    ledc_set_duty(mode, channel, value);
    ledc_update_duty(mode, channel);
}

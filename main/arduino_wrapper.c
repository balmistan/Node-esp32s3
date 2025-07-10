#include "arduino_wrapper.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "ARDUINO_WRAPPER";

void pinMode(gpio_num_t pin, int mode)
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
    level = (level == 0) ? 0 : 255;
    analogWrite(pin, level);
    ESP_LOGI(TAG, "   > digitalWrite(%d, %d)", pin, level);
}

int digitalRead(gpio_num_t pin)
{
    return gpio_get_level(pin);
}

void analogWrite(gpio_num_t pin, uint8_t value)
{

    ESP_LOGI(TAG, "   > analogWrite(%d, %d)", pin, value);

    // PWM resolution: 8 bits -> values from 0 to 255
    const int freq = 1000;
    const ledc_channel_t channel = LEDC_CHANNEL_0;
    const ledc_timer_t timer = LEDC_TIMER_0;
    const ledc_mode_t mode = LEDC_LOW_SPEED_MODE;
    const ledc_timer_bit_t resolution = LEDC_TIMER_12_BIT; //LEDC_TIMER_8_BIT;

    // Configure timer (only once per timer)
    ledc_timer_config_t ledc_timer = {
        .speed_mode = mode,
        .timer_num = timer,
        .duty_resolution = resolution,
        .freq_hz = freq,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    // Configure channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode = mode,
        .channel = channel,
        .timer_sel = timer,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = value, // Duty 0~255 for 8-bit resolution
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);

    // Update duty cycle
    ledc_set_duty(mode, channel, value);
    ledc_update_duty(mode, channel);
}

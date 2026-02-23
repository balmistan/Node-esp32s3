#include "timer_interrupt.h"
#include "driver/gptimer.h"
#include "esp_log.h"

static const char *TAG = "TIMER_ISR";

static bool IRAM_ATTR timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    // Timer ISR — chiamato ogni 1 ms
    // (per ora non fa nulla)
    return true;  // Ritorna true per "clear" automatico dell'interrupt
}

void init_timer_interrupt_1ms(void)
{
    gptimer_handle_t timer = NULL;
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1 µs per tick
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&config, &timer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_callback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer, &cbs, NULL));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000, // 1000 µs = 1 ms
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(timer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_enable(timer));
    ESP_ERROR_CHECK(gptimer_start(timer));

    ESP_LOGI(TAG, "1ms timer (GPTIMER) started");
}

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "polling led test";
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BUTTON_GPIO GPIO_NUM_4

#ifdef CONFIG_BLINK_LED_GPIO

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#else
#error "unsupported LED type"
#endif

static void configure_button(void){
    ESP_LOGI(TAG, "button setup starting...");
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    // button setup
    configure_button();
    bool last_level = 1;

    while (1) {
        int level = gpio_get_level(BUTTON_GPIO);
        if (level == 0 && level != last_level){
            ESP_LOGI(TAG, "button pressed! light is now on.");
            gpio_set_level(BLINK_GPIO, level);
            last_level = level;
        }else if(level != last_level && level == 1){
            ESP_LOGI(TAG, "button not pressed :( light is off.");
            gpio_set_level(BLINK_GPIO, level);
            last_level = level;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

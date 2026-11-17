#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "Train Light Signal";

#define GREEN_LED_PIN 25
#define YELLOW_LED_PIN 26
#define RED_LED_PIN 27

#define LED_ON  1
#define LED_OFF 0

static const gpio_num_t light_pins[] = {GREEN_LED_PIN, YELLOW_LED_PIN, RED_LED_PIN};
#define NUM_LIGHTS (sizeof(light_pins) / sizeof(light_pins[0]))
// static int current_light_color = GREEN_LED_PIN;

static void setup_lights(void){
    ESP_LOGI(TAG, "configuring lights...");
    for(int i = 0; i < NUM_LIGHTS; i++){
        gpio_reset_pin(light_pins[i]);
        gpio_set_direction(light_pins[i], GPIO_MODE_OUTPUT);
        
        // turn each on and off quickly to visually confirm they all work
        gpio_set_level(light_pins[i], LED_ON);
        vTaskDelay(pdMS_TO_TICKS(400));
        gpio_set_level(light_pins[i], LED_OFF);
    }

    // start with green on
    gpio_set_level(GREEN_LED_PIN, LED_ON);
}

// static void change_light_color(gpio_num_t pin){
     
// }

void app_main(void)
{
    setup_lights();
}

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
static int current_light_pin = GREEN_LED_PIN;

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

static void change_light_color(gpio_num_t pin){
     if(pin != current_light_pin){
        gpio_set_level(pin, LED_ON);
        gpio_set_level(current_light_pin, LED_OFF);
        current_light_pin = pin;
     }
}

static bool is_train_close_to_beginning(){
    // measure distance, check against defined min
    return true;
}

static bool has_train_passed_end(){
    // mark if train is approaching / passing (global var)
    // if train is no longer approaching (it must be passed then), return true and reset the global var
    return true;
    // on second thought, split this up into two functions. update the global var in the main loop
}

void app_main(void)
{
    setup_lights();

    while(1){
        if(is_train_close()){ // starting with just one signal for now, can update in the future once i have proof of concept
            change_light_color(YELLOW_LED_PIN);
        }else if(current_light_pin == YELLOW_LED_PIN && !has_train_passed_end()){
            change_light_color(RED_LED_PIN);
        }else if(current_light_pin == RED_LED_PIN && has_train_passed_end()){
            change_light_color(GREEN_LED_PIN);
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

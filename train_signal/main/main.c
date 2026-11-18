#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "Train Light Signal";

#define LED_ON  1
#define LED_OFF 0

typedef enum {
    GREEN,
    YELLOW,
    RED,
    NUM_LIGHTS_IN_SIGNAL // last element in enum tracks the number of lights
} signal_color;

typedef struct {
    gpio_num_t pins[NUM_LIGHTS_IN_SIGNAL];
    signal_color current_signal_color;
} signal_t;

static void setup_lights(signal_t *signal){
    ESP_LOGI(TAG, "configuring signal lights...");

    for(int i = 0; i < NUM_LIGHTS_IN_SIGNAL; i++){
        gpio_reset_pin(signal->pins[i]);
        gpio_set_direction(signal->pins[i], GPIO_MODE_OUTPUT);
        
        // turn each on and off quickly to visually confirm they all work
        gpio_set_level(signal->pins[i], LED_ON);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_set_level(signal->pins[i], LED_OFF);
    }

    // start with green on
    gpio_set_level(signal->pins[GREEN], LED_ON);
    signal->current_signal_color = GREEN;
}

static void change_light_color(signal_t *signal, const signal_color color){
     if(signal->current_signal_color != color){
        gpio_set_level(signal->pins[color], LED_ON);
        gpio_set_level(signal->pins[signal->current_signal_color], LED_OFF);
        signal->current_signal_color = color;
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
    static signal_t signal_1 = {
        .pins = { [GREEN] = 25, [YELLOW] = 26, [RED] = 27}};

    setup_lights(&signal_1);

    // while(1){
    //     if(is_train_close()){ // starting with just one signal for now, can update in the future once i have proof of concept
    //         change_light_color(YELLOW_LED_PIN);
    //     }else if(current_light_pin == YELLOW_LED_PIN && !has_train_passed_end()){
    //         change_light_color(RED_LED_PIN);
    //     }else if(current_light_pin == RED_LED_PIN && has_train_passed_end()){
    //         change_light_color(GREEN_LED_PIN);
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(20));
    // }
}

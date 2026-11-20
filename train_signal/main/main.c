#include <stdio.h>
#include "ultrasonic.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "Train Light Signal Project";

#define LED_ON  1
#define LED_OFF 0
#define MAX_NUM_SIGNALS 64
#define DISTANCE_TOLERANCE 2.0 // placeholder for now

typedef enum {
    GREEN,
    YELLOW,
    RED,
    NUM_LIGHTS_IN_SIGNAL // last element in enum tracks the number of lights
} signal_color;

typedef struct signal_t{
    ultrasonic_sensor_t sensor;
    gpio_num_t pins[NUM_LIGHTS_IN_SIGNAL];
    signal_color current_signal_color;
    int num_previous_signals;
    struct signal_t *previous_signals[MAX_NUM_SIGNALS];
} signal_t;

static signal_t all_signals[] = {
    { .pins = { [GREEN] = 25, [YELLOW] = 26, [RED] = 27 } },
    // { .pins = { [GREEN] = 10, [YELLOW] = 11, [RED] = 12 } },  // placeholder pins
};
#define NUM_SIGNALS (sizeof(all_signals) / sizeof(all_signals[0]))

static void setup_lights(signal_t *signal){
    ESP_LOGI(TAG, "configuring signal lights...");

    for(int i = 0; i < NUM_LIGHTS_IN_SIGNAL; i++){
        gpio_reset_pin(signal->pins[i]);
        gpio_set_direction(signal->pins[i], GPIO_MODE_OUTPUT);
        
        // turn each on and off quickly to visually confirm they all work
        gpio_set_level(signal->pins[i], LED_ON);
        vTaskDelay(pdMS_TO_TICKS(400));
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

// returns true if successful, false if too many previous signals
static bool add_previous_signal(signal_t *signal, signal_t *previous){
    if(signal->num_previous_signals >= MAX_NUM_SIGNALS){
        ESP_LOGE(TAG, "too many previous signals, returning false...");
        return false;
    } else {
        signal->previous_signals[signal->num_previous_signals++] = previous;
        return true;
    }
}

static float get_distance(signal_t signal){
 return 0.0;
}

void app_main(void)
{
    // simple loop with two signals
    // add_previous_signal(&all_signals[0], &all_signals[1]);
    // add_previous_signal(&all_signals[1], &all_signals[0]);
    ESP_LOGI(TAG, "signal map added");

    // init lights & ultrasonic sensors
    for(int i = 0; i < NUM_SIGNALS; i++){
        setup_lights(&all_signals[i]);
        // setup_sensor()
    }
    ESP_LOGI(TAG, "lights and sensors setup.");

    float distance = 0.0;

    while(1){
        for(int i = 0; i < NUM_SIGNALS; i++){
            distance = get_distance(all_signals[i]);
            if(distance < DISTANCE_TOLERANCE){
                // do something
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

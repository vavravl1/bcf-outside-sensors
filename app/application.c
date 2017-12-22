#include <application.h>
#include <radio.h>
#include "sensors.h"

bc_led_t led;

void application_init(void)
{
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);

    bc_module_sensor_init();
    bc_module_sensor_set_mode(BC_MODULE_SENSOR_CHANNEL_A, BC_MODULE_SENSOR_MODE_INPUT);
    bc_module_sensor_set_pull(BC_MODULE_SENSOR_CHANNEL_A, BC_MODULE_SENSOR_PULL_DOWN_INTERNAL);
    
    bc_radio_pairing_request(FIRMWARE, "1.0.0");
    bc_led_set_mode(&led, BC_LED_MODE_OFF);

    sensors_init_all();    
}

static bool switch_on_sensor_module_state = false;
void application_task(void) {
    bc_scheduler_plan_current_relative(500);
    static uint16_t pressed_count = 0;

    bool switch_state_actual = bc_module_sensor_get_input(BC_MODULE_SENSOR_CHANNEL_A);
    if(switch_on_sensor_module_state != switch_state_actual) {
	switch_on_sensor_module_state = switch_state_actual;
	bc_led_pulse(&led, 500);
        bc_led_set_mode(&led, BC_LED_MODE_OFF);	
        bc_radio_pub_push_button(&pressed_count);
	pressed_count++;
    } 
}


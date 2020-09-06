/**
 * @file main.c
 * @brief Example program to measure a voltage on an ESP32 Analog to Digital
 * channel and write it to a SSD106 OLED display over an I2c serial connection.
 *
 * @author Jim Sutton
 * @date August 24 2020
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "esp_err.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "oled_1306.h" // driver and functions for OLED Display
#include "a_to_d_utils.h" // driver and functions for Analog to Digital Converter
#include "d_to_a_utils.h" // driver and functions for Analog to Digital Converter

#define SDA_PIN GPIO_NUM_21 // serial data line
#define SCL_PIN GPIO_NUM_22 // serial clock line
#define DAC_EXAMPLE_CHANNEL     CONFIG_EXAMPLE_DAC_CHANNEL
#define ADC1_EXAMPLE_CHANNEL    CONFIG_EXAMPLE_ADC1_CHANNEL

#define tag "main        "

void app_main(void) {
	ESP_LOGI(tag, "--------------------------------");
	ESP_LOGI(tag, "Starting Main Program Execution\n");

	uint8_t dac_set_point=0;

	char span[4] = "";
	char oled_message [32] ="";

	init_ADC(ADC1_EXAMPLE_CHANNEL);
	init_DAC(DAC_EXAMPLE_CHANNEL);
	vTaskDelay(2 * portTICK_PERIOD_MS); //2 port ticks == 1/50 sec

	i2c_master_init(SDA_PIN, SCL_PIN);
	ssd1306_init();
	oled_display_clear();

	char str[] = "\n\n\n\n\n\n\n\nX";
	oled_display_text(str);

	while(1) {

			set_dac_output_voltage( DAC_EXAMPLE_CHANNEL, dac_set_point );
			//get_span_pct_string(ADC1_EXAMPLE_CHANNEL,span);
			get_voltage_string(ADC1_EXAMPLE_CHANNEL,span);
			dac_set_point++;

			strcpy(oled_message, "  Stone Tablet  \n\nVoltage ");
			strcat(oled_message,span);
			strcat(oled_message," VDC");
			oled_display_text(oled_message);

			vTaskDelay( 2 * portTICK_PERIOD_MS );
	}
}

/*
 * Copyright (c) 2024
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_lorawan.h"
#include "app_stm32_rtc.h"
#include <zephyr/logging/log.h>

//  ======== globals ============================================
static const struct gpio_dt_spec led_tx = GPIO_DT_SPEC_GET(LED_TX, gpios);
static const struct gpio_dt_spec led_rx = GPIO_DT_SPEC_GET(LED_RX, gpios);
static const struct device *rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

struct values {
	int16_t vbat;
	int16_t temp;
	int16_t hum;
	int16_t vadc;
//	uint8_t *id;
//	int32_t *timestamp;
} data[MAX_SAMPLES];

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *dev;
	struct values data;
	int8_t payload[MAX_SAMPLES];

    int8_t ret;
//	uint8_t dev_eui[] = LORAWAN_DEV_EUI;

	// configuration of LEDs
	gpio_pin_configure_dt(&led_tx, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&led_rx, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&led_tx, 0);
	//gpio_pin_set_dt(&led_rx, 0);

	// initialization of LoRaWAN - TTN
	app_lorawan_init(dev);
	app_rtc_init(dev);

	printk("Geophone Measurement Simulation and Process Information - Board: %s\n", CONFIG_BOARD);
	
	// beginning forever loop (polling mode)
	while (1) {
		// constrution of random data structure
		data.vbat = sys_rand16_get() % (3300 - 0 + 1) + 0;
		data.temp = -15 + sys_rand16_get() % (50 - 25 +1);
		data.hum = sys_rand16_get() % (100 - 0 + 1) + 0;
		data.vadc = sys_rand16_get() % (3300 - 0 + 1) + 0;

		//payload->id = dev_eui;
		//payload->timestamp = app_rtc_get_time (dev);

		// transmission of packets on lorawan protocole - encode payload to bytes
		// battery needs 2 bytes

		payload[0] = (data.vbat >> 8) & 0xFF;
        payload[1] = data.vbat & 0xFF;

		// temperature needs 3 byte 327.67 max value (signed short)
		if (data.temp < 0) {
		 	payload[2] = 1 & 0xff;
		} else {
			payload[2] = 0 & 0xff;
		}
		payload[3] = (data.temp >> 8) & 0xFF;
        payload[4] = data.temp & 0xFF;

		// humidity needs 2 bytes
		payload[5] = (data.hum >> 8) & 0xFF;
        payload[6] = data.hum & 0xFF;

		// adc needs 2 bytes
		payload[7] = (data.vadc >> 8) & 0xFF;
        payload[8] = data.vadc & 0xFF;

		printk("sending random data...\n");

		gpio_pin_set_dt(&led_tx, 1);
		ret = lorawan_send(LORAWAN_PORT, payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);

		gpio_pin_set_dt(&led_tx, 0);
		k_sleep(DELAY);
	}
	return 0;
}
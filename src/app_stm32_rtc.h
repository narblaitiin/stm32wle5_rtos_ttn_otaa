/*
 * Copyright (c) 2024
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_STM32_RTC_H
#define APP_STM32_RTC_H

//  ======== includes ==============================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/counter.h>

//  ======== defines ===============================================

//  ======== prototypes ============================================
int8_t app_rtc_init(const struct device *dev);
int32_t* app_rtc_get_time(const struct device *dev);
 
#endif /* APP_STM32_RTC_H */
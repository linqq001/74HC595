#ifndef LINQQ001_74HC595_H
#define LINQQ001_74HC595_H
#include "driver/gpio.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define SCK_GPIO_PIN 37  // 移位寄存器时钟线引脚 SHCP
#define RCK_GPIO_PIN 36  // 存储寄存器时钟线引脚 STCP
#define SDA_GPIO_PIN 35  // 数据引脚            DS
#define _74HC595_LEVEL 1 // 74HC595级联数

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     *  初始化 84HC595
     */
    void HC595_INIT();

    /**
     *  关闭全部LED
     */
    void close_led_all();

    /**
     *  打开全部LED
     */
    void open_led_all();
    /**
     * 打开几号LED
     */
    void open_led(uint8_t byte);
    /**
     * 关闭几号LED
     */
    void close_led(uint8_t byte);

#ifdef __cplusplus
}
#endif
#endif
#include <stdio.h>
#include "74HC595.h"

int light_def = 0b00000000;
#define HC595_SCK_Low() gpio_set_level(SCK_GPIO_PIN, 0)   // SCK置低
#define HC595_SCK_High() gpio_set_level(SCK_GPIO_PIN, 1)  // SCK置高
#define HC595_RCK_Low() gpio_set_level(RCK_GPIO_PIN, 0)   // RCK置低
#define HC595_RCK_High() gpio_set_level(RCK_GPIO_PIN, 1)  // RCK置高
#define HC595_Data_Low() gpio_set_level(SDA_GPIO_PIN, 0)  // 输入低电平
#define HC595_Data_High() gpio_set_level(SDA_GPIO_PIN, 1) // 输入高电平

static const char *TAG = "74HC595";
/*
 @brief:74HC595输出锁存 使能
*/
void HC595_Save(void)
{
    /**  步骤3：RCK产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
    HC595_RCK_Low(); // 将RCK拉低
    vTaskDelay(10);
    HC595_RCK_High(); // 再将RCK拉高，RCK即可产生一个上升沿
}

/***
 *74HC595 发送一个字节
 *即往74HC595的SDA引脚发送一个字节
 */
void HC595_Send_Byte(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++) // 一个字节8位，传输8次，一次一位，循环8次，刚好移完8位
    {
        /****  步骤1：将数据传到DS引脚    ****/
        HC595_SCK_Low(); // SCK拉低
        if (byte & 0x80)
        {                      // 先传输高位，通过与运算判断第八是否为1
            HC595_Data_High(); // 如果第八位是1，则与 595 DS连接的引脚输出高电平
        }
        else
        { // 否则输出低电平
            HC595_Data_Low();
        }

        /*** 步骤2：SCK每产生一个上升沿，当前的bit就被送入移位寄存器 ***/
        byte <<= 1;       // 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否为1
        HC595_SCK_High(); // SHCP拉高， SHCP产生上升沿
    }
}

/**
 *发送多个字节
 *便于级联时数据的发送
 *级联N级，就需要发送N个字节控制HC595
 ***/
void HC595_Send_Multi_Byte(uint8_t *data, uint16_t len)
{
    uint8_t i;
    // len 个字节
    for (i = 0; i < len; i++)
    {
        HC595_Send_Byte(data[i]);
        // ESP_LOGI(TAG, "leve  :%d  data:%x", i, data[i]);
    }
    HC595_Save();
}

/**
 * 打开几号LED
 */
void open_led(uint8_t byte)
{
    light_def |= (1 << byte);
    HC595_Send_Byte(light_def);
    HC595_Save();
}

/**
 * 关闭几号LED
 */
void close_led(uint8_t byte)
{
    light_def &= ~(1 << byte);
    HC595_Send_Byte(light_def);
    HC595_Save();
}

/**
 *  关闭全部LED
 */
void close_led_all()
{
    for (int i = 0; i < _74HC595_LEVEL * 8; i++)
    {
        close_led(i);
    }
}

/**
 *  打开全部LED
 */
void open_led_all()
{
    for (int i = 0; i < _74HC595_LEVEL * 8; i++)
    {
        open_led(i);
    }
}

void HC595_INIT()
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << SCK_GPIO_PIN) | (1ULL << RCK_GPIO_PIN) | (1ULL << SDA_GPIO_PIN),
        .pull_down_en = 1,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);
    HC595_SCK_Low();
    HC595_RCK_Low();
    HC595_Data_Low();
}

// void test(void)
// {
//     gpio_config_t io_conf = {
//         .intr_type = GPIO_INTR_DISABLE,
//         .mode = GPIO_MODE_OUTPUT,
//         .pin_bit_mask = (1ULL << SCK_GPIO_PIN) | (1ULL << RCK_GPIO_PIN) | (1ULL << SDA_GPIO_PIN),
//         .pull_down_en = 1,
//         .pull_up_en = 0,
//     };
//     gpio_config(&io_conf);
//     HC595_SCK_Low();
//     HC595_RCK_Low();
//     HC595_Data_Low();

//     uint8_t place = 0; // 级联位置
//     uint8_t floor = 0; // 级联层级
//     uint8_t ele[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//     while (1)
//     {
//         uint8_t led_Pos_Buf[_74HC595_LEVEL];
//         int max_num = _74HC595_LEVEL * 8;
//         for (int i = 1; i < max_num + 1; i++)
//         {
//             vTaskDelay(300); // 延时500毫秒
//             memset(led_Pos_Buf, 0, _74HC595_LEVEL);
//             floor = (i - 1) / 8;
//             if (0 != i % 8)
//             {
//                 place = i % 8 - 1;
//             }
//             else
//             {
//                 place = 7;
//             }

//             led_Pos_Buf[_74HC595_LEVEL - floor - 1] = ele[7 - place];
//             // ESP_LOGI(TAG,"floor :%d  place:%d ",floor+1,place);
//             HC595_Send_Multi_Byte(led_Pos_Buf, _74HC595_LEVEL); // 将当前数据发送到595
//         }
//     }
// }
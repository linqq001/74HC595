# 74HC595 移位寄存器
## 概述
- 74HC595 用于ESP32使用3个引脚直接控制多个LED。74HC595 是一款价格便宜的八位移位锁存器，通过三个引脚（DS、STCP、SHCP），就可以控制几乎无限量地输出。DS是串行输入端口，每当时钟输入（SHCP）上升沿来临时，DS引脚当前电平值在移位寄存器中会移一位，连续进行8次同样的动作，就可以完成全部（Q0至Q7）移位。最后当STCP（Latch）上升沿到来时，移位寄存器的值将会被锁定在存储器里，并从Q0至Q7引脚输出。ESP32 的数字输出引脚时有限的,如果需要更多的输出的话，期中的一种方法就是通过74HC 595扩展更多的输出引脚。
- 特性
    8 位串行输入，并行输出
    2 V 至 6 V 的宽工作电压范围
    大电流 3 态输出可驱动多达 15 个 LSTTL 负载
    低功耗，最大 80 μA ICC
    典型 tPD = 14 ns
    5 V 时 ±6 mA 输出驱动器
    最大 1 μA 的低输入电流
    直接复位的移位寄存器
    Q0-Q7：8 位并行数据输出引脚，适合直接控制 8 个 LED 或 8 个 7 段显示器的引脚。
- 引脚
    Q7'：串行输出引脚连接到另一个 74HC595 的 DS，以串联多个 74HC595
    MR：复位引脚，低电平有效;
    SHcp： 移位寄存器的时间序列输入。随着 rising edge， shift register 中的数据连续移动一位，即 Q1 中的数据进入 Q2 ，依此类推。对于 falling edge，shift register 中的数据保持不变。
    STcp： 内存寄存器的时序输入。随着 rising edge 的到来，数据从 shift register 移动到 memory register。
    OE：输出激活引脚，低电平有效。
    DS：串行数据输入引脚
    VCC：正电源电压。
    GND：大地。
## 接线图
![repo-settings-image](https://gitee.com/linqq001/esp32s3-components-all/raw/main/components/74HC595/1.jpg)
![repo-settings-image](https://gitee.com/linqq001/esp32s3-components-all/raw/main/components/74HC595/2.jpg)    
## 使用方法
``` c
#include "74HC595.h"


void app_main(void)
{
    HC595_INIT();     //初始化
    open_led_all();   //打开全部LED
    vTaskDelay(pdMS_TO_TICKS(1000));
    close_led_all();  //关闭全部LED
    while(1)
    {
        for(int i=0;i<8;i++)
        {//依次打开LED
            open_led(i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        for(int i=7;i>=0;i--)
        {//倒序关闭LED
            close_led(i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

```

## 许可证

本项目采用 MIT 许可证。更多信息请参阅 [LICENSE](LICENSE) 文件。

## 贡献

## 作者
LINQQ001
## 历史版本
- 1.0.0 初始版本



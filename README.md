# BearPi-HM_Nano案例开发


## 案例分析

BearPi-HM_Nano 提供的例程多达 30个，每个例程都有非常详细的注释，代码风格统一，按照基本例程到高级例程的方式编排，方便初学者由浅入深逐步学习。开发者拿到工程后经过简单的编译和下载即可看到实验现象。

这些例程包括四个类别：内核类、基本外设类、驱动类、物联网类。不仅包括了硬件资源的应用，更是提供了丰富的物联网领域的应用示例，帮助物联网开发者更好更快地进行开发。

例程列表如下所示：

| 编号 | 类别   | 例程名           | 说明                                                         |
| ---- | ------ | ---------------- | ------------------------------------------------------------ |
| A1   | 内核   | thread           |   [任务交替打印](/applications/BearPi/BearPi-HM_Nano/sample/A1_kernal_thread/README.md)  |
| A2   | 内核   | timer            |  [定时器  ](/applications/BearPi/BearPi-HM_Nano/sample/A2_kernel_timer/README.md)                                                      |
| A3   | 内核   | event            |   [事件](/applications/BearPi/BearPi-HM_Nano/sample/A3_kernel_event/README.md)|
| A4   | 内核   | mutex            |   [互斥锁](/applications/BearPi/BearPi-HM_Nano/sample/A4_kernel_mutex/README.md)|
| A5   | 内核   | semp             |   [信号量](/applications/BearPi/BearPi-HM_Nano/sample/A5_kernel_semaphore/README.md)|
| A6   | 内核   | message           |    [消息队列](/applications/BearPi/BearPi-HM_Nano/sample/A6_kernel_message/README.md)|
| B1   | 基础   | led_blink        |   [红色 LED 不间断烁](/applications/BearPi/BearPi-HM_Nano/sample/B1_basic_led_blink/README.md)|
| B2   | 基础   | button           |   [按键控制LED灯亮灭](/applications/BearPi/BearPi-HM_Nano/sample/B2_basic_button/README.md)|
| B3   | 基础   | pwm_led          |   [红色 LED 呼吸灯](/applications/BearPi/BearPi-HM_Nano/sample/B3_basic_pwm_led/README.md)|
| B4   | 基础   | adc_mq2          |   [ADC读取电压](/applications/BearPi/BearPi-HM_Nano/sample/B4_basic_adc/README.md)|
| B5   | 基础   | i2c_bh1750       |   [I2C读取NFC标签](/applications/BearPi/BearPi-HM_Nano/sample/B5_basic_i2c_nfc/README.md)|
| B6   | 基础   | basic_uart       |   [UART自发自收](/applications/BearPi/BearPi-HM_Nano/sample/B6_basic_uart/README.md)|
| C1   | 实操   | aiot_mqtt       |   [MQTT连接阿里云平台](/applications/BearPi/BearPi-HM_Nano/sample/my_app/README.md)|

## ssd1306移植
基于stm32仓库进行移植和改良。
1. [https://github.com/afiskon/stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306)

## mqtt通信
实现方法：参考阿里云官方对接方法，将其连接到阿里云平台上。

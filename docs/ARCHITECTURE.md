# 软件架构

## 1. 架构概览

系统采用 **前后台架构**：主循环轮询各任务（前台），中断服务程序处理异步事件（后台）。

```
┌──────────────────────────────────────────────────────────┐
│                      主循环 (while(1))                    │
│                                                          │
│  ┌───────────┐  ┌─────────────┐  ┌───────────────────┐ │
│  │ Blue_count │  │Control_Task │  │ Steer_Control     │ │
│  │   30ms     │  │    5ms      │  │    50ms           │ │
│  │ 蓝牙解析   │  │ PID 核心    │  │ 舵机 + WiFi 解析  │ │
│  └───────────┘  └─────────────┘  └───────────────────┘ │
│                                                          │
│  ┌───────────┐  ┌─────────────┐                        │
│  │ Sr04_Mode │  │  LED_Task   │                        │
│  │   50ms    │  │   ~500ms    │                        │
│  │ 超声波    │  │  心跳指示   │                        │
│  └───────────┘  └─────────────┘                        │
│                                                          │
└──────────────────────────────────────────────────────────┘

         ┌─────────────── 中断服务程序 ───────────────┐
         │                                            │
         │  SysTick_Handler     → 1ms 系统滴答计时    │
         │  USART2_IRQHandler   → 蓝牙数据逐字节接收  │
         │  USART3_IRQHandler   → WiFi 数据逐字节接收 │
         │  TIM2_IRQHandler     → 编码器 A 溢出计数   │
         │  TIM4_IRQHandler     → 编码器 B 溢出计数   │
         │  TIM5_IRQHandler     → 超声波触发+回波计数 │
         │  EXTI9_5_IRQHandler  → 超声波 Echo 边沿    │
         └────────────────────────────────────────────┘
```

## 2. 任务调度

### 2.1 SysTick 计时

SysTick 每 1ms 中断一次，递增所有软件定时器：

```c
// delay.c
void SysTick_Handler(void)
{
    System_Time++;
    LED_Time[0]++;
    ENC_Time[0]++;
    BLUE_TIME++;
    Control_TIME++;
    Steer_TIME++;
    Sr04_TIME++;
}
```

### 2.2 主循环轮询

```c
// main.c
while(1)
{
    LED_Task();
    if(BLUE_TIME > 30)     { BLUE_TIME = 0; Blue_count(); }
    if(Control_TIME > 5)   { Control_TIME = 0; Control_Task(); }
    if(Steer_TIME > 50)    { Steer_TIME = 0; Steer_Control();
                              if(Cut_mode_flag==1) WIFI_Analysis(); }
    if(Sr04_TIME > 50)     { Sr04_TIME = 0;
                              if(Cut_mode_flag==2) Sr04_Mode(); }
}
```

### 2.3 任务周期

| 任务 | 周期 | 说明 |
|------|------|------|
| Control_Task | 5ms | PID 控制核心，读取编码器和 IMU，计算 PWM |
| Blue_count | 30ms | 解析蓝牙指令，切换模式 |
| Steer_Control | 50ms | 更新舵机角度，解析 WiFi 数据 |
| Sr04_Mode | 50ms | 超声波避障决策（仅避障模式） |
| LED_Task | ~500ms | LED2 心跳闪烁 |

## 3. NVIC 优先级配置

```c
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
// 2 位抢占优先级 + 2 位子优先级
```

| 中断 | 抢占优先级 | 子优先级 | 说明 |
|------|-----------|---------|------|
| USART2 (蓝牙) | 0 | 1 | 高优先级，保证接收不丢字节 |
| USART3 (WiFi) | 0 | 0 | 与蓝牙同级，子优先级更高 |
| TIM2 (编码器A) | - | - | 溢出计数 |
| TIM4 (编码器B) | - | - | 溢出计数 |
| TIM5 (超声波) | 5 | 0 | 低优先级 |
| EXTI9_5 (Echo) | 15 | 15 | 最低优先级 |

## 4. 模块说明

### 4.1 control.c — PID 控制核心

| 函数 | 说明 |
|------|------|
| `Control_Task()` | 主控制任务，5ms 周期调用 |
| `Vertical()` | 直立环 PD 计算 |
| `Velocity()` | 速度环 PI 计算（含低通滤波 + 积分限幅） |
| `Turn()` | 转向环 PD 计算 |

### 4.2 motor.c — 电机驱动

| 函数 | 说明 |
|------|------|
| `Motor_Init()` | 初始化 TIM3 PWM (1kHz, ±7200) |
| `Motor_PWMLoad(a, b)` | 设置双电机 PWM，自动选择正/反转通道 |
| `Limit()` | PWM 限幅 ±7200 |

电机驱动通过 GPIO 寄存器直接操作引脚模式（AF_PP ↔ Out_PP），实现 PWM 通道切换和电机停转。

### 4.3 encoder.c — 编码器测速

| 函数 | 说明 |
|------|------|
| `Encoder_Init()` | 初始化 TIM2 + TIM4 编码器接口模式 |
| `Read_Speed(TIMx)` | 读取计数值并清零（用于 PID） |
| `Read_MotorASpeed()` | 带溢出补偿的读取（用于调试） |
| `Encoder_ReadSpeedTest()` | 完整测速测试（调试用） |

### 4.4 ble.c — 蓝牙通信

| 函数 | 说明 |
|------|------|
| `Ble_Init()` | 初始化 USART2 (9600bps) |
| `Blue_count()` | 解析蓝牙指令（30ms 周期） |
| `USART2_IRQHandler()` | 蓝牙接收中断 |
| `Blue_SetName()` | AT 指令设置蓝牙名称 |

### 4.5 wifi.c — WiFi 通信

| 函数 | 说明 |
|------|------|
| `Usart3_Config()` | 初始化 USART3 (9600bps) |
| `WIFI_Analysis()` | 解析 WiFi 协议帧 |
| `USART3_IRQHandler()` | WiFi 接收中断 |

### 4.6 sr04.c — 超声波避障

| 函数 | 说明 |
|------|------|
| `Sr04_Init()` | 初始化 GPIO + EXTI + TIM5 |
| `Sr04_SendTTL()` | TIM5 中断中触发 Trig 脉冲 |
| `Sr04_GetLength()` | 计算距离 |
| `Sr04_Mode()` | 避障决策（50ms 周期） |

### 4.7 steer.c — 舵机云台

| 函数 | 说明 |
|------|------|
| `Steer_Init()` | 初始化 TIM1 PWM (50Hz) |
| `Steer_SetAngle()` | 设置舵机角度（含限幅和映射） |
| `Steer_Control()` | 遥控调节舵机角度（50ms 周期） |

### 4.8 oled.c — OLED 显示

| 函数 | 说明 |
|------|------|
| `OLED_Init()` | 初始化 SSD1306 |
| `OLED_Clear()` | 清屏 |
| `OLED_ShowString()` | 显示 ASCII 字符串 |
| `OLED_ShowChinese()` | 显示汉字（查字库） |
| `OLED_Refresh()` | 将 GRAM 刷写到屏幕 |
| `OLED_ShowPicture()` | 显示图片 |

### 4.9 delay.c — 系统滴答

| 函数/变量 | 说明 |
|----------|------|
| `SysTick_Init()` | 初始化 SysTick (1ms) |
| `Delay_ms()` | 毫秒延时（基于 System_Time） |
| `Delay_us()` | 微秒延时（NOP 循环） |
| `System_Time` | 全局系统时间计数器 |

## 5. 源码文件清单

```
firmware/user/
├── main.c                      # 主程序 + LED_Task + ENC_Task
├── main.h                      # JTAG/SWD 配置
├── stm32f10x_it.c              # 异常处理（HardFault 等）
├── api/
│   ├── control.c/h             # PID 控制
│   ├── motor.c/h               # 电机 PWM
│   ├── encoder.c/h             # 编码器测速
│   ├── ble.c/h                 # 蓝牙通信
│   ├── wifi.c/h                # WiFi 通信
│   ├── sr04.c/h                # 超声波避障
│   ├── steer.c/h               # 舵机云台
│   ├── oled.c/h                # OLED 显示驱动
│   ├── oledfont.h              # OLED 字库（ASCII + 汉字）
│   ├── picture.c               # 开机图片数据
│   ├── LED.c/h                 # LED 驱动
│   ├── delay.c/h               # 延时与 SysTick
│   ├── usart1.c/h              # 串口调试（printf 重定向）
│   ├── IO_BIT.h                # 位操作宏
│   └── MPU6050/
│       ├── mpu6050.c/h         # MPU6050 寄存器操作
│       ├── mpuiic.c/h          # 软件 I2C
│       ├── inv_mpu.c/h         # InvenSense MPU 驱动
│       ├── inv_mpu_dmp_motion_driver.c/h  # DMP 固件加载
│       └── eMPL/               # eMPL 平台抽象层
├── startup/                    # 启动文件
├── STM32F10x_StdPeriph_Driver/ # 标准外设库
└── project/                    # Keil 工程文件
```

## 6. 开发环境

| 工具 | 说明 |
|------|------|
| Keil MDK-ARM | 5.x |
| 标准外设库 | 3.5.0 |
| 调试器 | ST-Link V2 / J-Link |
| 芯片 | STM32F103RC |
| 晶振 | 8MHz HSE → 72MHz 系统时钟 |

### 编译建议

- 优化等级: -O1 或以上
- MicroLIB: 启用（减小 printf 体积）
- C99 模式: 启用

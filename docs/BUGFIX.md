# 问题排查与修复记录

本文件记录项目联调与代码审查过程中定位并修复的典型软件 Bug。

## 1. 舵机初始化负角度隐式转换回绕（`steer.c`）

| 项目 | 内容 |
|------|------|
| 文件 | `user/api/steer.c` `Steer_Init()` |
| 现象 | 上电初始化时 `Steer_SetPWM(2, steer2Angle)` 直接传入 `steer2Angle = -90`，形参类型为 `uint16_t`，隐式转换后回绕为 `65446`，导致俯仰舵机上电打到异常极限位置 |
| 修复 | 初始化改为调用 `Steer_SetAngle()`，将角度线性映射为 500~2500μs 脉宽后再写入捕获比较寄存器 |

```c
// 修复前
Steer_SetPWM(1, steer1Angle);    // 0 -> OK
Steer_SetPWM(2, steer2Angle);    // -90 -> 65446 -> 脉宽异常

// 修复后
Steer_SetAngle(1, steer1Angle);  // 0° -> 1500μs
Steer_SetAngle(2, steer2Angle);  // -90° -> 1500μs
```

## 2. 蓝牙串口接收缓冲区边界保护（`ble.c`）

| 项目 | 内容 |
|------|------|
| 文件 | `user/api/ble.c` `USART2_IRQHandler()` |
| 现象 | `Blue_buf[2]` 长度为 2 字节，中断接收时 `Blue_leng++` 未做上限判断，串口连续收到多字节干扰数据时存在数组越界写入风险 |
| 修复 | 写入后增加边界检查 `if(Blue_leng >= sizeof(Blue_buf)) Blue_leng = 0;` |

## 3. WiFi 串口接收缓冲区越界与帧长校验（`wifi.c` / `wifi.h`）

| 项目 | 内容 |
|------|------|
| 文件 | `user/api/wifi.c` + `user/api/wifi.h` |
| 现象 | ① `recvData[256]` 接收无边界检查，且 `recvCount` 为 `uint8_t` 在满 255 字节后会溢出回绕覆盖；② `WIFI_Analysis()` 仅判断 `recvCount < 5`，未覆盖第 6 字节校验和 `recvData[5]` |
| 修复 | 将 `recvCount` 扩宽为 `uint16_t`，中断内增加 `if(wifi.recvCount < sizeof(wifi.recvData))` 保护，解析时校验完整 6 字节帧长 `if(wifi.recvCount < 6)` |

## 4. 5ms 控制环路冗余 I²C 读取与编码器溢出计数同步（`control.c` / `encoder.c`）

| 项目 | 内容 |
|------|------|
| 文件 | `user/api/control.c`、`user/api/encoder.c` |
| 现象 | ① `Control_Task()` 每 5ms 调用一次 `MPU_Get_Accelerometer()`，但三环 PID 仅使用 DMP 欧拉角与陀螺仪角速度，冗余读取白白占用软件 I²C 总线时间；② `Read_Speed()` 清零定时器 `CNT` 时未同步清零 `EncoderA/B_Overflow_Count` |
| 修复 | 移除未使用的加速度读取调用，并在 `Read_Speed()` 清零计数器时同步清零溢出计数 |

## 5. OLED 汉字字库查表索引初始化与孤立字符清理（`oled.c`）

| 项目 | 内容 |
|------|------|
| 文件 | `user/api/oled.c` `OLED_ShowChinese()` |
| 现象 | 函数定义前残留孤立 `/` 字符，且局部查表索引变量 `num` 未初始化，当字库未命中目标汉字时将使用随机栈值越界访问字模数组 |
| 修复 | 删除孤立 `/` 字符，并将索引变量声明初始化为 `u8 m, temp, num = 0;` |

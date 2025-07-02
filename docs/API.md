# API 参考文档

本文档描述了SC95F8523演示代码中主要的API接口。

## 头文件

### Function_Init.H
主要的功能初始化头文件，包含所有模块的函数声明。

### SC95F852x_C.H  
芯片寄存器定义头文件，包含所有寄存器和位定义。

### Multiplication_Division.H
乘除法运算模块的头文件。

## 初始化函数

### IO_Init()
```c
void IO_Init(void);
```
**功能**: 初始化GPIO引脚配置
**参数**: 无
**返回**: 无
**说明**: 配置各个引脚的输入输出模式和初始状态

## 基本定时器 (BTM)

### BTM_Test()
```c
void BTM_Test(void);
```
**功能**: 基本定时器测试函数
**参数**: 无
**返回**: 无
**说明**: 演示基本定时器的配置和使用

## 外部中断 (EXTI)

### EXTI_Test()
```c
void EXTI_Test(void);
```
**功能**: 外部中断测试函数
**参数**: 无
**返回**: 无
**说明**: 演示外部中断的配置和中断处理

## 定时器 (Timer)

### Timer_Test()
```c
void Timer_Test(void);
```
**功能**: 定时器测试函数
**参数**: 无
**返回**: 无
**说明**: 演示定时器的配置和使用

## PWM

### PWM_Test()
```c
void PWM_Test(void);
```
**功能**: PWM测试函数
**参数**: 无
**返回**: 无
**说明**: 演示PWM波形的生成和配置

## 串口通信 (UART)

### Uart_Test()
```c
void Uart_Test(void);
```
**功能**: 串口通信测试函数
**参数**: 无
**返回**: 无
**说明**: 演示串口的初始化、发送和接收

## 通用串行通信接口 (USCI)

### USCI0_Test()
```c
void USCI0_Test(void);
```
**功能**: USCI0测试函数
**参数**: 无
**返回**: 无
**说明**: 演示USCI0接口的使用(SPI/I²C模式)

### USCI1_Test()
```c
void USCI1_Test(void);
```
**功能**: USCI1测试函数
**参数**: 无
**返回**: 无
**说明**: 演示USCI1接口的使用

### USCI2_Test()
```c
void USCI2_Test(void);
```
**功能**: USCI2测试函数
**参数**: 无
**返回**: 无
**说明**: 演示USCI2接口的使用

## 模数转换器 (ADC)

### ADC_Test()
```c
void ADC_Test(void);
```
**功能**: ADC测试函数
**参数**: 无
**返回**: 无
**说明**: 演示ADC的配置和模拟信号采集

## 在线应用编程 (IAP)

### IAP_Test()
```c
void IAP_Test(unsigned int addr);
```
**功能**: IAP测试函数
**参数**: 
- `addr`: 要操作的Flash地址
**返回**: 无
**说明**: 演示Flash存储器的读写操作

## 乘除法运算器

### Multiplier_Divider_Test()
```c
void Multiplier_Divider_Test(void);
```
**功能**: 乘除法运算测试函数
**参数**: 无
**返回**: 无
**说明**: 演示硬件乘除法器的使用

## CRC校验

### CRC_Test()
```c
void CRC_Test(void);
```
**功能**: CRC校验测试函数
**参数**: 无
**返回**: 无
**说明**: 演示CRC校验功能

## 智能卡接口

### SmartCard_Test()
```c
void SmartCard_Test(void);
```
**功能**: 智能卡测试函数
**参数**: 无
**返回**: 无
**说明**: 演示智能卡接口的使用

## 寄存器定义

以下是一些重要的寄存器定义（在SC95F852x_C.H中）：

### 端口寄存器
```c
sfr P0 = 0x80;      // 端口0数据寄存器
sfr P1 = 0x90;      // 端口1数据寄存器
sfr P2 = 0xA0;      // 端口2数据寄存器
sfr P3 = 0xB0;      // 端口3数据寄存器
```

### 定时器寄存器
```c
sfr TCON = 0x88;    // 定时器控制寄存器
sfr TMOD = 0x89;    // 定时器模式寄存器
sfr TL0 = 0x8A;     // 定时器0低字节
sfr TL1 = 0x8B;     // 定时器1低字节
sfr TH0 = 0x8C;     // 定时器0高字节
sfr TH1 = 0x8D;     // 定时器1高字节
```

### 串口寄存器
```c
sfr SCON = 0x98;    // 串口控制寄存器
sfr SBUF = 0x99;    // 串口数据缓冲区
```

### 中断寄存器
```c
sfr IE = 0xA8;      // 中断使能寄存器
sfr IP = 0xB8;      // 中断优先级寄存器
```

## 位定义

### TCON寄存器位定义
```c
sbit TF1 = TCON^7;  // 定时器1溢出标志
sbit TR1 = TCON^6;  // 定时器1运行控制
sbit TF0 = TCON^5;  // 定时器0溢出标志
sbit TR0 = TCON^4;  // 定时器0运行控制
sbit IE1 = TCON^3;  // 外部中断1边沿标志
sbit IT1 = TCON^2;  // 外部中断1触发方式
sbit IE0 = TCON^1;  // 外部中断0边沿标志
sbit IT0 = TCON^0;  // 外部中断0触发方式
```

### SCON寄存器位定义
```c
sbit SM0 = SCON^7;  // 串口模式位0
sbit SM1 = SCON^6;  // 串口模式位1
sbit SM2 = SCON^5;  // 多机通信位
sbit REN = SCON^4;  // 接收使能位
sbit TB8 = SCON^3;  // 发送数据位8
sbit RB8 = SCON^2;  // 接收数据位8
sbit TI = SCON^1;   // 发送中断标志
sbit RI = SCON^0;   // 接收中断标志
```

## 使用示例

### 基本GPIO操作
```c
// 初始化IO
IO_Init();

// 设置P1.0为输出高电平
P1 |= 0x01;

// 设置P1.0为输出低电平  
P1 &= 0xFE;

// 读取P2.0的状态
if (P2 & 0x01) {
    // P2.0为高电平
}
```

### 定时器使用示例
```c
// 配置定时器0为模式1(16位定时器)
TMOD = 0x01;

// 设置定时器初值(1ms@12MHz)
TH0 = 0xFC;
TL0 = 0x18;

// 启动定时器0
TR0 = 1;

// 等待定时器溢出
while (!TF0);
TF0 = 0;  // 清除溢出标志
```

### 串口通信示例
```c
// 配置串口为模式1,8位异步通信
SCON = 0x50;

// 发送数据
SBUF = 'A';
while (!TI);  // 等待发送完成
TI = 0;       // 清除发送标志

// 接收数据
while (!RI);  // 等待接收完成
data = SBUF;  // 读取接收数据
RI = 0;       // 清除接收标志
```

## 注意事项

1. 在使用中断功能时，请确保正确配置中断优先级
2. 使用定时器时注意时钟频率的计算
3. ADC使用前需要适当的稳定时间
4. IAP操作时要注意Flash扇区的擦除和编程时序
5. 智能卡接口需要符合ISO7816标准的时序要求

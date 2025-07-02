# SC95F8523单片机演示代码

这是一个基于SinOne SC95F8523/8522/8521/8520S/8520M系列单片机的演示代码项目。

## 项目结构

```
├── SC95F8523_8522_8521_8520S_8520M_Demo_Code/  # 主要代码目录
│   ├── c/                                      # C源文件
│   │   ├── main.c                             # 主程序文件
│   │   ├── ADC_Init.c                         # ADC初始化
│   │   ├── BTM.c                              # 基本定时器
│   │   ├── CRC_Init.c                         # CRC校验
│   │   ├── EX_Init.c                          # 外部中断
│   │   ├── IAP_Init.c                         # IAP在线编程
│   │   ├── IO_Init.c                          # IO初始化
│   │   ├── PWM_Init.c                         # PWM初始化
│   │   ├── Timer_Init.c                       # 定时器初始化
│   │   ├── Uart_Init.c                        # 串口初始化
│   │   ├── USCI0_Init.c                       # USCI0初始化
│   │   ├── USCI1_Init.c                       # USCI1初始化
│   │   ├── USCI2_Init.c                       # USCI2初始化
│   │   ├── SSI0_Init.c                        # SSI0初始化
│   │   ├── SSI1_Init.c                        # SSI1初始化
│   │   ├── SSI2_Init.c                        # SSI2初始化
│   │   ├── SmartCard.c                        # 智能卡
│   │   └── Multiplication_Division.c          # 乘除法运算
│   ├── H/                                     # 头文件
│   │   ├── Function_Init.H                    # 功能初始化头文件
│   │   ├── Multiplication_Division.H          # 乘除法运算头文件
│   │   └── SC95F852x_C.H                     # 芯片寄存器定义
│   ├── Demo.uvproj                            # Keil项目文件
│   ├── Demo.uvopt                             # Keil选项文件
│   ├── STARTUP.A51                            # 启动文件
│   └── Templst.soclst                         # 模板列表
├── SC95F8523_8522_8521v1.0cn.pdf             # 中文数据手册
└── SC95F8523_8522_8521_8520S_8520M_Demo_Code_20240305.rar  # 原始代码包
```

## 功能特性

本演示代码包含以下功能模块：

- **BTM (Basic Timer Module)** - 基本定时器模块
- **EXTI (External Interrupt)** - 外部中断
- **Timer** - 定时器功能
- **PWM** - 脉宽调制输出
- **UART** - 串口通信
- **USCI (Universal Serial Communication Interface)** - 通用串行通信接口 (USCI0/1/2)
- **ADC** - 模数转换器
- **IAP (In Application Programming)** - 在线应用编程
- **乘除法运算器** - 硬件乘除法运算
- **CRC** - 循环冗余校验
- **SmartCard** - 智能卡接口

## 开发环境

- **IDE**: Keil uVision
- **编译器**: C51
- **目标芯片**: SC95F8523/8522/8521/8520S/8520M
- **架构**: 8051兼容

## 使用说明

1. 使用Keil uVision打开`Demo.uvproj`项目文件
2. 在`main.c`文件中修改`Test`宏定义来选择要测试的功能模块：
   ```c
   #define Test  6    // 0-12对应不同的功能模块
   ```
3. 编译并下载到目标芯片

### 功能模块对应表

| Test值 | 功能模块 | 描述 |
|--------|----------|------|
| 0      | BTM      | 基本定时器测试 |
| 1      | EXTI     | 外部中断测试 |
| 2      | Timer    | 定时器测试 |
| 3      | PWM      | PWM输出测试 |
| 4      | Uart     | 串口通信测试 |
| 5      | USCI0    | USCI0通信测试 |
| 6      | USCI1    | USCI1通信测试 |
| 7      | USCI2    | USCI2通信测试 |
| 8      | ADC      | ADC转换测试 |
| 9      | IAP      | IAP编程测试 |
| 10     | 乘除法    | 硬件乘除法测试 |
| 11     | CRC      | CRC校验测试 |
| 12     | SmartCard| 智能卡测试 |

## 注意事项

1. 在项目设置中，需要在`Options for Target → Target1 → BL51 Locate → Code Range`设置为`0x100`
2. 可以选择`DISRST`复位，将复位引脚作为普通IO使用
3. 注意在`Function.H`中选择正确的型号
4. 版本：V1.0

## 芯片特性

SC95F8523系列是基于8051内核的高性能8位单片机，具有以下特点：

- 8051兼容指令集
- 多种通信接口（UART、SPI、I²C等）
- 丰富的定时器资源
- 多通道ADC
- 硬件乘除法器
- 智能卡接口
- IAP功能支持

## 许可证

请参考SinOne官方许可证协议。

## 联系方式

如有技术问题，请联系SinOne技术支持。

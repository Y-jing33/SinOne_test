# 贡献指南

感谢您对SC95F8523演示代码项目的关注！我们欢迎任何形式的贡献。

## 贡献方式

### 报告Bug
- 使用GitHub Issues报告bug
- 使用提供的bug报告模板
- 包含详细的复现步骤
- 提供编译器和硬件环境信息

### 建议新功能
- 使用GitHub Issues提出功能请求
- 使用提供的功能请求模板
- 清楚描述功能的价值和用例

### 提交代码
- Fork这个仓库
- 创建您的功能分支 (`git checkout -b feature/AmazingFeature`)
- 提交您的更改 (`git commit -m 'Add some AmazingFeature'`)
- 推送到分支 (`git push origin feature/AmazingFeature`)
- 打开一个Pull Request

## 开发规范

### 代码风格
- 遵循K&R C语言编码风格
- 使用4个空格缩进，不使用制表符
- 函数名使用PascalCase格式
- 变量名使用camelCase格式
- 常量和宏使用UPPER_CASE格式

### 示例代码风格
```c
// 函数定义
void Timer_Init(void)
{
    TMOD = 0x01;    // 设置定时器模式
    TH0 = 0xFC;     // 高字节初值
    TL0 = 0x18;     // 低字节初值
    TR0 = 1;        // 启动定时器
}

// 变量定义
unsigned char timerCount = 0;
unsigned int adcResult;

// 宏定义
#define TIMER_INIT_VALUE    0xFC18
#define LED_ON              1
#define LED_OFF             0
```

### 注释规范
- 为每个函数添加功能说明注释
- 在复杂的代码段添加解释性注释
- 使用中文注释说明业务逻辑
- 使用英文注释说明技术细节

```c
/**
 * @brief 初始化ADC模块
 * @param channel ADC通道选择 (0-7)
 * @param vref 参考电压选择
 * @return 初始化结果 (0:成功, 1:失败)
 */
unsigned char ADC_Init(unsigned char channel, unsigned char vref)
{
    // 检查参数有效性
    if (channel > 7) {
        return 1;   // 参数错误
    }
    
    // 配置ADC寄存器
    ADCON0 = 0x80 | channel;    // 使能ADC并选择通道
    ADCON1 = vref;              // 设置参考电压
    
    return 0;   // 初始化成功
}
```

### 文件组织
- 每个功能模块使用单独的.c和.h文件
- 头文件只包含函数声明和必要的宏定义
- 源文件包含具体的实现代码
- 相关的功能放在同一个文件中

### 提交消息格式
使用以下格式编写提交消息：

```
<类型>: <简短描述>

<详细描述>

<相关Issue>
```

类型包括：
- `feat`: 新功能
- `fix`: bug修复
- `docs`: 文档更新
- `style`: 代码格式化（不影响功能）
- `refactor`: 代码重构
- `test`: 添加或修改测试
- `chore`: 构建过程或辅助工具的变动

示例：
```
feat: 添加SPI主机模式支持

- 实现SPI主机模式初始化函数
- 添加数据发送和接收函数
- 包含完整的示例代码

Fixes #123
```

## 测试要求

### 编译测试
- 确保代码在Keil uVision中编译无错误和警告
- 测试不同的芯片型号配置
- 验证所有功能模块都能正确编译

### 功能测试
- 在实际硬件上测试新功能
- 确保不破坏现有功能
- 提供测试用例和预期结果

### 文档测试
- 确保README.md准确反映当前功能
- 更新API文档
- 检查示例代码的正确性

## 代码审查

### 自我审查检查清单
- [ ] 代码遵循项目编码规范
- [ ] 添加了适当的注释
- [ ] 更新了相关文档
- [ ] 编译无错误和警告
- [ ] 在硬件上测试通过
- [ ] 不包含调试代码或临时文件

### Pull Request要求
- 使用提供的PR模板
- 包含清晰的变更描述
- 链接到相关的Issue
- 通过所有CI检查
- 获得至少一个维护者的审查批准

## 发布流程

### 版本管理
- 遵循[语义化版本](https://semver.org/lang/zh-CN/)规范
- 主要版本：不兼容的API更改
- 次要版本：向后兼容的功能性新增
- 补丁版本：向后兼容的问题修正

### 发布步骤
1. 更新CHANGELOG.md
2. 更新版本号
3. 创建发布标签
4. 编写发布说明
5. 发布到GitHub Releases

## 社区准则

### 行为规范
- 保持专业和尊重的态度
- 欢迎新贡献者
- 提供建设性的反馈
- 专注于项目目标

### 沟通方式
- 使用GitHub Issues进行技术讨论
- 在PR中进行代码审查
- 保持讨论的技术性和客观性

## 技术支持

### 常见问题
1. **编译错误**: 检查Keil配置和芯片型号设置
2. **硬件问题**: 确认硬件连接和时钟配置
3. **功能异常**: 检查寄存器配置和中断设置

### 获得帮助
- 查看文档和示例代码
- 搜索现有的Issues
- 创建新的Issue寻求帮助
- 参考芯片数据手册

## 许可证

通过贡献代码，您同意您的贡献将在MIT许可证下授权。

感谢您的贡献！

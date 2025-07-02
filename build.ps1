# SC95F8523 Demo Code Build Script for Windows
# PowerShell脚本用于自动化编译过程

param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("build", "clean", "rebuild", "info", "help")]
    [string]$Action = "help"
)

# 脚本配置
$ProjectName = "Demo"
$ProjectDir = "SC95F8523_8522_8521_8520S_8520M_Demo_Code"
$KeilPath = "C:\Keil_v5\UV4\UV4.exe"  # 根据实际安装路径调整

# 颜色定义
$Colors = @{
    Red = "Red"
    Green = "Green"
    Yellow = "Yellow"
    Cyan = "Cyan"
}

# 打印带颜色的消息
function Write-ColorMessage {
    param(
        [string]$Message,
        [string]$Color = "White"
    )
    Write-Host $Message -ForegroundColor $Color
}

# 检查Keil是否安装
function Test-KeilInstallation {
    Write-ColorMessage "检查Keil uVision安装..." $Colors.Yellow
    
    if (-not (Test-Path $KeilPath)) {
        Write-ColorMessage "错误: 找不到Keil uVision，请检查安装路径" $Colors.Red
        Write-ColorMessage "当前配置路径: $KeilPath" $Colors.Yellow
        Write-ColorMessage "请修改脚本中的KeilPath变量" $Colors.Yellow
        exit 1
    }
    
    Write-ColorMessage "Keil uVision 已找到" $Colors.Green
}

# 检查项目文件
function Test-ProjectFiles {
    Write-ColorMessage "检查项目文件..." $Colors.Yellow
    
    if (-not (Test-Path $ProjectDir)) {
        Write-ColorMessage "错误: 找不到项目目录 $ProjectDir" $Colors.Red
        exit 1
    }
    
    $projectFile = Join-Path $ProjectDir "$ProjectName.uvproj"
    if (-not (Test-Path $projectFile)) {
        Write-ColorMessage "错误: 找不到项目文件 $ProjectName.uvproj" $Colors.Red
        exit 1
    }
    
    Write-ColorMessage "项目文件检查通过" $Colors.Green
}

# 清理编译输出
function Clear-BuildOutput {
    Write-ColorMessage "清理编译输出..." $Colors.Yellow
    
    Push-Location $ProjectDir
    
    try {
        # 删除编译生成的文件
        $filesToRemove = @(
            "*.hex", "*.bin", "*.axf", "*.map", "*.lnp", "*.lst", "*.obj", "*.o",
            "*.plg", "*.tra", "*.htm", "*.build_log.htm", "build.log"
        )
        
        foreach ($pattern in $filesToRemove) {
            Get-ChildItem -Path . -Filter $pattern | Remove-Item -Force -ErrorAction SilentlyContinue
        }
        
        # 删除目录
        $dirsToRemove = @("Objects", "Listings", "DebugConfig")
        foreach ($dir in $dirsToRemove) {
            if (Test-Path $dir) {
                Remove-Item -Path $dir -Recurse -Force -ErrorAction SilentlyContinue
            }
        }
        
        Write-ColorMessage "清理完成" $Colors.Green
    }
    finally {
        Pop-Location
    }
}

# 编译项目
function Build-Project {
    Write-ColorMessage "开始编译项目..." $Colors.Yellow
    
    Push-Location $ProjectDir
    
    try {
        # 使用Keil命令行编译
        $arguments = @("-b", "$ProjectName.uvproj", "-o", "build.log")
        $process = Start-Process -FilePath $KeilPath -ArgumentList $arguments -Wait -PassThru -NoNewWindow
        
        # 检查编译结果
        if ($process.ExitCode -eq 0) {
            Write-ColorMessage "编译成功!" $Colors.Green
            
            # 显示生成的文件
            $hexFile = "Objects\$ProjectName.hex"
            if (Test-Path $hexFile) {
                Write-ColorMessage "生成的HEX文件: $hexFile" $Colors.Green
                Get-ChildItem $hexFile | Format-Table Name, Length, LastWriteTime
            }
            
            $axfFile = "Objects\$ProjectName.axf"
            if (Test-Path $axfFile) {
                Write-ColorMessage "生成的AXF文件: $axfFile" $Colors.Green
                Get-ChildItem $axfFile | Format-Table Name, Length, LastWriteTime
            }
        }
        else {
            Write-ColorMessage "编译失败!" $Colors.Red
            
            # 显示编译日志
            if (Test-Path "build.log") {
                Write-ColorMessage "编译日志:" $Colors.Yellow
                Get-Content "build.log"
            }
            
            exit 1
        }
    }
    finally {
        Pop-Location
    }
}

# 显示项目信息
function Show-ProjectInfo {
    Write-ColorMessage "项目信息:" $Colors.Yellow
    Write-Host "  项目名称: $ProjectName"
    Write-Host "  项目目录: $ProjectDir"
    Write-Host "  目标芯片: SC95F8523/8522/8521/8520S/8520M"
    Write-Host "  开发环境: Keil uVision + C51"
    Write-Host "  Keil路径: $KeilPath"
    Write-Host ""
}

# 显示帮助信息
function Show-Help {
    Write-Host "SC95F8523 Demo Code Build Script for Windows" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "用法: .\build.ps1 [选项]" -ForegroundColor White
    Write-Host ""
    Write-Host "选项:" -ForegroundColor White
    Write-Host "  build     - 编译项目" -ForegroundColor Green
    Write-Host "  clean     - 清理编译输出" -ForegroundColor Green
    Write-Host "  rebuild   - 清理后重新编译" -ForegroundColor Green
    Write-Host "  info      - 显示项目信息" -ForegroundColor Green
    Write-Host "  help      - 显示此帮助信息" -ForegroundColor Green
    Write-Host ""
    Write-Host "示例:" -ForegroundColor White
    Write-Host "  .\build.ps1 build      # 编译项目" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 clean      # 清理编译输出" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 rebuild    # 清理后重新编译" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "注意:" -ForegroundColor Red
    Write-Host "  请确保Keil uVision已正确安装并且路径配置正确" -ForegroundColor Red
    Write-Host "  如果遇到执行策略问题，请运行: Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser" -ForegroundColor Red
    Write-Host ""
}

# 主函数
function Main {
    switch ($Action) {
        "build" {
            Show-ProjectInfo
            Test-KeilInstallation
            Test-ProjectFiles
            Build-Project
        }
        "clean" {
            Show-ProjectInfo
            Test-ProjectFiles
            Clear-BuildOutput
        }
        "rebuild" {
            Show-ProjectInfo
            Test-KeilInstallation
            Test-ProjectFiles
            Clear-BuildOutput
            Build-Project
        }
        "info" {
            Show-ProjectInfo
        }
        "help" {
            Show-Help
        }
        default {
            Write-ColorMessage "未知操作: $Action" $Colors.Red
            Write-Host ""
            Show-Help
        }
    }
}

# 检查PowerShell执行策略
$executionPolicy = Get-ExecutionPolicy
if ($executionPolicy -eq "Restricted") {
    Write-ColorMessage "警告: PowerShell执行策略为Restricted，可能无法执行脚本" $Colors.Red
    Write-ColorMessage "请以管理员身份运行: Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser" $Colors.Yellow
    Write-Host ""
}

# 运行主函数
Main

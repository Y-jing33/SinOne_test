#!/bin/bash
# SC95F8523 Demo Code Build Script
# This script helps automate the build process

# 脚本配置
PROJECT_NAME="Demo"
PROJECT_DIR="SC95F8523_8522_8521_8520S_8520M_Demo_Code"
KEIL_PATH="/c/Keil_v5/UV4/UV4.exe"  # 根据实际安装路径调整

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_message() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# 检查Keil是否安装
check_keil() {
    print_message $YELLOW "检查Keil uVision安装..."
    
    if [ ! -f "$KEIL_PATH" ]; then
        print_message $RED "错误: 找不到Keil uVision，请检查安装路径"
        print_message $YELLOW "当前配置路径: $KEIL_PATH"
        print_message $YELLOW "请修改脚本中的KEIL_PATH变量"
        exit 1
    fi
    
    print_message $GREEN "Keil uVision 已找到"
}

# 检查项目文件
check_project() {
    print_message $YELLOW "检查项目文件..."
    
    if [ ! -d "$PROJECT_DIR" ]; then
        print_message $RED "错误: 找不到项目目录 $PROJECT_DIR"
        exit 1
    fi
    
    if [ ! -f "$PROJECT_DIR/${PROJECT_NAME}.uvproj" ]; then
        print_message $RED "错误: 找不到项目文件 ${PROJECT_NAME}.uvproj"
        exit 1
    fi
    
    print_message $GREEN "项目文件检查通过"
}

# 清理编译输出
clean_build() {
    print_message $YELLOW "清理编译输出..."
    
    cd "$PROJECT_DIR"
    
    # 删除编译生成的文件
    rm -f *.hex *.bin *.axf *.map *.lnp *.lst *.obj *.o
    rm -f *.plg *.tra *.htm *.build_log.htm
    rm -rf Objects/ Listings/ DebugConfig/
    
    cd ..
    print_message $GREEN "清理完成"
}

# 编译项目
build_project() {
    print_message $YELLOW "开始编译项目..."
    
    cd "$PROJECT_DIR"
    
    # 使用Keil命令行编译
    "$KEIL_PATH" -b "${PROJECT_NAME}.uvproj" -o build.log
    
    # 检查编译结果
    if [ $? -eq 0 ]; then
        print_message $GREEN "编译成功!"
        
        # 显示生成的文件
        if [ -f "Objects/${PROJECT_NAME}.hex" ]; then
            print_message $GREEN "生成的HEX文件: Objects/${PROJECT_NAME}.hex"
            ls -la "Objects/${PROJECT_NAME}.hex"
        fi
        
        if [ -f "Objects/${PROJECT_NAME}.axf" ]; then
            print_message $GREEN "生成的AXF文件: Objects/${PROJECT_NAME}.axf"
            ls -la "Objects/${PROJECT_NAME}.axf"
        fi
    else
        print_message $RED "编译失败!"
        
        # 显示编译日志
        if [ -f "build.log" ]; then
            print_message $YELLOW "编译日志:"
            cat build.log
        fi
        
        exit 1
    fi
    
    cd ..
}

# 显示项目信息
show_info() {
    print_message $YELLOW "项目信息:"
    echo "  项目名称: $PROJECT_NAME"
    echo "  项目目录: $PROJECT_DIR"
    echo "  目标芯片: SC95F8523/8522/8521/8520S/8520M"
    echo "  开发环境: Keil uVision + C51"
    echo ""
}

# 显示帮助信息
show_help() {
    echo "SC95F8523 Demo Code Build Script"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  build     - 编译项目"
    echo "  clean     - 清理编译输出"
    echo "  rebuild   - 清理后重新编译"
    echo "  info      - 显示项目信息"
    echo "  help      - 显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 build      # 编译项目"
    echo "  $0 clean      # 清理编译输出"
    echo "  $0 rebuild    # 清理后重新编译"
    echo ""
}

# 主函数
main() {
    case "$1" in
        "build")
            show_info
            check_keil
            check_project
            build_project
            ;;
        "clean")
            show_info
            check_project
            clean_build
            ;;
        "rebuild")
            show_info
            check_keil
            check_project
            clean_build
            build_project
            ;;
        "info")
            show_info
            ;;
        "help"|"--help"|"-h")
            show_help
            ;;
        "")
            print_message $YELLOW "未指定操作，显示帮助信息:"
            echo ""
            show_help
            ;;
        *)
            print_message $RED "未知选项: $1"
            echo ""
            show_help
            exit 1
            ;;
    esac
}

# 运行主函数
main "$@"

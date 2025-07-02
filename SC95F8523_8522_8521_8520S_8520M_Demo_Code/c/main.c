#include "H/Function_Init.H"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号
4、版本：V1.0
***************************************************************/
#define Test  6    //BTM:0 EXTI:1 Timer:2 PWM:3 Uart0:4 USCI0:5 USCI1:6 USCI2:7 ADC:8 IAP:9 Multiplier_Divider:10 CRC:11 SmartCard:12

void main(void)
{
  IO_Init();
//	WDTCON |= 0x10;		    //清看门狗
  switch(Test)
  {
    case 0:
      BTM_Test();
      break;
    case 1:
      EXTI_Test();
      break;
    case 2:
      Timer_Test();
      break;
    case 3:
      PWM_Test();
      break;
    case 4:
      Uart_Test();
      break;
    case 5:
      USCI0_Test();
      break;
    case 6:
      USCI1_Test();
      break;
    case 7:
      USCI2_Test();
      break;
    case 8:
      ADC_Test();
      break;
    case 9:
      IAP_Test(0x1FFF);   //对地址0x1fff进行读写测试
      break;
    case 10:
      Multiplier_Divider_Test();
      break;
    case 11:
      CRC_Test();
      break;
    case 12:
      SmartCard_Test();//需要修改USCI0_Init.c中的USCI0_Mode为SmartCard
      break;
    default:
      break;
  }
}



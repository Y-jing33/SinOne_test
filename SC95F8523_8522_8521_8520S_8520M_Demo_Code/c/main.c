#include "H/Function_Init.H"

/**************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
3��ע�⣺����Function.H����ѡ������ͺ�
4���汾��V1.0
***************************************************************/
#define Test  6    //BTM:0 EXTI:1 Timer:2 PWM:3 Uart0:4 USCI0:5 USCI1:6 USCI2:7 ADC:8 IAP:9 Multiplier_Divider:10 CRC:11 SmartCard:12

void main(void)
{
  IO_Init();
//	WDTCON |= 0x10;		    //�忴�Ź�
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
      IAP_Test(0x1FFF);   //�Ե�ַ0x1fff���ж�д����
      break;
    case 10:
      Multiplier_Divider_Test();
      break;
    case 11:
      CRC_Test();
      break;
    case 12:
      SmartCard_Test();//��Ҫ�޸�USCI0_Init.c�е�USCI0_ModeΪSmartCard
      break;
    default:
      break;
  }
}



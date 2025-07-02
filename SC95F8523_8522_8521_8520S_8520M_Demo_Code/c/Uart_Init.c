#include "H/Function_Init.H" 
uint UART_RxData[5];//����5��8λ�Զ������ݣ���Ҫ�޸����ݸ������޸���Ӧ�����С�ͷ���/����forѭ����С��
void Uart_Init(uint Freq,unsigned long int baud);
bit UartSendFlag = 0; //�����жϱ�־λ
bit UartReceiveFlag = 0; //�����жϱ�־λ
/*****************************************************
*�������ƣ�void Uart_Test(void)
*�������ܣ�Uart����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Uart_Test(void)
{
	int num1,num2;
	Uart_Init(32,115200);
	while(1)
	{
		for(num1=0;num1<5;num1++)
		{
			while(!UartReceiveFlag);
			UartReceiveFlag = 0;
			UART_RxData[num1] = SBUF;
		}
		for(num2=0;num2<5;num2++)
		{
			SBUF = UART_RxData[num2];
			while(!UartSendFlag);
			UartSendFlag = 0;
		}

		Delay(1000);
	}
}
/*****************************************************
*�������ƣ�void Uart_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart_Init(uint Freq,unsigned long int baud)    //ѡ��Timer1��Ϊ�������źŷ�����
{
	P1CON &= 0xF3;   //TX/RX����Ϊ���������
	P1PH  |= 0x0C;
	
	SCON  |= 0X50;   //����ͨ�ŷ�ʽΪģʽһ���������
	TMCON |= 0X02;
	TH1 = (Freq*1000000/baud)>>8;	  //������ΪT1�����ʱ�䣻
	TL1 = Freq*1000000/baud;
	TR1 = 0;
	ET1 = 0;
	EUART = 1;     //����Uart�ж�
	EA = 1;
}

/*
void Uart_Init(uint Freq,unsigned long int baud)    //ѡ��Timer2��Ϊ�������źŷ�����
{
	P1CON &= 0xF3;   //TX/RX����Ϊ���������
	P1PH  |= 0x0C;

	SCON  |= 0X50;   //����ͨ�ŷ�ʽΪģʽһ���������
    TXINX = 0x02;
	TMCON |= 0X04;
	TXMOD = 0X00;
	TXCON = 0X30;
	RCAPXH = Freq*1000000/baud/256;
	RCAPXL = Freq*1000000/baud%256;
	TRX = 0;
	ET2 = 0;
	EUART = 1;     //����Uart�ж�
	EA = 1;
}
*/

/*****************************************************
*�������ƣ�void UartInt(void) interrupt 4
*�������ܣ�Uart�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		RI = 0;	
		UartReceiveFlag = 1;
	}	
}

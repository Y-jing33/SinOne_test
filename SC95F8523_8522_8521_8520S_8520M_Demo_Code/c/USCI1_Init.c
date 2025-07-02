#include "H/Function_Init.H"

//ѡ������һģʽ��
#define  Uart1   0
#define  TWI1    1
#define  SPI1    2

#define  USCI1_Mode  SPI1

#define TWI1_Mode 1//0:��������ģʽ   1���ӻ�����ģʽ
#define SPI1_Mode 1 //0:��������ģʽ   1���ӻ�����ģʽ

void Uart1_Init(uint Freq, unsigned long int baud);
void TWI1_Init(void);
void SPI1_Init(void);

bit Uart1SendFlag = 0;    //Uart1�����жϱ�־λ
bit Uart1ReceiveFlag = 0; //Uart1�����жϱ�־λ
bit SPI1Flag = 0;         //SPI1���ݴ�����ɱ�־λ
bit TWI1Flag = 0;         //TWI1�жϱ�־λ
uint8_t TxXferCount1 = 0;  //SPI/TWI����������Ŀ
uint8_t RxXferCount1 = 0;  //SPI/TWI����������Ŀ


volatile uint8_t Uart1_RxData[5];

uint8_t SPI1_Master_SendData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t SPI1_Slaver_ReceiveData[8];

uint8_t TWI1_Master_SendData[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t TWI1_Slaver_ReceiveData[8];
/*****************************************************
*�������ƣ�void USCI1_Test(void)
*�������ܣ�USCI1����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void USCI1_Test(void)
{
#if (USCI1_Mode == Uart1)
		int num1,num2;
	Uart1_Init(32,9600);
	while(1)
	{
			for(num1=0;num1<5;num1++)
			{
				while(!Uart1ReceiveFlag);
				Uart1ReceiveFlag = 0;
				Uart1_RxData[num1] = US1CON3 ;
			}
			for(num2=0;num2<5;num2++)
			{
				US1CON3  = Uart1_RxData[num2];
				while(!Uart1SendFlag);
				Uart1SendFlag = 0;
			}
#endif
	
#if (USCI1_Mode == TWI1)
  TWI1_Init();
  while(1)
  {
#if (TWI1_Mode == 0)  //����
		  US1CON1 |= 0x20;    //������ʼ����
		  Delay(100);
      while(TWI1Flag == 0);
      TWI1Flag = 0; 
      US1CON3 = 0x02;	    //���͵�ַ��д����
      while(TWI1Flag == 0);
      TWI1Flag = 0; 
      while(TxXferCount1 < 8)
			{
				US1CON3 = TWI1_Master_SendData[TxXferCount1];             //TWI��������
				while(TWI1Flag == 0);
				TWI1Flag = 0; 
			  TxXferCount1++;
			}
     TxXferCount1=0;

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      US1CON1 |= 0x10;
	
			Delay(1000);
#endif   
#if (TWI1_Mode == 1)  //�ӻ�  

      while((0x07 & US1CON0)!=0x02);//�ж�Ϊ����״̬
			Delay(100);
			while(TWI1Flag == 0);
			TWI1Flag = 0;                 
     	Delay(100);
    /* �������ݹ��� */
			do
			{
				if(RxXferCount1 == 7) US1CON0 &= 0XF7;   //AAʹ��λ,������UAC
				while(TWI1Flag == 0);
				TWI1Flag = 0;  
				TWI1_Slaver_ReceiveData[RxXferCount1] = US1CON3;             //TWI��������
				RxXferCount1++; 
			}  while(RxXferCount1 < 8);

			US1CON0 |= 0X08; //AAʹ��λ
			RxXferCount1 =0;
			while((0x07 & US1CON0)!=0x00);
  
		
#endif
		}
#endif
	
#if (USCI1_Mode == SPI1)
  SPI1_Init();
	
			while(1)
			{
#if (SPI1_Mode == 0)  //����
				TxXferCount1 = 0;
				while(TxXferCount1 <  8)//�ж��Ƿ������������
				{
				 
					US1CON2 = SPI1_Master_SendData[TxXferCount1];
					while(SPI1Flag == 0);	//�ȴ��������		   
					SPI1Flag = 0;		
					TxXferCount1++;	//��������������
				}
				Delay(1000);
#endif
#if (SPI1_Mode == 1)  //�ӻ�
			while(RxXferCount1 <8)
				{
					/* �ȴ�SPI�жϱ�־λ���� */
					while(SPI1Flag == 0);	//�ȴ��������
					SPI1Flag = 0;		
					SPI1_Slaver_ReceiveData[RxXferCount1] = US1CON2;			//��ȡ����
					RxXferCount1++;	//����������������1       
				}
				RxXferCount1 = 0;
				Delay(100);

#endif
		
 
  }
#endif
}
/*****************************************************
*�������ƣ�void Uart1_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart1�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart1_Init(uint Freq,unsigned long int baud)
{
	P0CON &= 0xF3;    //TX/RX����Ϊ���������
	P0PH  |= 0x0C;
	
	OTCON |= 0xC0;    //���нӿ�USCI1ѡ��Uart1ͨ��
	US1CON0 = 0x50;   //����ͨ�ŷ�ʽΪģʽһ���������
	US1CON1 = Freq*1000000/baud;   //�����ʵ�λ����
	US1CON2 = (Freq*1000000/baud)>>8;   //�����ʸ�λ����
	IE2 |= 0x01;      //����USCI1�ж�
    EA = 1;	
}

/*****************************************************
*�������ƣ�void TWI1_Init(void)
*�������ܣ�TWI1��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWI1_Init(void)
{
  OTCON |= 0x80;   //ѡ��TWI1ģʽ
  US1CON0 = 0x88;  //��ģʽ�� ʹ��Ӧ���־λ
  US1CON1 = 0x40;  //---- xxxx  xΪʱ������
  US1CON2 = 0x02;  //�ӻ���ַ
  IE2 |= 0x01;
  EA = 1;
}

/*****************************************************
*�������ƣ�void SPI1_Init(void)
*�������ܣ�SPI1��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPI1_Init(void)
{
	OTCON |= 0X40;    //ѡ��SPI1ģʽ
#if (SPI1_Mode == 0)	
 {  US1CON0 = 0x23;   }   //����SPI0Ϊ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#elif (SPI1_Mode == 1)	
 {  US1CON0 = 0x03;   }   //����SPI0Ϊ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#endif 
	US1CON1 = 0x00;   //MSB���ȷ��ͣ�8λ���䣬���������ж�
	US1CON0 |= 0x80;  //����SPI1
	IE2 |= 0x01;
	EA = 1;
}


/*****************************************************
*�������ƣ�void TWI1/PI/UART1_Int() interrupt 7
*�������ܣ�USCI1�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (USCI1_Mode == Uart1)
void Uart1_Int() interrupt 15   //Uart1�жϺ���
{
	if(US1CON0&0x02)    //���ͱ�־λ�ж�
	{
		US1CON0 &= 0xFD;
		Uart1SendFlag = 1;
	}
	if((US1CON0&0x01))  //���ձ�־λ�ж�
	{
		US1CON0 &= 0xFE;
		Uart1ReceiveFlag = 1;
	}	
}
#endif

#if (USCI1_Mode == TWI1)
void TWI1_Int() interrupt 15     //TWI1�жϺ���
{
	if(US1CON0&0x40)
	{
		US1CON0 &= 0xbf;  //�ж�����
		TWI1Flag = 1;
	}	
}
#endif 

#if (USCI1_Mode == SPI1)
void SpiInt(void) interrupt 15    //SPI1�жϺ���
{	  
	if(US1CON1&0X08)    //���ͻ������ձ�־�ж�
	{
		US1CON1 &= ~0X08;
	}
	if(US1CON1&0X80)    //���ݴ����־λ�ж�
	{
		US1CON1 &= ~0X80;
		SPI1Flag = 1;
	}
}
#endif 
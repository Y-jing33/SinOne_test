#include "H/Function_Init.H"

//ѡ������һģʽ��
#define  Uart2   0
#define  TWI2    1
#define  SPI2    2

#define  USCI2_Mode  TWI2
#define TWI2_Mode 0//0:��������ģʽ   1���ӻ�����ģʽ
#define SPI2_Mode 0 //0:��������ģʽ   1���ӻ�����ģʽ

void Uart2_Init(uint Freq,unsigned long int baud);
void TWI2_Init(void);
void SPI2_Init(void);

bit Uart2SendFlag = 0;     //Uart2�����жϱ�־λ
bit Uart2ReceiveFlag = 0;  //Uart2�����жϱ�־λ
bit SPI2Flag = 0;         //SPI2���ݴ�����ɱ�־λ
bit TWI2Flag = 0;         //TWI2�жϱ�־λ

uint8_t TxXferCount2 = 0;  //SPI/TWI����������Ŀ
uint8_t RxXferCount2 = 0;  //SPI/TWI����������Ŀ 
volatile uint8_t Uart2_RxData[5];
uint8_t SPI2_Master_SendData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t SPI2_Master_ReceiveData[8];
uint8_t SPI2_Slaver_SendData[8] = {0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
volatile uint8_t SPI2_Slaver_ReceiveData[8];

uint8_t TWI2_Master_SendData[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t TWI2_Master_ReceiveData[8];
uint8_t TWI2_Slaver_SendData[8]={0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
volatile uint8_t TWI2_Slaver_ReceiveData[8];
/*****************************************************
*�������ƣ�void USCI2_Test(void)
*�������ܣ�USCI2����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void USCI2_Test(void)
{
#if (USCI2_Mode == Uart2)
	int num1,num2;
	Uart2_Init(32,9600);
	while(1)
	{
		   for(num1=0;num1<5;num1++)
		{
			while(!Uart2ReceiveFlag);
			Uart2ReceiveFlag = 0;
			Uart2_RxData[num1] = US2CON3 ;
		}
		for(num2=0;num2<5;num2++)
		{
			US2CON3  = Uart2_RxData[num2];
		  	while(!Uart2SendFlag);
		    Uart2SendFlag = 0;
    }
	}
#endif
	
#if (USCI2_Mode == TWI2)
	TWI2_Init();
	while(1)
	{
		#if (TWI2_Mode == 0)  //������������
			{
				US2CON1 |= 0x20;    //������ʼ����	
				while(TWI2Flag == 0);
				TWI2Flag = 0; 
				US2CON3 = 0x02|0X01;	    //���͵�ַ��д����
				while(TWI2Flag == 0);
				TWI2Flag = 0; 

		/* ���ݽ��չ��� */
				do
				{
						if(RxXferCount2 ==7)
					{
							US2CON0 &= 0XF7;    	
					}
				
					 while(TWI2Flag == 0);
					 TWI2Flag = 0; 
					 TWI2_Master_ReceiveData[RxXferCount2] = US2CON3;             //TWI��������
					 RxXferCount2++;	
				}
				while(RxXferCount2 < 8);
			
			 RxXferCount2=0;
				/* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
			 US2CON0 |= 0X08; //AAʹ��λ,������UAC
			 US2CON1 |= 0x10;
			 Delay(500);
			}
	#endif
	#if (TWI2_Mode == 1)  //�ӻ�  
		 {  
			 
			 TWI2Flag = 0; 
			 while((0x07 & US2CON0)!=0x00);//�ж�״̬�Ƿ�ص�����״̬
			 
			  while((0x07 & US2CON0)!=0x03);//�ж�״̬�Ƿ���״̬
			  while(TWI2Flag == 0);
			 TWI2Flag = 0; 
			TxXferCount2=0;
			 while(TxXferCount2 < 8)
			 {

				US2CON3 = TWI2_Slaver_SendData[TxXferCount2];             //TWI��������
				while(TWI2Flag == 0);
				TWI2Flag = 0; 
				TxXferCount2++;

			 }
				US2CON3 = TWI2_Slaver_SendData[0];
			 
			 while((0x07 & US2CON0)!=0x00);//�ж�״̬�Ƿ�ص�����״̬

				TxXferCount2=0;
				Delay(100);
			 }
	#endif
	}
#endif

#if (USCI2_Mode == SPI2)
	SPI2_Init();
	while(1)
	{
     #if (SPI2_Mode == 0)  //����
	   TxXferCount2 = 0;
    while(TxXferCount2 <  8)//�ж��Ƿ������������
    {
     
      US2CON2 = SPI2_Master_SendData[TxXferCount2];
      while(SPI2Flag == 0);	//�ȴ��������
      SPI2Flag = 0;		
      SPI2_Master_ReceiveData[TxXferCount2]=US2CON2;
      TxXferCount2 ++;	//��������������
      
		}
		Delay(500);;
		

     #elif (SPI2_Mode == 1) //�ӻ�
 
 
	   while(RxXferCount2 <8)
    {
			 US2CON2=SPI2_Slaver_SendData[RxXferCount2];
      /* �ȴ�SPI�жϱ�־λ���� */
      
			while(SPI2Flag == 0);	//�ȴ��������
		   
      SPI2Flag = 0;		
      SPI2_Slaver_ReceiveData[RxXferCount2] = US2CON2;	
      /* �ȴ�SPI�жϱ�־λ���� */
      RxXferCount2++;	//����������������1
      
      }
		 Delay(100);
	  RxXferCount2 = 0;
		
     #endif
	}
#endif	
}
/*****************************************************
*�������ƣ�void Uart2_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart2�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart2_Init(uint Freq,unsigned long int baud)
{
	P1CON &= 0xCF;    //TX/RX����Ϊ���������
	P1PH  |= 0x30;
	
	TMCON |= 0xC0;     //���нӿ�USCI2ѡ��Uart2ͨ��
	US2CON0 = 0x50;    //����ͨ�ŷ�ʽΪģʽһ���������
	US2CON1 = Freq*1000000/baud;   //�����ʵ�λ����
	US2CON2 = (Freq*1000000/baud)>>8;    //�����ʸ�λ����
	IE2 |= 0x02;       //����USCI2�ж�
    EA = 1;	
}

/*****************************************************
*�������ƣ�void TWI2_Init(void)
*�������ܣ�TWI2��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWI2_Init(void)
{
	#if (TWI2_Mode == 0)	
 { US2CON3 = TWI2_Slaver_SendData[0];   }   //�ӻ�Ԥ��׼��������
#endif
	TMCON |= 0x80;   //ѡ��TWI2ģʽ
	US2CON0 = 0x88;  //��ģʽ�� ʹ��Ӧ���־λ
	US2CON1 = 0x40;  //---- xxxx  xΪʱ������
    US2CON2 = 0x02;  //�ӻ���ַ
	IE2 |= 0x02;
	EA = 1;
}

/*****************************************************
*�������ƣ�void SPI2_Init(void)
*�������ܣ�SPI2��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPI2_Init(void)
{
	TMCON |= 0X40;    //ѡ��SPI2ģʽ
#if (SPI2_Mode == 0)	
 {  US2CON0 = 0x23;   }   //����SPIXΪ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#elif (SPI2_Mode == 1)	
 {  US2CON0 = 0x03; }   //����SPIXΪ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#endif 
	US2CON1 = 0x00;   //MSB���ȷ��ͣ�8λ���䣬�������ж�
	US2CON0 |= 0x80;  //����SPI2
	IE2 |= 0x02;
	EA = 1;
}


/*****************************************************
*�������ƣ�void TWI2/PI/Uart2_Int() interrupt 7
*�������ܣ�USCI2�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (USCI2_Mode == Uart2)
void Uart2_Int() interrupt 16   //Uart2�жϺ���
{
	if(US2CON0&0x02)   //���ͱ�־λ�ж�
	{
		US2CON0 &= 0xFD;
		Uart2SendFlag = 1;
	}
	if((US2CON0&0x01))  //���ձ�־λ�ж�
	{
		US2CON0 &= 0xFE;
		Uart2ReceiveFlag = 1;
	}	
}
#endif

#if (USCI2_Mode == TWI2)
void TWI2_Int() interrupt 16    //TWI2�жϺ���
{
	if(US2CON0&0x40)
	{
		US2CON0 &= 0xbf;  //�ж�����
		TWI2Flag = 1;
	}	
}
#endif 

#if (USCI2_Mode == SPI2)
void SpiInt(void) interrupt 16    //SPI2�жϺ���
{	  
	if(US2CON1&0X08)     //���ͻ������ձ�־�ж�
	{
		US2CON1 &= ~0X08;
	}
	if(US2CON1&0X80)   //���ݴ����־λ�ж�
	{
		US2CON1 &= ~0X80;
		SPI2Flag = 1;
	}
}
#endif 
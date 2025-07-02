#include "H/Function_Init.H"

//ѡ������һģʽ��
#define  Uart0   0
#define  TWI0    1
#define  SPI0    2
#define  SmartCard    3

#define  USCI0_Mode  SPI0
#define TWI0_Mode 1//0:��������ģʽ   1���ӻ�����ģʽ
#define SPI0_Mode 1 //0:��������ģʽ   1���ӻ�����ģʽ

void Uart0_Init(uint Freq,unsigned long int baud);
void TWI0_Init(void);
void SPI0_Init(void);

bit Uart0SendFlag = 0;    //Uart0�����жϱ�־λ
bit Uart0ReceiveFlag = 0; //Uart0�����жϱ�־λ
bit SPI0Flag = 0;         //SPI0���ݴ�����ɱ�־λ
bit TWI0Flag = 0;         //TWI0�жϱ�־λ
uint8_t TxXferCount = 0;  //SPI/TWI����������Ŀ
uint8_t RxXferCount = 0;  //SPI/TWI����������Ŀ

volatile uint8_t Uart0_RxData[5];

uint8_t SPI0_Master_SendData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t SPI0_Slaver_ReceiveData[8];

uint8_t TWI0_Master_SendData[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t TWI0_Slaver_ReceiveData[8];
/*****************************************************
*�������ƣ�void USCI0_Test(void)
*�������ܣ�USCI0����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void USCI0_Test(void)
{
#if (USCI0_Mode == Uart0)
	int num1,num2;
	Uart0_Init(32,9600);
	while(1)
	{
		for(num1=0;num1<5;num1++)
		{
			while(!Uart0ReceiveFlag);
			Uart0ReceiveFlag = 0;
			Uart0_RxData[num1] = US0CON3 ;
		}
		for(num2=0;num2<5;num2++)
		{
			US0CON3  = Uart0_RxData[num2];
	  	while(!Uart0SendFlag);
      Uart0SendFlag = 0;
		}
#endif
	
#if (USCI0_Mode == TWI0)
  TWI0_Init();
  while(1)
  {
#if (TWI0_Mode == 0)  //����
      US0CON1 |= 0x20;    //������ʼ����
		  Delay(100);
      while(TWI0Flag == 0);
      TWI0Flag = 0; 
      US0CON3 = 0x02;	    //���͵�ַ��д����
      while(TWI0Flag == 0);
      TWI0Flag = 0; 
      while(TxXferCount < 8)
			{
				US0CON3 = TWI0_Master_SendData[TxXferCount];             //TWI��������
				while(TWI0Flag == 0);
				TWI0Flag = 0; 
			  TxXferCount++;
			}
     TxXferCount=0;

    /* �����Ƿ�����ɣ�����Ҫ���ͽ����źţ���ֹ����ռ������ */
      US0CON1 |= 0x10;
	
			Delay(1000);
#endif   
#if (TWI0_Mode == 1)  //�ӻ�  

			while((0x07 & US0CON0)!=0x02);//�ж�Ϊ����״̬
			Delay(100);
			while(TWI0Flag == 0);
			TWI0Flag = 0;                 
     	Delay(100);
    /* �������ݹ��� */
			do
			{
				if(RxXferCount == 7) US0CON0 &= 0XF7;   //AAʹ��λ,������UAC
				while(TWI0Flag == 0);
				TWI0Flag = 0;  
				TWI0_Slaver_ReceiveData[RxXferCount] = US0CON3;             //TWI��������
				RxXferCount++; 
			}  while(RxXferCount < 8);

			US0CON0 |= 0X08; //AAʹ��λ
			RxXferCount =0;
			while((0x07 & US0CON0)!=0x00);
		 
		
#endif
  }
#endif

#if (USCI0_Mode == SPI0)
   SPI0_Init();
   
   while(1)
   { 
#if (SPI0_Mode == 0)  //����
	    TxXferCount = 0;
			while(TxXferCount <  8)//�ж��Ƿ������������
			{
			 
				US0CON2 = SPI0_Master_SendData[TxXferCount];
				while(SPI0Flag == 0);	//�ȴ��������		   
				SPI0Flag = 0;		
				TxXferCount ++;	//��������������
			
			}
	   	Delay(1000);
		

#elif (SPI0_Mode == 1) //�ӻ�
   
 
			 while(RxXferCount <8)
				{
					/* �ȴ�SPI�жϱ�־λ���� */
					while(SPI0Flag == 0);	//�ȴ��������
					SPI0Flag = 0;		
					SPI0_Slaver_ReceiveData[RxXferCount] = US0CON2;			//��ȡ16λ����,���ҵ�ַ����
					RxXferCount++;	//����������������1       
				}
				RxXferCount = 0;
			  Delay(100);
#endif
  }
#endif
}
/*****************************************************
*�������ƣ�void Uart0_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart0�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)
{
	P2CON &= 0xFC;    //TX/RX����Ϊ���������
	P2PH  |= 0x03;
	
	OTCON |= 0x30;    //���нӿ�USCI0ѡ��Uart0ͨ��
	US0CON0 = 0x50;   //����ͨ�ŷ�ʽΪģʽһ���������
	US0CON1 = Freq*1000000/baud;   //�����ʵ�λ����
	US0CON2 = (Freq*1000000/baud)>>8;   //�����ʸ�λ����
  IE1 |= 0x01;      //����USCI0�ж�
  EA = 1;
}

/*****************************************************
*�������ƣ�void TWI0_Init(void)
*�������ܣ�TWI0��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWI0_Init(void)
{
	OTCON |= 0x20;   //ѡ��TWI0ģʽ
  US0CON0 = 0x88;  // ʹ��Ӧ���־λ
  US0CON1 = 0x40;  //---- xxxx  xΪʱ������
  US0CON2 = 0x02;  //�ӻ���ַ0x01
	IE1 |= 0x01;
	EA = 1;
}

/*****************************************************
*�������ƣ�void SPI0_Init(void)
*�������ܣ�SPI0��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPI0_Init(void)
{
	OTCON |= 0X10;    //ѡ��SPI0ģʽ
#if (SPI0_Mode == 0)	
 {  US0CON0 = 0x23;   }   //����SPI0Ϊ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#elif (SPI0_Mode == 1)	
 {  US0CON0 = 0x03;   }   //����SPI0Ϊ���豸��SCK����ʱ��Ϊ�͵�ƽ��SCK���ڵ�һ�زɼ����ݣ�ʱ������ΪFsy/8
#endif 
	US0CON1 = 0x00;   //MSB���ȷ��ͣ�8λ����
	US0CON0 |= 0x80;  //����SPI0
	IE1 |= 0x01;
	EA = 1;
}


/*****************************************************
*�������ƣ�void TWI0/PI/UART1_Int() interrupt 7
*�������ܣ�USCI0�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (USCI0_Mode == Uart0)
void Uart0_Int() interrupt 7   //Uart0�жϺ���
{
	if(US0CON0&0x02)    //���ͱ�־λ�ж�
	{
		US0CON0 &= 0xFD;
		Uart0SendFlag = 1;
	}
	if((US0CON0&0x01))  //���ձ�־λ�ж�
	{
		US0CON0 &= 0xFE;
		Uart0ReceiveFlag = 1;
	}	
}
#elif (USCI0_Mode == TWI0)
void TWI0_Int() interrupt 7     //TWI0�жϺ���
{
	if(US0CON0&0x40)
	{
		US0CON0 &= 0xbf;  //�ж�����
		TWI0Flag = 1;
	}	
}
#elif (USCI0_Mode == SPI0)
void SpiInt1(void) interrupt 7    //SPI0�жϺ���
{	  
	if(US0CON1&0X08)    //���ͻ������ձ�־�ж�
	{
		US0CON1 &= ~0X08;
	}
	if(US0CON1&0X80)    //���ݴ����־λ�ж�
	{
		US0CON1 &= ~0X80;
		SPI0Flag = 1;
	}
}
#else
extern void SC_Interrupt();
void USCI0Int(void) interrupt 7    //SPI0�жϺ���
{	  
	SC_Interrupt();
}
#endif 
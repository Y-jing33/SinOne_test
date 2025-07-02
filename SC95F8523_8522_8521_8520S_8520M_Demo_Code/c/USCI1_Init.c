#include "H/Function_Init.H"

//选择三合一模式：
#define  Uart1   0
#define  TWI1    1
#define  SPI1    2

#define  USCI1_Mode  SPI1

#define TWI1_Mode 1//0:主机工作模式   1：从机工作模式
#define SPI1_Mode 1 //0:主机工作模式   1：从机工作模式

void Uart1_Init(uint Freq, unsigned long int baud);
void TWI1_Init(void);
void SPI1_Init(void);

bit Uart1SendFlag = 0;    //Uart1发送中断标志位
bit Uart1ReceiveFlag = 0; //Uart1接收中断标志位
bit SPI1Flag = 0;         //SPI1数据传输完成标志位
bit TWI1Flag = 0;         //TWI1中断标志位
uint8_t TxXferCount1 = 0;  //SPI/TWI发送数据数目
uint8_t RxXferCount1 = 0;  //SPI/TWI接受数据数目


volatile uint8_t Uart1_RxData[5];

uint8_t SPI1_Master_SendData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t SPI1_Slaver_ReceiveData[8];

uint8_t TWI1_Master_SendData[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t TWI1_Slaver_ReceiveData[8];
/*****************************************************
*函数名称：void USCI1_Test(void)
*函数功能：USCI1测试
*入口参数：void
*出口参数：void
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
#if (TWI1_Mode == 0)  //主机
		  US1CON1 |= 0x20;    //产生起始条件
		  Delay(100);
      while(TWI1Flag == 0);
      TWI1Flag = 0; 
      US1CON3 = 0x02;	    //发送地址及写命令
      while(TWI1Flag == 0);
      TWI1Flag = 0; 
      while(TxXferCount1 < 8)
			{
				US1CON3 = TWI1_Master_SendData[TxXferCount1];             //TWI发送数据
				while(TWI1Flag == 0);
				TWI1Flag = 0; 
			  TxXferCount1++;
			}
     TxXferCount1=0;

    /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      US1CON1 |= 0x10;
	
			Delay(1000);
#endif   
#if (TWI1_Mode == 1)  //从机  

      while((0x07 & US1CON0)!=0x02);//判断为接收状态
			Delay(100);
			while(TWI1Flag == 0);
			TWI1Flag = 0;                 
     	Delay(100);
    /* 接收数据过程 */
			do
			{
				if(RxXferCount1 == 7) US1CON0 &= 0XF7;   //AA使能位,回主机UAC
				while(TWI1Flag == 0);
				TWI1Flag = 0;  
				TWI1_Slaver_ReceiveData[RxXferCount1] = US1CON3;             //TWI接收数据
				RxXferCount1++; 
			}  while(RxXferCount1 < 8);

			US1CON0 |= 0X08; //AA使能位
			RxXferCount1 =0;
			while((0x07 & US1CON0)!=0x00);
  
		
#endif
		}
#endif
	
#if (USCI1_Mode == SPI1)
  SPI1_Init();
	
			while(1)
			{
#if (SPI1_Mode == 0)  //主机
				TxXferCount1 = 0;
				while(TxXferCount1 <  8)//判断是否接收所有数据
				{
				 
					US1CON2 = SPI1_Master_SendData[TxXferCount1];
					while(SPI1Flag == 0);	//等待发送完成		   
					SPI1Flag = 0;		
					TxXferCount1++;	//发送数据量计数
				}
				Delay(1000);
#endif
#if (SPI1_Mode == 1)  //从机
			while(RxXferCount1 <8)
				{
					/* 等待SPI中断标志位置起 */
					while(SPI1Flag == 0);	//等待发送完成
					SPI1Flag = 0;		
					SPI1_Slaver_ReceiveData[RxXferCount1] = US1CON2;			//读取数据
					RxXferCount1++;	//接收数据量计数加1       
				}
				RxXferCount1 = 0;
				Delay(100);

#endif
		
 
  }
#endif
}
/*****************************************************
*函数名称：void Uart1_Init(uint Freq,unsigned long int baud)
*函数功能：Uart1中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart1_Init(uint Freq,unsigned long int baud)
{
	P0CON &= 0xF3;    //TX/RX设置为输入带上拉
	P0PH  |= 0x0C;
	
	OTCON |= 0xC0;    //串行接口USCI1选择Uart1通信
	US1CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US1CON1 = Freq*1000000/baud;   //波特率低位控制
	US1CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE2 |= 0x01;      //开启USCI1中断
    EA = 1;	
}

/*****************************************************
*函数名称：void TWI1_Init(void)
*函数功能：TWI1初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWI1_Init(void)
{
  OTCON |= 0x80;   //选择TWI1模式
  US1CON0 = 0x88;  //主模式， 使能应答标志位
  US1CON1 = 0x40;  //---- xxxx  x为时钟速率
  US1CON2 = 0x02;  //从机地址
  IE2 |= 0x01;
  EA = 1;
}

/*****************************************************
*函数名称：void SPI1_Init(void)
*函数功能：SPI1初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPI1_Init(void)
{
	OTCON |= 0X40;    //选择SPI1模式
#if (SPI1_Mode == 0)	
 {  US1CON0 = 0x23;   }   //设置SPI0为主设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#elif (SPI1_Mode == 1)	
 {  US1CON0 = 0x03;   }   //设置SPI0为从设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#endif 
	US1CON1 = 0x00;   //MSB优先发送，8位传输，不允许发送中断
	US1CON0 |= 0x80;  //开启SPI1
	IE2 |= 0x01;
	EA = 1;
}


/*****************************************************
*函数名称：void TWI1/PI/UART1_Int() interrupt 7
*函数功能：USCI1中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (USCI1_Mode == Uart1)
void Uart1_Int() interrupt 15   //Uart1中断函数
{
	if(US1CON0&0x02)    //发送标志位判断
	{
		US1CON0 &= 0xFD;
		Uart1SendFlag = 1;
	}
	if((US1CON0&0x01))  //接收标志位判断
	{
		US1CON0 &= 0xFE;
		Uart1ReceiveFlag = 1;
	}	
}
#endif

#if (USCI1_Mode == TWI1)
void TWI1_Int() interrupt 15     //TWI1中断函数
{
	if(US1CON0&0x40)
	{
		US1CON0 &= 0xbf;  //中断清零
		TWI1Flag = 1;
	}	
}
#endif 

#if (USCI1_Mode == SPI1)
void SpiInt(void) interrupt 15    //SPI1中断函数
{	  
	if(US1CON1&0X08)    //发送缓存器空标志判断
	{
		US1CON1 &= ~0X08;
	}
	if(US1CON1&0X80)    //数据传输标志位判断
	{
		US1CON1 &= ~0X80;
		SPI1Flag = 1;
	}
}
#endif 
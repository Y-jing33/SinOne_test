#include "H/Function_Init.H"

//选择三合一模式：
#define  Uart2   0
#define  TWI2    1
#define  SPI2    2

#define  USCI2_Mode  TWI2
#define TWI2_Mode 0//0:主机工作模式   1：从机工作模式
#define SPI2_Mode 0 //0:主机工作模式   1：从机工作模式

void Uart2_Init(uint Freq,unsigned long int baud);
void TWI2_Init(void);
void SPI2_Init(void);

bit Uart2SendFlag = 0;     //Uart2发送中断标志位
bit Uart2ReceiveFlag = 0;  //Uart2接收中断标志位
bit SPI2Flag = 0;         //SPI2数据传输完成标志位
bit TWI2Flag = 0;         //TWI2中断标志位

uint8_t TxXferCount2 = 0;  //SPI/TWI发送数据数目
uint8_t RxXferCount2 = 0;  //SPI/TWI接受数据数目 
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
*函数名称：void USCI2_Test(void)
*函数功能：USCI2测试
*入口参数：void
*出口参数：void
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
		#if (TWI2_Mode == 0)  //主机接收数据
			{
				US2CON1 |= 0x20;    //产生起始条件	
				while(TWI2Flag == 0);
				TWI2Flag = 0; 
				US2CON3 = 0x02|0X01;	    //发送地址及写命令
				while(TWI2Flag == 0);
				TWI2Flag = 0; 

		/* 数据接收过程 */
				do
				{
						if(RxXferCount2 ==7)
					{
							US2CON0 &= 0XF7;    	
					}
				
					 while(TWI2Flag == 0);
					 TWI2Flag = 0; 
					 TWI2_Master_ReceiveData[RxXferCount2] = US2CON3;             //TWI接收数据
					 RxXferCount2++;	
				}
				while(RxXferCount2 < 8);
			
			 RxXferCount2=0;
				/* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
			 US2CON0 |= 0X08; //AA使能位,回主机UAC
			 US2CON1 |= 0x10;
			 Delay(500);
			}
	#endif
	#if (TWI2_Mode == 1)  //从机  
		 {  
			 
			 TWI2Flag = 0; 
			 while((0x07 & US2CON0)!=0x00);//判断状态是否回到空闲状态
			 
			  while((0x07 & US2CON0)!=0x03);//判断状态是否发送状态
			  while(TWI2Flag == 0);
			 TWI2Flag = 0; 
			TxXferCount2=0;
			 while(TxXferCount2 < 8)
			 {

				US2CON3 = TWI2_Slaver_SendData[TxXferCount2];             //TWI发送数据
				while(TWI2Flag == 0);
				TWI2Flag = 0; 
				TxXferCount2++;

			 }
				US2CON3 = TWI2_Slaver_SendData[0];
			 
			 while((0x07 & US2CON0)!=0x00);//判断状态是否回到空闲状态

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
     #if (SPI2_Mode == 0)  //主机
	   TxXferCount2 = 0;
    while(TxXferCount2 <  8)//判断是否接收所有数据
    {
     
      US2CON2 = SPI2_Master_SendData[TxXferCount2];
      while(SPI2Flag == 0);	//等待发送完成
      SPI2Flag = 0;		
      SPI2_Master_ReceiveData[TxXferCount2]=US2CON2;
      TxXferCount2 ++;	//发送数据量计数
      
		}
		Delay(500);;
		

     #elif (SPI2_Mode == 1) //从机
 
 
	   while(RxXferCount2 <8)
    {
			 US2CON2=SPI2_Slaver_SendData[RxXferCount2];
      /* 等待SPI中断标志位置起 */
      
			while(SPI2Flag == 0);	//等待发送完成
		   
      SPI2Flag = 0;		
      SPI2_Slaver_ReceiveData[RxXferCount2] = US2CON2;	
      /* 等待SPI中断标志位置起 */
      RxXferCount2++;	//接收数据量计数加1
      
      }
		 Delay(100);
	  RxXferCount2 = 0;
		
     #endif
	}
#endif	
}
/*****************************************************
*函数名称：void Uart2_Init(uint Freq,unsigned long int baud)
*函数功能：Uart2中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart2_Init(uint Freq,unsigned long int baud)
{
	P1CON &= 0xCF;    //TX/RX设置为输入带上拉
	P1PH  |= 0x30;
	
	TMCON |= 0xC0;     //串行接口USCI2选择Uart2通信
	US2CON0 = 0x50;    //设置通信方式为模式一，允许接收
	US2CON1 = Freq*1000000/baud;   //波特率低位控制
	US2CON2 = (Freq*1000000/baud)>>8;    //波特率高位控制
	IE2 |= 0x02;       //开启USCI2中断
    EA = 1;	
}

/*****************************************************
*函数名称：void TWI2_Init(void)
*函数功能：TWI2初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWI2_Init(void)
{
	#if (TWI2_Mode == 0)	
 { US2CON3 = TWI2_Slaver_SendData[0];   }   //从机预先准备好数据
#endif
	TMCON |= 0x80;   //选择TWI2模式
	US2CON0 = 0x88;  //主模式， 使能应答标志位
	US2CON1 = 0x40;  //---- xxxx  x为时钟速率
    US2CON2 = 0x02;  //从机地址
	IE2 |= 0x02;
	EA = 1;
}

/*****************************************************
*函数名称：void SPI2_Init(void)
*函数功能：SPI2初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPI2_Init(void)
{
	TMCON |= 0X40;    //选择SPI2模式
#if (SPI2_Mode == 0)	
 {  US2CON0 = 0x23;   }   //设置SPIX为主设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#elif (SPI2_Mode == 1)	
 {  US2CON0 = 0x03; }   //设置SPIX为从设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#endif 
	US2CON1 = 0x00;   //MSB优先发送，8位传输，允许发送中断
	US2CON0 |= 0x80;  //开启SPI2
	IE2 |= 0x02;
	EA = 1;
}


/*****************************************************
*函数名称：void TWI2/PI/Uart2_Int() interrupt 7
*函数功能：USCI2中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (USCI2_Mode == Uart2)
void Uart2_Int() interrupt 16   //Uart2中断函数
{
	if(US2CON0&0x02)   //发送标志位判断
	{
		US2CON0 &= 0xFD;
		Uart2SendFlag = 1;
	}
	if((US2CON0&0x01))  //接收标志位判断
	{
		US2CON0 &= 0xFE;
		Uart2ReceiveFlag = 1;
	}	
}
#endif

#if (USCI2_Mode == TWI2)
void TWI2_Int() interrupt 16    //TWI2中断函数
{
	if(US2CON0&0x40)
	{
		US2CON0 &= 0xbf;  //中断清零
		TWI2Flag = 1;
	}	
}
#endif 

#if (USCI2_Mode == SPI2)
void SpiInt(void) interrupt 16    //SPI2中断函数
{	  
	if(US2CON1&0X08)     //发送缓存器空标志判断
	{
		US2CON1 &= ~0X08;
	}
	if(US2CON1&0X80)   //数据传输标志位判断
	{
		US2CON1 &= ~0X80;
		SPI2Flag = 1;
	}
}
#endif 
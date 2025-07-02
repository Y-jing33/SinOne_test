#include "H/Function_Init.H"

//选择三合一模式：
#define  Uart0   0
#define  TWI0    1
#define  SPI0    2
#define  SmartCard    3

#define  USCI0_Mode  SPI0
#define TWI0_Mode 1//0:主机工作模式   1：从机工作模式
#define SPI0_Mode 1 //0:主机工作模式   1：从机工作模式

void Uart0_Init(uint Freq,unsigned long int baud);
void TWI0_Init(void);
void SPI0_Init(void);

bit Uart0SendFlag = 0;    //Uart0发送中断标志位
bit Uart0ReceiveFlag = 0; //Uart0接收中断标志位
bit SPI0Flag = 0;         //SPI0数据传输完成标志位
bit TWI0Flag = 0;         //TWI0中断标志位
uint8_t TxXferCount = 0;  //SPI/TWI发送数据数目
uint8_t RxXferCount = 0;  //SPI/TWI接受数据数目

volatile uint8_t Uart0_RxData[5];

uint8_t SPI0_Master_SendData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t SPI0_Slaver_ReceiveData[8];

uint8_t TWI0_Master_SendData[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile uint8_t TWI0_Slaver_ReceiveData[8];
/*****************************************************
*函数名称：void USCI0_Test(void)
*函数功能：USCI0测试
*入口参数：void
*出口参数：void
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
#if (TWI0_Mode == 0)  //主机
      US0CON1 |= 0x20;    //产生起始条件
		  Delay(100);
      while(TWI0Flag == 0);
      TWI0Flag = 0; 
      US0CON3 = 0x02;	    //发送地址及写命令
      while(TWI0Flag == 0);
      TWI0Flag = 0; 
      while(TxXferCount < 8)
			{
				US0CON3 = TWI0_Master_SendData[TxXferCount];             //TWI发送数据
				while(TWI0Flag == 0);
				TWI0Flag = 0; 
			  TxXferCount++;
			}
     TxXferCount=0;

    /* 无论是否发送完成，都需要发送结束信号，防止主机占用总线 */
      US0CON1 |= 0x10;
	
			Delay(1000);
#endif   
#if (TWI0_Mode == 1)  //从机  

			while((0x07 & US0CON0)!=0x02);//判断为接收状态
			Delay(100);
			while(TWI0Flag == 0);
			TWI0Flag = 0;                 
     	Delay(100);
    /* 接收数据过程 */
			do
			{
				if(RxXferCount == 7) US0CON0 &= 0XF7;   //AA使能位,回主机UAC
				while(TWI0Flag == 0);
				TWI0Flag = 0;  
				TWI0_Slaver_ReceiveData[RxXferCount] = US0CON3;             //TWI接收数据
				RxXferCount++; 
			}  while(RxXferCount < 8);

			US0CON0 |= 0X08; //AA使能位
			RxXferCount =0;
			while((0x07 & US0CON0)!=0x00);
		 
		
#endif
  }
#endif

#if (USCI0_Mode == SPI0)
   SPI0_Init();
   
   while(1)
   { 
#if (SPI0_Mode == 0)  //主机
	    TxXferCount = 0;
			while(TxXferCount <  8)//判断是否接收所有数据
			{
			 
				US0CON2 = SPI0_Master_SendData[TxXferCount];
				while(SPI0Flag == 0);	//等待发送完成		   
				SPI0Flag = 0;		
				TxXferCount ++;	//发送数据量计数
			
			}
	   	Delay(1000);
		

#elif (SPI0_Mode == 1) //从机
   
 
			 while(RxXferCount <8)
				{
					/* 等待SPI中断标志位置起 */
					while(SPI0Flag == 0);	//等待发送完成
					SPI0Flag = 0;		
					SPI0_Slaver_ReceiveData[RxXferCount] = US0CON2;			//读取16位数据,并且地址自增
					RxXferCount++;	//接收数据量计数加1       
				}
				RxXferCount = 0;
			  Delay(100);
#endif
  }
#endif
}
/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)
{
	P2CON &= 0xFC;    //TX/RX设置为输入带上拉
	P2PH  |= 0x03;
	
	OTCON |= 0x30;    //串行接口USCI0选择Uart0通信
	US0CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US0CON1 = Freq*1000000/baud;   //波特率低位控制
	US0CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
  IE1 |= 0x01;      //开启USCI0中断
  EA = 1;
}

/*****************************************************
*函数名称：void TWI0_Init(void)
*函数功能：TWI0初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWI0_Init(void)
{
	OTCON |= 0x20;   //选择TWI0模式
  US0CON0 = 0x88;  // 使能应答标志位
  US0CON1 = 0x40;  //---- xxxx  x为时钟速率
  US0CON2 = 0x02;  //从机地址0x01
	IE1 |= 0x01;
	EA = 1;
}

/*****************************************************
*函数名称：void SPI0_Init(void)
*函数功能：SPI0初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPI0_Init(void)
{
	OTCON |= 0X10;    //选择SPI0模式
#if (SPI0_Mode == 0)	
 {  US0CON0 = 0x23;   }   //设置SPI0为主设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#elif (SPI0_Mode == 1)	
 {  US0CON0 = 0x03;   }   //设置SPI0为从设备，SCK空闲时间为低电平，SCK周期第一沿采集数据，时钟速率为Fsy/8
#endif 
	US0CON1 = 0x00;   //MSB优先发送，8位传输
	US0CON0 |= 0x80;  //开启SPI0
	IE1 |= 0x01;
	EA = 1;
}


/*****************************************************
*函数名称：void TWI0/PI/UART1_Int() interrupt 7
*函数功能：USCI0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (USCI0_Mode == Uart0)
void Uart0_Int() interrupt 7   //Uart0中断函数
{
	if(US0CON0&0x02)    //发送标志位判断
	{
		US0CON0 &= 0xFD;
		Uart0SendFlag = 1;
	}
	if((US0CON0&0x01))  //接收标志位判断
	{
		US0CON0 &= 0xFE;
		Uart0ReceiveFlag = 1;
	}	
}
#elif (USCI0_Mode == TWI0)
void TWI0_Int() interrupt 7     //TWI0中断函数
{
	if(US0CON0&0x40)
	{
		US0CON0 &= 0xbf;  //中断清零
		TWI0Flag = 1;
	}	
}
#elif (USCI0_Mode == SPI0)
void SpiInt1(void) interrupt 7    //SPI0中断函数
{	  
	if(US0CON1&0X08)    //发送缓存器空标志判断
	{
		US0CON1 &= ~0X08;
	}
	if(US0CON1&0X80)    //数据传输标志位判断
	{
		US0CON1 &= ~0X80;
		SPI0Flag = 1;
	}
}
#else
extern void SC_Interrupt();
void USCI0Int(void) interrupt 7    //SPI0中断函数
{	  
	SC_Interrupt();
}
#endif 
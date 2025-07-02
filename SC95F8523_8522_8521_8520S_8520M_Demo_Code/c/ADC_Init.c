#include "H/Function_Init.H"

void ADC_Init(uint Channel);
unsigned int ADCValue = 0x0000;
unsigned int ADC_AverageValue(void);
bit AdcFlag = 0;

/***********ADC采样口选择*************/
#if (IC_MODEL == SC95F8523)
	enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=31};
#endif

#if (IC_MODEL == SC95F8522)
enum Channel {AIN2=2,AIN3,AIN4,AIN5,AIN8=8,AIN9,VDD4=31};
#endif

#if (IC_MODEL == SC95F8521)
enum Channel {AIN2=2,AIN3,AIN4,AIN8=8,AIN9,VDD4=31};
#endif

#if (IC_MODEL == SC95F8520S)
enum Channel {AIN2=2,AIN8=8,VDD4=31};
#endif

#if (IC_MODEL == SC95F8520M)
enum Channel {AIN2=2,VDD4=31};
#endif

/*****************************************************
*函数名称：void ADC_Test(void)
*函数功能：ADC测试
*入口参数：void
*出口参数：void
*****************************************************/
void ADC_Test(void)
{
	ADC_Init(AIN2);
	while(1)
	{
		 ADCValue = ADC_AverageValue();	
	}
}
/*****************************************************
*函数名称：void ADC_Init(uint Channel)
*函数功能：ADC初始化
*入口参数：Channel
*出口参数：void
*****************************************************/
void ADC_Init(uint Channel)
{
	ADCCON = 0X80|Channel;		//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   //设置Channel作为采样口
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
	}
	ADCCFG2 = 0x10;			//采样时间为三个系统时钟
	IE |= 0X40;         //开启ADC中断
	EA = 1;
}


unsigned int ADC_AverageValue()
{
	int i = 0; 
	unsigned int ADC_ValueSum = 0, ADC_ValueMax = 0 ,ADC_ValueMin = 0x01 << 12 ,ADC_ValueMean = 0 ,ADC_ValueTad = 0;
	for(i=0;i<10;i++ )
	{	
		ADCCON |= 0X40;   //开始ADC转换
	  while(!AdcFlag);	         		 //等待 ADC转换完成;
		AdcFlag = 0;
		ADC_ValueTad = (ADCVH<<4)+(ADCVL>>4);
		if(ADC_ValueTad>ADC_ValueMax)
		{
		 ADC_ValueMax = ADC_ValueTad;
		}
		if(ADC_ValueTad<ADC_ValueMin)
		{
		 ADC_ValueMin = ADC_ValueTad;
		}
		ADC_ValueSum += ADC_ValueTad;
	}
	ADC_ValueMean = (unsigned int)((ADC_ValueSum - ADC_ValueMax - ADC_ValueMin) / 8);
  return ADC_ValueMean;
}



void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //清中断标志位
	AdcFlag = 1;
}
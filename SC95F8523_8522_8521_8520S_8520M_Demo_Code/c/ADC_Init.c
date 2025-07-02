#include "H/Function_Init.H"

void ADC_Init(uint Channel);
unsigned int ADCValue = 0x0000;
unsigned int ADC_AverageValue(void);
bit AdcFlag = 0;

/***********ADC������ѡ��*************/
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
*�������ƣ�void ADC_Test(void)
*�������ܣ�ADC����
*��ڲ�����void
*���ڲ�����void
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
*�������ƣ�void ADC_Init(uint Channel)
*�������ܣ�ADC��ʼ��
*��ڲ�����Channel
*���ڲ�����void
*****************************************************/
void ADC_Init(uint Channel)
{
	ADCCON = 0X80|Channel;		//����ADC��ADC����Ƶ��Ϊ2M ,ѡ��ChannelλADC������
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   //����Channel��Ϊ������
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //����Channel��Ϊ������
	}
	ADCCFG2 = 0x10;			//����ʱ��Ϊ����ϵͳʱ��
	IE |= 0X40;         //����ADC�ж�
	EA = 1;
}


unsigned int ADC_AverageValue()
{
	int i = 0; 
	unsigned int ADC_ValueSum = 0, ADC_ValueMax = 0 ,ADC_ValueMin = 0x01 << 12 ,ADC_ValueMean = 0 ,ADC_ValueTad = 0;
	for(i=0;i<10;i++ )
	{	
		ADCCON |= 0X40;   //��ʼADCת��
	  while(!AdcFlag);	         		 //�ȴ� ADCת�����;
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
	ADCCON &= ~(0X20);  //���жϱ�־λ
	AdcFlag = 1;
}
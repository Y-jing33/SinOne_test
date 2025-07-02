#include "H/Function_Init.H"

u32 product = 0;//乘积
u32 quotient = 0;//商
u16 remainder = 0;//余数
Result_union result;

void Multiplier_Divider_Test(void)
{
  while(1)
  {
    //乘除法器
    Multiplication(0x55AA, 0xAA55);  //乘法计算
    if(product == 0x38ff5572)   //判断积
    {
      P04 = 0;
    }

    Division(0xFFAA5500, 0xAA55);   //除法计算
    if(quotient == 0x18040 && remainder == 0x3FC0)  //判断商和余数
    {
      P04 = 1;
    }
  }
}

/****************************************************
*函数名称：Multiplication(u16 faciend, u16 Multiplier)
*函数功能：乘法运算
*入口参数：faciend，Multiplier
*出口参数：无
****************************************************/
void Multiplication(u16 faciend, u16 Multiplier)
{
    OPERCON &= ~0x40;   //选择乘法
    
	EXA0 = faciend;
	EXA1 = faciend>>8;
	EXBL = Multiplier;
	EXBH = Multiplier>>8;

	OPERCON |= 0x80;      //开始计算
	while(OPERCON & 0x80);

	result.reg.a0 = EXA0;
	result.reg.a1 = EXA1;
	result.reg.a2 = EXA2;
	result.reg.a3 = EXA3;

	product = result.Result;
}	

/****************************************************
*函数名称：Division(u32 dividend,u16 divisor)
*函数功能：除法运算
*入口参数：dividend，divisor
*出口参数：无
****************************************************/
void Division(u32 dividend,u16 divisor)
{
	Result_union temp;
	temp.Result = dividend;

    OPERCON &= ~0x40;
    OPERCON |= 0x40;    //选择除法
    
	EXA0 = temp.reg.a0;
	EXA1 = temp.reg.a1;
	EXA2 = temp.reg.a2;
	EXA3 = temp.reg.a3;

	EXBL = divisor;
	EXBH = divisor>>8;

	OPERCON |= 0xC0;
	while(OPERCON & 0x80);

	result.reg.a0 = EXA0;
	result.reg.a1 = EXA1;
	result.reg.a2 = EXA2;
	result.reg.a3 = EXA3;

	remainder = EXBH*256+ EXBL;
	quotient  = result.Result;
}


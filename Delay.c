#include <REGX52.H>
#include <INTRINS.H>

// 延时函数 (毫秒级，适用于11.0592MHz晶振)
void Delay_ms(unsigned int xms)	//@11.0592MHz
{
	unsigned char data i, j;
	
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
// 延时函数 (微秒级)
void Delay_us(unsigned int xus) {
    unsigned int i;
    // 1次循环约等于1us
    for(i = 0; i < xus; i++);
}
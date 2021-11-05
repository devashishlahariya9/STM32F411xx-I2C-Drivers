#include "STM32F411xx_I2C1.h"

int main(void)
{
	I2C1_init(I2C_SM);

	I2C1_writeByte(0x68, 0x02);

	while(1)
	{

	}
	return 0;
}

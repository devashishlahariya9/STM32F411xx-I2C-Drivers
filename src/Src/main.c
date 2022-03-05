#include "STM32F411xx_I2C.h"

#define SLAVE_ADDR 0x68

int main(void)
{
	I2C_Init(I2C1, I2C1_USE_GPIOB8_9, I2C_FAST_MODE);

	I2C_WriteByte(I2C1, SLAVE_ADDR, 0x02);

	while(1)
	{

	}
	return 0;
}

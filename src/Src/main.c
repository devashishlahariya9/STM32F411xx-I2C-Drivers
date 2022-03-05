#include <string.h>
#include "STM32F411xx_I2C.h"

int main(void)
{
	uint8_t buffer[10];

	memset(buffer, 0xAA, sizeof(buffer));

	I2C_Init(I2C1, I2C1_USE_GPIOB8_9, I2C_FAST_MODE);

	I2C_WriteByte(I2C1, 0x68, 0x02);

	while(1)
	{

	}
	return 0;
}

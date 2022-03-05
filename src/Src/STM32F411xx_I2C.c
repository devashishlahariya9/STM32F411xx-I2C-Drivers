/*
 * STM32F411xx_I2C1.c
 *
 * Created on: 04-Nov-2021
 * Author: Devashish Lahariya
*/

#include <STM32F411xx_I2C.h>

void I2C1_USE_GPIOB6_7(void)
{
	GPIOB_CLK_ENABLE();
	I2C1_CLK_ENABLE();

	GPIOB -> AFRL &= ~(0xF << 24);
	GPIOB -> AFRL &= ~(0xF << 28);
	GPIOB -> AFRL |=  (AF4 << 24) | (AF4 << 28);

	GPIO_SetMode(GPIOB, I2C1_GPIOB6_SCL, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
	GPIO_SetMode(GPIOB, I2C1_GPIOB7_SDA, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
}

void I2C1_USE_GPIOB8_9(void)
{
	GPIOB_CLK_ENABLE();
	I2C1_CLK_ENABLE();

	GPIOB -> AFRH &= ~(0xF << 0);
	GPIOB -> AFRH &= ~(0xF << 4);
	GPIOB -> AFRH |=  (AF4 << 0) | (AF4 << 4);

	GPIO_SetMode(GPIOB, I2C1_GPIOB8_SCL, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
	GPIO_SetMode(GPIOB, I2C1_GPIOB9_SDA, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
}

void I2C2_USE_GPIOB10_11(void)
{
	GPIOB_CLK_ENABLE();
	I2C2_CLK_ENABLE();

	GPIOB -> AFRH &= ~(0xF << 8);
	GPIOB -> AFRH &= ~(0xF << 12);
	GPIOB -> AFRH |=  (AF4 << 8) | (AF4 << 12);

	GPIO_SetMode(GPIOB, I2C2_GPIOB10_SCL, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
	GPIO_SetMode(GPIOB, I2C2_GPIOB11_SDA, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
}

void I2C3_USE_GPIOA8_C9(void)
{
	GPIOA_CLK_ENABLE();
	GPIOC_CLK_ENABLE();
	I2C3_CLK_ENABLE();

	GPIOA -> AFRH &= ~(0xF << 0);
	GPIOC -> AFRH &= ~(0xF << 4);
	GPIOA -> AFRH |=  (AF4 << 0);
	GPIOC -> AFRH |=  (AF4 << 4);

	GPIO_SetMode(GPIOA, I2C3_GPIOA8_SCL, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
	GPIO_SetMode(GPIOC, I2C3_GPIOC9_SDA, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
}

void I2C_Init(I2C_PERIPHERALS_t* I2C, void(*remap_func)(void), uint8_t _mode)
{
	remap_func();

	I2C -> CR1 |=  (1 << I2C_SWRST);
	I2C -> CR1 &= ~(1 << I2C_SWRST);			//Reset The Peripheral

	switch(_mode)
	{
		case I2C_STANDARD_MODE:
			I2C -> CR2 |= (16 << I2C_FREQ);		//Peripheral Clock Set To 16MHz
			I2C -> CCR |= (80 << 0);			//To Generate SCL Of 100KHz
			I2C -> TRISE = 17;					//To Generate SCL Of 100KHz
			break;

		case I2C_FAST_MODE:
			I2C -> CR2 |= (10 << I2C_FREQ);		//Peripheral Clock Set To 10MHz
			I2C -> CCR |= (1 << 0);				//To Generate SCL Of 400KHz
			I2C -> TRISE = 4;					//To Generate SCL Of 400KHz

			I2C -> CCR |= (1 << I2C_DUTY);		//To Reach 400KHz
			I2C -> CCR |= (1 << I2C_FS);		//Select I2C Fast Mode
			break;
	}
	I2C -> CR1 |= (1 << I2C_PE);				//Enable The Peripheral
}

void I2C_Deinit(I2C_PERIPHERALS_t* I2C)
{
	if(I2C == I2C1)
	{
		RCC -> APB1RSTR |= (1 << I2C1RST);
	}

	else if(I2C == I2C2)
	{
		RCC -> APB1RSTR |= (1 << I2C2RST);
	}

	else if(I2C == I2C3)
	{
		RCC -> APB1RSTR |= (1 << I2C3RST);
	}
}

void I2C_Start(I2C_PERIPHERALS_t* I2C)
{
	I2C -> CR1 |= (1 << I2C_START);				//Generate The START Condition

	while(!(I2C -> SR1 & (1 << I2C_SB)));		//Wait Until START Condition Is Successfully Generated
}

void I2C_Stop(I2C_PERIPHERALS_t* I2C)
{
	I2C -> CR1 |= (1 << I2C_STOP);				//Generate STOP Condition
}

void I2C_BeginAt(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _op)
{
	uint8_t _address;

	switch(_op)
	{
		case I2C_WRITE:
			_address = (_addr << 1);
			break;

		case I2C_READ:
			_address = ((_addr << 1) | 0x01);
			break;
	}
	I2C -> DR = _address;
	while(!(I2C -> SR1 & (1 << I2C_ADDR)));
	__attribute__((unused)) uint8_t _read = (I2C -> SR1 & I2C -> SR2);
}

static inline void I2C_Write(I2C_PERIPHERALS_t* I2C, uint8_t _data)
{
	while(!(I2C -> SR1 & (1 << I2C_TXE)));
	I2C -> DR = _data;
	while(!(I2C -> SR1 & (1 << I2C_BTF)));
}

void I2C_WriteByte(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _data)
{
	I2C_Start(I2C);
	I2C_BeginAt(I2C, _addr, I2C_WRITE);
	I2C_Write(I2C, _data);
	I2C_Stop(I2C);
}

static inline uint8_t I2C_Read(I2C_PERIPHERALS_t* I2C)
{
	while(!(I2C -> SR1 & (1 << I2C_RXNE)));

	return (uint8_t)I2C -> DR;
}

uint8_t I2C_ReadByte(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _regAddr)
{
	uint8_t _buffer = 0;

	I2C_Start(I2C);
	I2C_BeginAt(I2C, _addr, I2C_WRITE);
	I2C_Write(I2C, _regAddr);
	I2C_Start(I2C);
	I2C_BeginAt(I2C, _addr, I2C_READ);
	_buffer = I2C_Read(I2C);
	I2C_Stop(I2C);

	return _buffer;
}

void I2C_ReadMulti(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _baseAddr, uint8_t* _buffer, uint8_t _nBytes)
{
	uint8_t remaining = _nBytes;

	I2C -> CR1 |= (1 << I2C_ACK);						//Enable ACK

	I2C_Start(I2C);
	I2C_BeginAt(I2C, _addr, I2C_WRITE);
	I2C_Write(I2C, _baseAddr);
	I2C_Start(I2C);
	I2C_BeginAt(I2C, _addr, I2C_READ);

	while(remaining > 2)
	{
		while(!(I2C -> SR1 & (1 << I2C_RXNE)));
		_buffer[_nBytes - remaining] = (uint8_t)I2C -> DR;
		I2C -> CR1 |= (1 << I2C_ACK);
		remaining--;
	}
	while(!(I2C -> SR1 & (1 << I2C_RXNE)));
	_buffer[_nBytes - remaining] = (uint8_t)I2C -> DR;

	I2C -> CR1 &= ~(1 << I2C_ACK);						//Generate NACK
	I2C -> CR1 |=  (1 << I2C_STOP);					//Stop The Communication
	remaining--;

	while(!(I2C -> SR1 & (1 << I2C_RXNE)));
	_buffer[_nBytes - remaining] = (uint8_t)I2C -> DR;
}

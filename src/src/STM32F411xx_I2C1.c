/*
 * STM32F411xx_I2C1.c
 *
 * Created on: 04-Nov-2021
 * Author: Devashish Lahariya
*/

#include "STM32F411xx_I2C1.h"

void I2C1_init(uint8_t _mode)
{
	RCC -> APB1ENR |= (1 << I2C1EN);			//Enable Clock For I2C1
	RCC -> AHB1ENR |= (1 << GPIOBEN);			//Enable Clock For GPIOB

	GPIOB -> AFRH |= (AF4 << 0) | (AF4 << 4);	//Re-map Pins For I2C

	GPIO_setMode(PORTB, I2C_SDA, HIGH_SPEED, OPEN_DRAIN, AFIO);
	GPIO_setMode(PORTB, I2C_SCL, HIGH_SPEED, OPEN_DRAIN, AFIO);

	I2C1 -> CR1 |=  (1 << I2C_SWRST);
	I2C1 -> CR1 &= ~(1 << I2C_SWRST);			//Reset The Peripheral

	switch(_mode)
	{
		case I2C_SM:
			I2C1 -> CR2 |= (16 << I2C_FREQ);	//Peripheral Clock Set To 16MHz
			I2C1 -> CCR |= (80 << 0);			//To Generate SCL Of 100KHz
			I2C1 -> TRISE = 17;					//To Generate SCL Of 100KHz
			break;

		case I2C_FM:
			I2C1 -> CR2 |= (10 << I2C_FREQ);	//Peripheral Clock Set To 10MHz
			I2C1 -> CCR |= (1 << 0);			//To Generate SCL Of 400KHz
			I2C1 -> TRISE = 4;					//To Generate SCL Of 400KHz

			I2C1 -> CCR |= (1 << I2C_DUTY);		//To Reach 400KHz
			I2C1 -> CCR |= (1 << I2C_FS);		//Select I2C Fast Mode
			break;
	}
	I2C1 -> CR1 |= (1 << I2C_PE);				//Enable The Peripheral
}

void I2C1_start(void)
{
	I2C1 -> CR1 |= (1 << I2C_START);			//Generate The START Condition

	while(!(I2C1 -> SR1 & (1 << I2C_SB)));		//Wait Until START Condition Is Successfully Generated
}

void I2C1_stop(void)
{
	I2C1 -> CR1 |= (1 << I2C_STOP);				//Generate STOP Condition
}

void I2C1_beginAt(uint8_t _addr, uint8_t _op)
{
	uint8_t _address;

	switch(_op)
	{
		case write:
			_address = (_addr << 1);
			break;

		case read:
			_address = ((_addr << 1) | 0x01);
			break;
	}
	I2C1 -> DR = _address;
	while(!(I2C1 -> SR1 & (1 << I2C_ADDR)));
	uint8_t _read = (I2C1 -> SR1 & I2C1 -> SR2);
}

void I2C1_write(uint8_t _data)
{
	while(!(I2C1 -> SR1 & (1 << I2C_TXE)));
	I2C1 -> DR = _data;
	while(!(I2C1 -> SR1 & (1 << I2C_BTF)));
}

void I2C1_writeByte(uint8_t _addr, uint8_t _data)
{
	I2C1_start();
	I2C1_beginAt(_addr, write);
	I2C1_write(_data);
	I2C1_stop();
}

uint8_t I2C1_read(void)
{
	while(!(I2C1 -> SR1 & (1 << I2C_RXNE)));

	return (uint8_t)I2C1 -> DR;
}

uint8_t I2C1_readByte(uint8_t _addr, uint8_t _regAddr)
{
	uint8_t _buffer = 0;

	I2C1_start();
	I2C1_beginAt(_addr, write);
	I2C1_write(_regAddr);
	I2C1_start();
	I2C1_beginAt(_addr, read);
	_buffer = I2C1_read();
	I2C1_stop();

	return _buffer;
}

void I2C1_readMulti(uint8_t _addr, uint8_t _baseAddr, uint8_t* _buffer, uint8_t nBytes)
{
	uint8_t remaining = nBytes;

	I2C1 -> CR1 |= (1 << I2C_ACK);						//Enable ACK

	I2C1_start();
	I2C1_beginAt(_addr, write);
	I2C1_write(_baseAddr);
	I2C1_start();
	I2C1_beginAt(_addr, read);

	while(remaining > 2)
	{
		while(!(I2C1 -> SR1 & (1 << I2C_RXNE)));
		_buffer[nBytes - remaining] = (uint8_t)I2C1 -> DR;
		I2C1 -> CR1 |= (1 << I2C_ACK);
		remaining--;
	}
	while(!(I2C1 -> SR1 & (1 << I2C_RXNE)));
	_buffer[nBytes - remaining] = (uint8_t)I2C1 -> DR;

	I2C1 -> CR1 &= ~(1 << I2C_ACK);						//Generate NACK
	I2C1 -> CR1 |=  (1 << I2C_STOP);					//Stop The Communication
	remaining--;

	while(!(I2C1 -> SR1 & (1 << I2C_RXNE)));
	_buffer[nBytes - remaining] = (uint8_t)I2C1 -> DR;
}

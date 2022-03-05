/*
 * STM32F411xx_I2C1.h
 *
 * Created on: 28-Oct-2021
 * Author: Devashish Lahariya
*/

#ifndef STM32F411XX_I2C_H_
#define STM32F411XX_I2C_H_

#include "STM32F411xx_GPIO.h"

//I2C1 -> CR1
#define I2C_PE 		   0U
#define I2C_SMBUS 	   1U
#define I2C_SMBTYPE    3U
#define I2C_ENARP 	   4U
#define I2C_ENPEC 	   5U
#define I2C_ENGC 	   6U
#define I2C_NOSTRETCH  7U
#define I2C_START 	   8U
#define I2C_STOP 	   9U
#define I2C_ACK 	   10U
#define I2C_POS 	   11U
#define I2C_PEC 	   12U
#define I2C_ALERT 	   13U
#define I2C_SWRST 	   15U

//I2C1 -> CR2
#define I2C_FREQ       0U
#define I2C_ITERREN    8U
#define I2C_ITEVTEN    9U
#define I2C_ITBUFEN    10U
#define I2C_DMAEN  	   11U
#define I2C_LAST 	   12U

//I2C1 -> OAR1
#define I2C_ADD0	   0U
#define I2C_ADD7_1     1U
#define I2C_ADD9_8     8U
#define I2C_ADDMODE    15U

//I2C1 -> OAR1
#define I2C_ENDUAL 	   0U
#define I2C_ADD2	   1U

//I2C1 -> SR1
#define I2C_SB		   0U
#define I2C_ADDR	   1U
#define I2C_BTF		   2U
#define I2C_ADD10	   3U
#define I2C_STOPF	   4U
#define I2C_RXNE	   6U
#define I2C_TXE		   7U
#define I2C_BERR	   8U
#define I2C_ARLO	   9U
#define I2C_AF		   10U
#define I2C_OVR		   11U
#define I2C_PECERR	   12U
#define I2C_TIMEOUT	   14U
#define I2C_SMBALERT   15U

//I2C1 -> SR2
#define I2C_MSL		   0U
#define I2C_BUSY	   1U
#define I2C_TRA		   2U
#define I2C_GENCALL	   4U
#define I2C_SMBDEFAULT 5U
#define I2C_SMBHOST	   6U
#define I2C_DUALF	   7U

//I2C1 -> CCR
#define I2C_DUTY	   14U
#define I2C_FS		   15U

//I2C1 -> FLTR
#define I2C_DNF 	   0U
#define I2C_ANOFF 	   1U

//PORTB I2C1 PINS
#define I2C_SCL 	   8U
#define I2C_SDA 	   9U

enum I2C_OPERATION_MODES
{
	I2C_STANDARD_MODE,
	I2C_FAST_MODE
};

//I2C1_beginAt() parameters
enum I2C_FUNCTION_PARAMETERS
{
	I2C_WRITE,
	I2C_READ
};

#define I2C1_CLK_ENABLE()	(RCC -> APB1ENR |= (1 << I2C1EN))
#define I2C2_CLK_ENABLE()	(RCC -> APB1ENR |= (1 << I2C2EN))
#define I2C3_CLK_ENABLE()	(RCC -> APB1ENR |= (1 << I2C3EN))

#define I2C1_CLK_DISABLE()	(RCC -> APB1ENR &= ~(1 << I2C1EN))
#define I2C2_CLK_DISABLE()	(RCC -> APB1ENR &= ~(1 << I2C2EN))
#define I2C3_CLK_DISABLE()	(RCC -> APB1ENR &= ~(1 << I2C3EN))

#define I2C1_GPIOB6_SCL		6
#define I2C1_GPIOB7_SDA		7

#define I2C1_GPIOB8_SCL		8
#define I2C1_GPIOB9_SDA		9

#define I2C2_GPIOB10_SCL	10
#define I2C2_GPIOB11_SDA	11

#define I2C3_GPIOA8_SCL		8
#define I2C3_GPIOC9_SDA		9

void I2C1_USE_GPIOB6_7(void);
void I2C1_USE_GPIOB8_9(void);

void I2C2_USE_GPIOB10_11(void);	//Note: On The STM32F411E-DISCO Board The I2C2 Will Not Work Due To The Absence Of PB11 (I2C2_SDA)

void I2C3_USE_GPIOA8_C9(void);

void I2C_Init(I2C_PERIPHERALS_t* I2C, void(*remap_func)(void), uint8_t _mode);
void I2C_Deinit(I2C_PERIPHERALS_t* I2C);
void I2C_Start(I2C_PERIPHERALS_t* I2C);
void I2C_Stop(I2C_PERIPHERALS_t* I2C);
void I2C_BeginAt(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _op);
void I2C_WriteByte(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _data);
uint8_t I2C_ReadByte(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _regAddr);
void I2C_ReadMulti(I2C_PERIPHERALS_t* I2C, uint8_t _addr, uint8_t _baseAddr, uint8_t* _buffer, uint8_t nBytes);

#endif /* STM32F411XX_I2C_H_ */

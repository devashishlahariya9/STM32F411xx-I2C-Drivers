/*
 * STM32F411xx_I2C1.h
 *
 * Created on: 28-Oct-2021
 * Author: Devashish Lahariya
*/

#ifndef STM32F411XX_I2C1_H_
#define STM32F411XX_I2C1_H_

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

//I2C1_beginAt() parameters
#define write 	   	   0U
#define read	   	   1U

//PORTB I2C1 PINS
#define I2C_SCL 	   8U
#define I2C_SDA 	   9U

//I2C Modes
#define I2C_SM		   0U
#define I2C_FM		   1U

//I2C Functions
void I2C1_init(uint8_t _mode);
void I2C1_start(void);
void I2C1_stop(void);
void I2C1_beginAt(uint8_t _addr, uint8_t _op);
void I2C1_write(uint8_t _data);
void I2C1_writeByte(uint8_t _addr, uint8_t _data);
uint8_t I2C1_read(void);
uint8_t I2C1_readByte(uint8_t _addr, uint8_t _regAddr);
void I2C1_readMulti(uint8_t _addr, uint8_t _baseAddr, uint8_t* _buffer, uint8_t nBytes);

#endif /* STM32F411XX_I2C1_H_ */

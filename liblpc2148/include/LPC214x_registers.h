/*
 * registers.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_LPC214X_REGISTERS_H_
#define INCLUDE_LPC214X_REGISTERS_H_

#define PINSEL0_SCK0 8
#define PINSEL0_MISO0 10
#define PINSEL0_MOSI0 12
#define PINSEL0_SSEL0 14

#define S0SPCR_BITENABLE 2
#define S0SPCR_CPHA 3
#define S0SPCR_CPOL 4
#define S0SPCR_MSTR 5
#define S0SPCR_LSBF 6
#define S0SPCR_SPIE 7
#define S0SPCR_BITS 8

#define S0SPSR_ABRT 3
#define S0SPSR_MODF 4
#define S0SPSR_ROVR 5
#define S0SPSR_WCOL 6
#define S0SPSR_SPIF 7

#endif /* INCLUDE_LPC214X_REGISTERS_H_ */

/*
 * spi.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_SPI_H_
#define INCLUDE_SPI_H_

void spi_init(int xfer_size, int cpha, int cpol, int master, int lsb_first, int clk_div);
void spi_xfer(int xfer_count, unsigned long *txbuf, unsigned long *rxbuf);

#endif /* INCLUDE_SPI_H_ */

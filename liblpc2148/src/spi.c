/*
 * spi.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#include "LPC214x.h"
#include "LPC214x_registers.h"

void spi_init(int xfer_size, int cpha, int cpol, int master, int lsb_first, int clk_div)
{
  PINSEL0 = (PINSEL0 & ~(3 << PINSEL0_MOSI0)) | (1 << PINSEL0_MOSI0);
  PINSEL0 = (PINSEL0 & ~(3 << PINSEL0_MISO0)) | (1 << PINSEL0_MISO0);
  PINSEL0 = (PINSEL0 & ~(3 << PINSEL0_SCK0)) | (1 << PINSEL0_SCK0);

  if (!master)
  {
    PINSEL0 = (PINSEL0 & ~(3 << PINSEL0_SSEL0)) | (1 << PINSEL0_SSEL0);
  }

  S0SPCCR = (clk_div < 8) ? 8 : (clk_div & 0xfe);;

  xfer_size = (xfer_size == 16) ? 0 : xfer_size;

  S0SPCR = ((xfer_size == 8 ? 0 : 1) << S0SPCR_BITENABLE)
      | ((cpha & 1) << S0SPCR_CPHA)
      | ((cpol & 1) << S0SPCR_CPOL)
      | ((master & 1) << S0SPCR_MSTR)
      | ((lsb_first & 1) << S0SPCR_LSBF)
      | ((xfer_size & 0xf) << S0SPCR_BITS)
      ;
}

void spi_xfer(int xfer_count, unsigned long *txbuf, unsigned long *rxbuf)
{
  int i;
  unsigned long status = 0;

  // Clear any pending status.
  status = S0SPSR;
  status = S0SPDR;

  for(i=0; i<xfer_count; i++)
  {
    S0SPDR = txbuf[i];
    status = 0;
    while(!(status & (1 << S0SPSR_SPIF)))
    {
      status = S0SPSR;
    }
    if (rxbuf != 0)
    {
      rxbuf[i] = S0SPDR;
    }
  }
}

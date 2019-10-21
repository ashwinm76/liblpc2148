/*
 * uart.c
 *
 *  Created on: Jul 24, 2017
 *      Author: Ashwin Menon
 */

#include "uart.h"
#include "vic.h"
#include "LPC214x.h"
#include "interrupts.h"

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

#define RBR          (0x00/sizeof(unsigned long))
#define THR          (0x00/sizeof(unsigned long))
#define DLL          (0x00/sizeof(unsigned long))
#define DLM          (0x04/sizeof(unsigned long))
#define IER          (0x04/sizeof(unsigned long))
#define IIR          (0x08/sizeof(unsigned long))
#define FCR          (0x08/sizeof(unsigned long))
#define LCR          (0x0C/sizeof(unsigned long))
#define MCR          (0x10/sizeof(unsigned long))
#define LSR          (0x14/sizeof(unsigned long))
#define MSR          (0x18/sizeof(unsigned long))
#define SCR          (0x1C/sizeof(unsigned long))
#define ACR          (0x20/sizeof(unsigned long))
#define FDR          (0x28/sizeof(unsigned long))
#define TER          (0x30/sizeof(unsigned long))

#define UART_BASE(n) (n == 0 ? \
    (volatile unsigned long *) UART0_BASE_ADDR : \
    (volatile unsigned long *) UART1_BASE_ADDR)

char *welcome_msg = "\r\nWelcome to the machine.\r\n";

char tx_buffer[2][TX_BUFFER_SIZE];
char rx_buffer[2][RX_BUFFER_SIZE];

volatile int tx_wr_ptr[2];
volatile int tx_rd_ptr[2];
volatile int rx_wr_ptr[2];
volatile int rx_rd_ptr[2];
volatile int rx_count[2];
volatile int tx_count[2];
volatile int tx_empty[2];
volatile int ready[2];

unsigned long divisor_table[] =
    {
        0, // auto baud
        0x1047, // 300 baud
        0x0412, // 1200 baud
        0x0082, // 9600 baud
        0x0041, // 19200 baud
        0x0021, // 38400 baud,
        0x0016 // 57600 baud
    };

unsigned long fractional_divisor_table[] =
    {
        0, // auto baud
        0x31, // 300 baud
        0x32, // 1200 baud
        0x52, // 9600 baud
        0xab, // 19200 baud
        0x7c, // 38400 baud,
        0x71 // 57600 baud
    };

static void fill_tx_fifo(int uart_num);
static void write_tx_buffer(int uart_num, char c);

void uart_init(int uart_num, struct uart_config cfg)
{
  volatile unsigned long *uart_base = UART_BASE(uart_num);

  unsigned long pinsel = PINSEL0;

  tx_wr_ptr[uart_num] = tx_rd_ptr[uart_num] = 0;
  rx_wr_ptr[uart_num] = rx_rd_ptr[uart_num] = 0;
  rx_count[uart_num] = tx_count[uart_num] = 0;
  tx_empty[uart_num] = 1;

  // Enable the UART pins
  if (uart_num == 0)
  {
    pinsel &= ~(3<<0 | 3<<2);
    pinsel |= (1<<0 | 1<<2);
  }
  else
  {
    pinsel &= ~(3<<16 | 3<<18);
    pinsel |= (1<<16 | 1<<18);
  }
  PINSEL0 = pinsel;

  if (cfg.baud != UART_BAUD_AUTO)
  {
    // Set DLAB.
    *(uart_base + LCR) |= (1 << 7);

    *(uart_base + DLL) = divisor_table[cfg.baud] & 0xff;
    *(uart_base + DLM) = divisor_table[cfg.baud] >> 8;
    *(uart_base + FDR) = fractional_divisor_table[cfg.baud];

    // Clear DLAB.
    *(uart_base + LCR) &= ~(1 << 7);

    ready[uart_num] = 1;
  }

  *(uart_base + LCR) = cfg.word_length
      | (cfg.stop_bits << 2)
      | (cfg.parity == UART_PARITY_NONE ? 0 : (1 << 3))
      | ((cfg.parity & 3) << 4);

  // Enable the FIFOs and reset them.
  *(uart_base + FCR) = 0x3;

  // Set up the UART ISR and enable it.
  vic_setup_isr(uart_num == 0 ? VIC_SRC_UART0 : VIC_SRC_UART1,
      VIC_INT_IRQ,
      cfg.interrupt_priority,
      uart_num == 0 ? uart0_isr : uart1_isr);
  vic_interrupt_enable(1 << (uart_num == 0 ? VIC_SRC_UART0 : VIC_SRC_UART1));

  if (cfg.baud == UART_BAUD_AUTO)
  {
    // Enable Mode 0 auto-baud with auto restart.
    *(uart_base + ACR) = 0x5;

    // Enable the ABEO interrupt.
    *(uart_base + IER) = 0x100;

    ready[uart_num] = 0;
  }

  // Enable the RBR interrupt.
  *(uart_base + IER) |= 1;
}

int uart_ready(int uart_num)
{
  return ready[uart_num];
}

char uart_getc(int uart_num)
{
  char d;
  while (rx_count[uart_num] == 0)
  {

  }

  CRITICAL_SECTION_START
  {
    d = rx_buffer[uart_num][rx_rd_ptr[uart_num]++];
    if (rx_rd_ptr[uart_num] >= RX_BUFFER_SIZE)
    {
      rx_rd_ptr[uart_num] = 0;
    }
    rx_count[uart_num]--;
  }
  CRITICAL_SECTION_END

  return d;
}

void uart_gets(int uart_num, char *buf, int buf_size)
{
  char c = 0;
  int count = 0;

  while(c != '\n' && count < buf_size-1)
  {
    c = uart_getc(uart_num);

    // Handle backspace
    if (c == '\b' && count > 0)
    {
      count--;
    }
    // Handle printable characters
    else if (c > 31 && c < 127)
    {
      buf[count++] = c;
    }
  }
  buf[count] = '\0';
}

void uart_putc(int uart_num, int c)
{
  while(tx_count[uart_num] >= TX_BUFFER_SIZE)
  {

  }

  CRITICAL_SECTION_START
  {
    write_tx_buffer(uart_num, c);
  }
  CRITICAL_SECTION_END
}

void uart_puts(int uart_num, const char *s)
{
  while(*s)
  {
    uart_putc(uart_num, *s++);
  }
}

int uart_rx_count(int uart_num)
{
  return rx_count[uart_num];
}

static void uart_isr(int uart_num, volatile unsigned long *uart_base)
{
  unsigned long status;

  // Read the IIR to clear it.
  status = *(uart_base + IIR);

  // Handle the Auto Baud Complete interrupt
  if ((status & 1) == 0)
  {
    if (status & 0x100)
    {
      // Clear the interrupt.
      *(uart_base + ACR) |= 0x100;

      // Flush the sync character
      (void)*(uart_base + RBR);

      // Send the welcome message.
      char *c = welcome_msg;
      while(*c)
      {
        write_tx_buffer(uart_num, *c++);
      }

      ready[uart_num] = 1;
    }
  }

  status = *(uart_base + LSR);

  // Handle THRE
  if (status & (1<<5))
  {
    if (tx_count[uart_num] == 0)
    {
      // If there's nothing to send, then disable THRE.
      *(uart_base + IER) &= ~2;

      tx_empty[uart_num] = 1;
    }
    else
    {
      fill_tx_fifo(uart_num);
    }
  }

  // Handle RDA and CTI
  if (status & 1)
  {
    while(*(uart_base + LSR) & 1)
    {
      unsigned char data = *(uart_base + RBR);
      if (rx_count[uart_num] < RX_BUFFER_SIZE)
      {
        rx_buffer[uart_num][rx_wr_ptr[uart_num]++] = data;
        if (rx_wr_ptr[uart_num] >= RX_BUFFER_SIZE)
        {
          rx_wr_ptr[uart_num] = 0;
        }
        rx_count[uart_num]++;
      }
    }
  }
}

void uart0_isr()
{
  uart_isr(0, (unsigned long*)UART0_BASE_ADDR);
}

void uart1_isr()
{
  uart_isr(1, (unsigned long*)UART1_BASE_ADDR);
}

static void fill_tx_fifo(int uart_num)
{
  volatile unsigned long *uart_base = UART_BASE(uart_num);

  int count = tx_count[uart_num] < 16 ? tx_count[uart_num] : 16;

  while(count > 0)
  {
    *(uart_base + THR) = tx_buffer[uart_num][tx_rd_ptr[uart_num]++];
    if (tx_rd_ptr[uart_num] >= TX_BUFFER_SIZE)
    {
      tx_rd_ptr[uart_num] = 0;
    }
    tx_count[uart_num]--;
    count--;
  }
}

static void write_tx_buffer(int uart_num, char c)
{
  volatile unsigned long *uart_base = UART_BASE(uart_num);

  tx_buffer[uart_num][tx_wr_ptr[uart_num]++] = c;
  if (tx_wr_ptr[uart_num] >= TX_BUFFER_SIZE)
  {
    tx_wr_ptr[uart_num] = 0;
  }
  tx_count[uart_num]++;

  // Enable THRE.
  *(uart_base + IER) |= 2;

  if (tx_empty[uart_num])
  {
    fill_tx_fifo(uart_num);
    tx_empty[uart_num] = 0;
  }
}

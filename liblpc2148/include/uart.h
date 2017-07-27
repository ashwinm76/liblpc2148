/*
 * uart.h
 *
 *  Created on: Jul 23, 2017
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

enum uart_word_length
{
  UART_5BITS = 0,
  UART_6BITS = 1,
  UART_7BITS = 2,
  UART_8BITS = 3
};

enum uart_stop_bits
{
  UART_1STOP_BITS = 0,
  UART_2STOP_BITS = 1
};

enum uart_parity
{
  UART_PARITY_ODD = 0,
  UART_PARITY_EVEN = 1,
  UART_PARITY_FORCE1 = 2,
  UART_PARITY_FORCE0 = 3,
  UART_PARITY_NONE = 4
};

enum uart_baud
{
  UART_BAUD_AUTO = 0,
  UART_BAUD_300,
  UART_BAUD_1200,
  UART_BAUD_9600,
  UART_BAUD_19200,
  UART_BAUD_38400,
  UART_BAUD_57600
};

struct uart_config
{
  enum uart_baud baud;
  enum uart_word_length word_length;
  enum uart_stop_bits stop_bits;
  enum uart_parity parity;
  int interrupt_priority;
};

void uart_init(int uart_num, struct uart_config cfg);
int uart_ready(int uart_num);
char uart_getc(int uart_num);
void uart_gets(int uart_num, char *buf, int buf_size);
void uart_putc(int uart_num, int c);
void uart_puts(int uart_num, const char *s);

#endif /* INCLUDE_UART_H_ */

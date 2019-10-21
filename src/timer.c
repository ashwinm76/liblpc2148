/*
 * timer.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Ashwin Menon
 */

#include <stddef.h>
#include "timer.h"
#include "vic.h"
#include "LPC214x.h"

#define TIMER_IR   (0x00/sizeof(unsigned long))
#define TIMER_TCR  (0x04/sizeof(unsigned long))
#define TIMER_TC   (0x08/sizeof(unsigned long))
#define TIMER_PR   (0x0C/sizeof(unsigned long))
#define TIMER_PC   (0x10/sizeof(unsigned long))
#define TIMER_MCR  (0x14/sizeof(unsigned long))
#define TIMER_MR0  (0x18/sizeof(unsigned long))
#define TIMER_MR1  (0x1C/sizeof(unsigned long))
#define TIMER_MR2  (0x20/sizeof(unsigned long))
#define TIMER_MR3  (0x24/sizeof(unsigned long))
#define TIMER_CCR  (0x28/sizeof(unsigned long))
#define TIMER_CR0  (0x2C/sizeof(unsigned long))
#define TIMER_CR1  (0x30/sizeof(unsigned long))
#define TIMER_CR2  (0x34/sizeof(unsigned long))
#define TIMER_CR3  (0x38/sizeof(unsigned long))
#define TIMER_EMR  (0x3C/sizeof(unsigned long))
#define TIMER_CTCR (0x70/sizeof(unsigned long))

internal_timer_callback *match_callbacks[2][8];
static void internal_timer0_isr();
static void internal_timer1_isr();

void internal_timer_init(int timer_num, struct internal_timer_config cfg)
{
  int i;
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  for(i=0; i<4; i++)
  {
    match_callbacks[timer_num][i] = NULL;
  }

  // Disable and reset the timer counter.
  *(timer_base + TIMER_TCR) = 2;

  // Clear any interrupts.
  *(timer_base + TIMER_IR) = 0xff;

  // Select timer mode on every rising PCLK edge.
  *(timer_base + TIMER_CTCR) = 0;

  // Set the prescale.
  *(timer_base + TIMER_PR) = cfg.pclk_prescale;

  // Disable matches.
  *(timer_base + TIMER_MCR) = 0;

  // Disable captures.
  *(timer_base + TIMER_CCR) = 0;

  // Disable external matches.
  *(timer_base + TIMER_EMR) = 0;

  // Set up the TIMER ISR and enable it
  vic_setup_isr(timer_num == 0 ? VIC_SRC_TIMER0 : VIC_SRC_TIMER1,
      VIC_INT_IRQ,
      cfg.interrupt_priority,
      timer_num == 0 ? internal_timer0_isr : internal_timer1_isr);
  vic_interrupt_enable(1 << (timer_num == 0 ? VIC_SRC_TIMER0 : VIC_SRC_TIMER1));
}

void internal_timer_resume(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 1;
}

void internal_timer_pause(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 0;
}

void internal_timer_reset(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_TCR) = 2;

  // Wait for the timer to reset.
  while(*(timer_base + TIMER_TC) != 0)
  {

  }
}

unsigned long internal_timer_read(int timer_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  return *(timer_base + TIMER_TC);
}

void internal_timer_setup_match(int timer_num,
                                   unsigned long period,
                                   enum internal_timer_match_number match_num,
                                   enum internal_timer_match_action action,
                                   enum internal_timer_ext_match_action ext_action,
                                   internal_timer_callback* cb)
{
  unsigned long mr;

  volatile unsigned long *timer_base;
    timer_base = (
        timer_num == 0 ?
            (volatile unsigned long *) TMR0_BASE_ADDR :
            (volatile unsigned long *) TMR1_BASE_ADDR);

    // Use Match 0 by default
    switch(match_num)
    {
      case MATCH1: mr = TIMER_MR1; break;
      case MATCH2: mr = TIMER_MR2; break;
      case MATCH3: mr = TIMER_MR3; break;
      default: mr = TIMER_MR0; break; //  MATCH0
    }
    *(timer_base + mr) = period;

    // Configure external match
    *(timer_base + TIMER_EMR) = (*(timer_base + TIMER_EMR) & (~3 << (4 + (2 * match_num)))) & 0xfff;
    *(timer_base + TIMER_EMR) |= ext_action << (4 + (2 * match_num));

    // Interrupt (if required) and reset the timer on match
    *(timer_base + TIMER_MCR) = (*(timer_base + TIMER_MCR) & (~7 << (3 * match_num))) & 0xfff;
    if (cb)
    {
      match_callbacks[timer_num][match_num] = cb;
      *(timer_base + TIMER_MCR) |= ((action << 1) + 1) << (3 * match_num);
    }
    else
    {
      *(timer_base + TIMER_MCR) |= (action << 1) << (3 * match_num);
    }
}

void internal_timer_setup_capture(int timer_num,
                                  enum internal_timer_capture_number cap_num,
                                  enum internal_timer_capture_edge edge,
                                  internal_timer_callback* cb)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  // Set up the capture edge and interrupt (if required)
  *(timer_base + TIMER_CCR) = (*(timer_base + TIMER_CCR) & (~7 << (3 * cap_num))) & 0xfff;
  if (cb)
  {
    match_callbacks[timer_num][4+cap_num] = cb;
    *(timer_base + TIMER_CCR) |= (4 + edge) << (3 * cap_num);
  }
  else
  {
    *(timer_base + TIMER_CCR) |= edge << (3 * cap_num);
  }
}

void internal_timer_set_match_output(int timer_num, enum internal_timer_match_number match_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_EMR) = (*(timer_base + TIMER_EMR) | (1 << match_num)) & 0xfff;
}

void internal_timer_clr_match_output(int timer_num, enum internal_timer_match_number match_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_EMR) = (*(timer_base + TIMER_EMR) & ~(1 << match_num)) & 0xfff;
}

int internal_timer_get_match_output(int timer_num, enum internal_timer_match_number match_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  return (*(timer_base + TIMER_EMR) >>  match_num) & 1;
}

void internal_timer_toggle_match_output(int timer_num, enum internal_timer_match_number match_num)
{
  volatile unsigned long *timer_base;
  timer_base = (
      timer_num == 0 ?
          (volatile unsigned long *) TMR0_BASE_ADDR :
          (volatile unsigned long *) TMR1_BASE_ADDR);

  *(timer_base + TIMER_EMR) = (*(timer_base + TIMER_EMR) ^ (1 << match_num)) & 0xfff;
}

static void internal_timer0_isr()
{
#define TIMER_BASE ((volatile unsigned long*)TMR0_BASE_ADDR)
#define TIMER_NUM 0

  int i = 0;
  unsigned long status;

  // Read and clear the interrupt bit
  status = *(TIMER_BASE + TIMER_IR);
  *(TIMER_BASE + TIMER_IR) = status & 0xff;

  // Call the callbacks
  while(status)
  {
    if ((status & 1) && match_callbacks[TIMER_NUM][i])
    {
      match_callbacks[TIMER_NUM][i]();
    }
    i++;
    status >>= 1;
  }

#undef TIMER_NUM
#undef TIMER_BASE
}

static void internal_timer1_isr()
{
#define TIMER_BASE ((volatile unsigned long*)TMR1_BASE_ADDR)
#define TIMER_NUM 1

  int i = 0;
  unsigned long status;

  // Read and clear the interrupt bit
  status = *(TIMER_BASE + TIMER_IR);
  *(TIMER_BASE + TIMER_IR) = status & 0xff;

  // Call the callbacks
  while(status)
  {
    if ((status & 1) && match_callbacks[TIMER_NUM][i])
    {
      match_callbacks[TIMER_NUM][i]();
    }
    i++;
    status >>= 1;
  }

#undef TIMER_NUM
#undef TIMER_BASE
}


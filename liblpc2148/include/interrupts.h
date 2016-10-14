/*
 * interrupts.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_INTERRUPTS_H_
#define INCLUDE_INTERRUPTS_H_

static inline void enable_irq()
{
  asm volatile (
          "mrs  r0, cpsr \n\t"
          "bic  r0, r0, #0x80 \n\t"
          "msr  cpsr_c, r0 \n\t"
          :
          :
          : "r0"
                  );
}

static inline void disable_irq()
{
  asm volatile (
        "mrs  r0, cpsr \n\t"
        "orr  r0, r0, #0x80 \n\t"
        "msr  cpsr_c, r0 \n\t"
        :
        :
        : "r0"
                );
}

static inline void enable_fiq()
{
  asm volatile (
            "mrs  r0, cpsr \n\t"
            "bic  r0, r0, #0x40 \n\t"
            "msr  cpsr_c, r0 \n\t"
            :
            :
            : "r0"
                    );
}

static inline void disable_fiq()
{
  asm volatile (
          "mrs  r0, cpsr \n\t"
          "orr  r0, r0, #0x40 \n\t"
          "msr  cpsr_c, r0 \n\t"
          :
          :
          : "r0"
                  );
}

#endif /* INCLUDE_INTERRUPTS_H_ */

/*
 * interrupts.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Ashwin Menon
 */

#ifndef INCLUDE_INTERRUPTS_H_
#define INCLUDE_INTERRUPTS_H_

#define CRITICAL_SECTION_START \
{ \
  volatile unsigned long cpsr_save; \
  asm volatile ( \
                 "mrs r0, cpsr \n\t" \
                 "and r1, r0, #0x80 \n\t" \
                 "str r1, %0 \n\t" \
                 "orr  r0, r0, #0x80 \n\t" \
                 "msr  cpsr_c, r0 \n\t" \
                 : "=m" (cpsr_save) \
                 : \
                 : "r0","r1" \
                 );

#define CRITICAL_SECTION_END \
  asm volatile ( \
                 "mrs r0, cpsr \n\t" \
                 "ldr r1, %0 \n\t" \
                 "cmp r1, #0 \n\t" \
                 "andeq r0, #0x7f \n\t" \
                 "orrne r0, #0x80 \n\t" \
                 "msr  cpsr_c, r0 \n\t" \
                 : \
                 : "m" (cpsr_save) \
                 : "r0","r1" \
                 ); \
}

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

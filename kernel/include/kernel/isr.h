#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

#define		ISQ0	32
#define		ISQ1	33
#define		ISQ2	34
#define		ISQ3	35
#define		ISQ4	36
#define		ISQ5	37
#define		ISQ6	38
#define		ISQ7	39
#define		ISQ8	40
#define		ISQ9	41
#define		ISQ10	42
#define		ISQ11	43
#define		ISQ12	44
#define		ISQ13	45
#define		ISQ14	46
#define		ISQ15	47

struct	registers_s
{
	uint32_t	ds;			// data segment
	uint32_t	edi;			// 32b destination
	uint32_t	esi;			// 32b source
	uint32_t	ebp;			// 32b stack base pointer
	uint32_t	esp;			// 32b stack pointer
	uint32_t	ebx;			// 32b base
	uint32_t	edx;			// 32b destination
	uint32_t	ecx;			// 32b counter
	uint32_t	eax;			// 32b accumlator
	uint32_t	interrupt_number;
	uint32_t	error_code;
	uint32_t	eip;			// 32b instruction pointer
	uint32_t	cs;			// code segment

	uint32_t	eflags;			// 32b register used as a collection of 
						// bits representing bool values
						// to store results of operations and 
						// the state of the processor

	uint32_t	user_esp;		// user 32b stack pointer
	uint32_t	ss;			// stack segment
};
typedef struct registers_s	registers_t;

typedef void (*isr_t)(registers_t);

// registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the first param;
void register_interrupt_handler(uint8_t interrupt_number, isr_t handler);

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);


#endif

#include <kernel/isr.h>
#include <stdio.h>

isr_t interrupt_handlers[256];

void 
isr_handler (registers_t regs)
{
	printf ("recieved --ISR (from CPU)-- interrupt: %d.\n", regs.interrupt_number);
	
	switch (regs.interrupt_number)
	{
		case 0:
			printf("divide by 0 interupt\n");
			// divide by 0 interrupt
			break;
		default:
			printf("unknown interrupt %d", regs.interrupt_number);
			// unknown interrupt
			break;
	}
}

void
irq_handler (registers_t regs)
{
	if (interrupt_handlers[regs.interrupt_number] != 0)
	{
		isr_t handler = interrupt_handlers[regs.interrupt_number];
		handler(regs);
	}

	if (regs.interrupt_number >= 40)
	{
		asm volatile ( "outb %0, %1" : : "a"((uint8_t) 0xA0), "Nd"((uint8_t) 0x20));
	}
	asm volatile ( "outb %0, %1" : : "a"((uint8_t) 0x20), "Nd"((uint8_t) 0x20));
}

void
register_interrupt_handler  (uint8_t interrupt_number, isr_t handler)
{
	interrupt_handlers[interrupt_number] = handler;
}


//#include <kernel/dt.h>
#include "../../include/kernel/dt.h"
#include <string.h>

static inline uint8_t 
inb (uint16_t port)
{
	uint8_t ret;

	asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );

	return (ret);
}

extern void	gdt_flush (uint32_t);
extern void	idt_flush (uint32_t);

// null, code, data, user code & user data segments 
sd_t		gdt_entries [5];
sd_ptr_t	gdt_ptr;

id_t		idt_entries [256];
id_ptr_t	idt_ptr;

static void
sd_set_gate (uint32_t num, uint32_t base, uint32_t limit,
		uint8_t access, uint8_t granularity)
{
	gdt_entries[num].base_low	= (base			& 0xFFFF);
	gdt_entries[num].base_middle	= (base  >>  0x10)	& 0x00FF;
	gdt_entries[num].base_high	= (base  >>  0x18)	& 0x00FF;

	gdt_entries[num].limit_low	= (limit		& 0xFFFF);
	gdt_entries[num].granularity	= (limit >>  0x10)	& 0x000F;
	
	gdt_entries[num].granularity	|= granularity		& 0x00F0;
	gdt_entries[num].access		 = access;
};

static void
id_set_gate (uint8_t num, uint32_t base, 
		uint16_t sel, uint8_t flags)
{
	idt_entries[num].base_lo = base		  & 0xFFFF;
	idt_entries[num].base_hi = (base >> 0x10) & 0xFFFF;

	idt_entries[num].sel	 = sel;
	idt_entries[num].always0 = 0x00;
	
	idt_entries[num].flags	 = flags;
}

static void
gdt_init (void)
{
	gdt_ptr.limit = (sizeof(sd_t) * 5) - 1;
	gdt_ptr.base  = (uint32_t) & gdt_entries;

	sd_set_gate(0, 0, 0, 0, 0);
	sd_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	sd_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	sd_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	sd_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((uint32_t) & gdt_ptr);
}

static void 
idt_init (void)
{
	size_t i;

	idt_ptr.limit = sizeof(id_t) * 256 - 1;
	idt_ptr.base  = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(id_t) * 256);
	
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint8_t)0x11) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0xA0), "Nd"((uint8_t)0x11) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0x21), "Nd"((uint8_t)0x20) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0xA1), "Nd"((uint8_t)0x28) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0x21), "Nd"((uint8_t)0x04) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0xA1), "Nd"((uint8_t)0x02) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0x21), "Nd"((uint8_t)0x01) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0xA1), "Nd"((uint8_t)0x01) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0x21), "Nd"((uint8_t)0x00) );
	asm volatile ( "outb %0, %1" : : "a"((uint8_t)0xA1), "Nd"((uint8_t)0x00) );
	
	i = 0;
	id_set_gate (i, (uint32_t)isr0, 0x08, 0x8E);
	while (i < 31)
	{
		id_set_gate (i, (uint32_t)isr1, 0x08, 0x8E);
		++i;
	}
	i = 32;
	id_set_gate(i, (uint32_t)irq0, 0x08, 0x8E);
	i++;
	id_set_gate(i, (uint32_t)irq1, 0x08, 0x8E);

	idt_flush((uint32_t)&idt_ptr);
}

void 
descriptor_tables_init (void)
{
	gdt_init(); 
	idt_init();
}


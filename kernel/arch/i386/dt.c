//#include <kernel/dt.h>
#include "../../include/kernel/dt.h"

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

}

static void 
idt_init (void)
{

}

void 
descriptor_tables_init (void)
{
	gdt_init(); 
	idt_init();
}

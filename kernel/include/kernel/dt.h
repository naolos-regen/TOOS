#ifndef _DT_H
#define _DT_H

#include <stdint.h>

// source :. http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

// structure contains the value of one segment descriptor
// we use the attribute packed to tell gcc to not change the order of bits
// any dealignment causes issues
struct	sd_s
{
	uint16_t	 limit_low;	// lower 16 b limit
	uint16_t	 base_low;	// lower 16 b base
	uint8_t		 base_middle;	// next   8 b base
	uint8_t		 access;	// aka type, what ring (?)
	uint8_t		 granularity;
	uint8_t		 base_high;	// last   8 b base
} __attribute__((packed));
typedef struct sd_s		sd_t;

// to tell the cpu where to find out segment descriptor, we have to give
// it the addr of the special pointer structure
struct	sd_ptr_s
{
	uint16_t	 limit;		// upper 16 b all selector limits 
	uint32_t	 base;		// addr of the first sd_entry_t.
} __attribute__((packed));
typedef struct sd_ptr_s		sd_ptr_t;

// interrupt descriptor gate
struct	id_s
{
	uint16_t	 base_lo;	// lower 16 b addr to jump to when interrupt fires.
	uint16_t	 sel;		// kernel segment selector
	uint8_t		 always0;	// must be always 0
	uint8_t		 flags;		// flags.
	uint8_t		 base_hi;	// upper 16 b addr to jump to;
} __attribute__((packed));
typedef struct id_s		id_t;

// struct describing a pointer to an array of interrupt handlers.
struct  id_ptr_s
{
	uint16_t	 limit;
	uint32_t	 base;
} __attribute__((packed));
typedef struct id_ptr_s		id_ptr_t;

// INIT gdt & idt
void	descriptor_tables_init	(void);

// these let us access the addresses of our ASM ISR/IRQ handlers.
extern	void isr0 ();
extern	void isr1 ();
extern	void irq0 ();
extern	void irq1 ();

#endif

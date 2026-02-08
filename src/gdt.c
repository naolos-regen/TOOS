#include "gdt.h"

struct	global_descriptor_table 
initialize_global_descriptor_table()
{
	struct global_descriptor_table gdt = 
	{
		.null_segment_selector	 = initialize_segment_descriptor (0, 0, 0),
		.unused_segment_selector = initialize_segment_descriptor (0, 0, 0),
		.code_segment_selector   = initialize_segment_descriptor (0, 64*1024*1024, 0x9A),
		.data_segment_selector   = initialize_segment_descriptor (0, 64*1024*1024, 0x92)
	};

	u32 i[2];
	i[1] = (u32) &gdt;
	i[0] = sizeof(struct global_descriptor_table) << 16;

	asm volatile("lgdt (%0)": :"p" ( ( (u8 *) i)+2));

	return (gdt);
}

u16	code_segment_selector		
(struct global_descriptor_table * gdt)
{
	return (u8 *) &(gdt->code_segment_selector) - (u8 *)gdt;
};

u16	data_segment_selector		
(struct global_descriptor_table * gdt)
{
	return (u8 *) &(gdt->data_segment_selector) - (u8 *)gdt;
};


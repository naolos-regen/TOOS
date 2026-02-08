#include "gdt.h"

struct	segment_descriptor
initialize_segment_descriptor (u32 base, u32 limit, u8 type)
{
	struct segment_descriptor desc;
	
	u8 *target;
       
	target = (u8*) &desc;
	if (limit < 65536)
	{
		// 16 bit
		target[6] = 0x40;
	}
	else 
	{
		// 32 bit
		if ((limit & 0xFFF) != 0xFFF)
			limit = (limit >> 12) -1;
		else
			limit = limit >> 12;

		target[6] = 0xC0;
	};
	
	target[0]	= limit & 0xFFFF;
	target[1]	= (limit >> 16) & 0x0F;
	target[6]	= target[6] | (limit >> 16) & 0xF;

	target[2]	= base & 0xFF;
	target[3]	= (base >> 8)  & 0xFF;
	target[4]	= (base >> 16) & 0xFF;
	target[7]	= (base >> 24) & 0xFF;

	target[5]	= type;

	return (desc);
}

u32
segment_descriptor_table_base (struct segment_descriptor * sd)
{
	u8 *target;
	u32 res;

	target = (u8 *) sd;

	res = target[7];
	res = (res << 8) + target[4];
	res = (res << 8) + target[3];
	res = (res << 8) + target[2];

	return (res);
}

u32
segment_descriptor_table_limit (struct segment_descriptor * sd)
{
	u8 *target;
	u32 res;

	target = (u8 *) sd;
	res = target[6] & 0xF;
	res = (res << 8) + target[1];
	res = (res << 8) + target[0];

	if ((target[6] & 0xC0) == 0xC0)
		res = (res << 12) | 0xFFF;
	
	return (res);
}


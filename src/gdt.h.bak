#ifndef __GDT_H
#define __GDT_H

#include "types.h"

struct	segment_descriptor
{
	u16		limit_lo;
	u16		base_lo;
	u8		base_hi;
	u8		type;
	u8		limit_hi;
	u8		base_vhi;
};

struct  segment_descriptor	initialize_segment_descriptor
				(u32, u32, u8);

u32	global_descriptor_table_base	(struct segment_descriptor *);
u32	global_descriptor_table_limit	(struct segment_descriptor *);

struct  global_descriptor_table
{
	struct segment_descriptor null_segment_selector;
	struct segment_descriptor unused_segment_selector;
	struct segment_descriptor code_segment_selector;
	struct segment_descriptor data_segment_selector;
};

struct	global_descriptor_table initialize_global_descriptor_table();

u16	code_segment_selector		(struct global_descriptor_table *);
u16	data_segment_selector		(struct global_descriptor_table *);

#endif


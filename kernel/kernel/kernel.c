#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/dt.h>

void kernel_main(void) 
{
	terminal_initialize();
	
	terminal_writestring("Hello, World!\n");
	terminal_writestring("initializing descriptor tables\n");

	descriptor_tables_init();
}

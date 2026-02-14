#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/dt.h>

void kernel_main(void) 
{
	terminal_initialize();
	
	printf("Hello, kernel World!\n");
	
	printf("Initializing descriptor table\n");

	descriptor_tables_init();
}

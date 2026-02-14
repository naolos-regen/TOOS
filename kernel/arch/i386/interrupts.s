.global isr0
.global isr1
.extern isr_handler

isr0:
	cli			# Disable interrupts
	push $0			# Dummy error code (if ISR0 doesn't push it's own error code)
	push $0			# Push the interrupts number (0)
	jmp isr_common_stub	# goto common handler

isr1:
	cli			# Disable interrupts
	push $0			# Dummy error code (if ISR1 doesn't push it's own error code)
	push $1			# Push the interrupts number (1)
	jmp isr_common_stub	# goto common handler


# Saves the processor state, sets up for the kernel mode segments
# calls the C-level fault handler and restores the stack frame.
#
isr_common_stub:
	pusha			# push edi, esi, ebp, esp, ebx, edx, ecx, eax

	mov %ds, %ax		# lower 16b eax = ds.
	push %eax		# save the data segment descriptor

	mov $0x10, %ax		# load the kernel data segment descriptor
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call isr_handler

	pop %eax		# reload the original data segment descriptor
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa			# pops edi, esi, ebp, esp, ebx, edx, ecx, eax
	add $8, %esp
	iret

# IRQ handles - Interrupts 32 - 47 that are coming from the Master and Slave PICs
# This will handle real hardware
.global irq0
.global irq1
.extern irq_handler # isr.c


# Programmable timer interrupt
irq0:
	cli
	push $0
	push $32
	jmp irq_common_stub

# Keyboard interrupt
irq1:
	cli
	push $0
	push $33
	jmp irq_common_stub


# Saves the processor state, sets up for the kernel mode segments
# calls the C-level fault handler and restores the stack frame.
# 
irq_common_stub:
	pusha			# push edi, esi, ebp, esp, ebx, edx, ecx, eax

	mov %ds, %ax		# lower 16b of eax = ds
	push %eax		# save the data segment descriptor

	mov $0x10, %ax		# load the kernel data segment descriptor
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	call irq_handler
	
	pop %ebx		# reload the original data segment descriptor
	mov %bx, %ds
	mov %bx, %es
	mov %bx, %fs
	mov %bx, %gs

	popa			# pops edi, esi, ebp, esp, ebx, edx, ecx, eax
	add $8, %esp		# Cleans up the pushed error code and pushed ISR number
	sti
	iret			# pops 5 things at once: CS, EIP, EFLAGS, SS and ESP


#include "interrupt.h"
#include "keyboard.h"

struct idt_gate interrupt_descriptor_table[256];
volatile unsigned int ticks = 0;
volatile char last_key = 0;

uint8_t inb (uint16_t _port) {
    uint8_t rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void set_idt_gate(void *handler_address, unsigned int gate_number) {
    interrupt_descriptor_table[gate_number].offset_1 = (uint32_t)handler_address & 0xffff;
    interrupt_descriptor_table[gate_number].offset_2 = ((uint32_t)handler_address >> 16) & 0xffff;
    interrupt_descriptor_table[gate_number].cs_selector = 0x10;
    interrupt_descriptor_table[gate_number].zero = 0;
    interrupt_descriptor_table[gate_number].attributes = 0x8E;
}


void idt_flush(struct idt_descriptor *idt) {
   asm("lidt %0\n"
    :              // outputs
    : "m"(*idt)    // inputs
    :);            // clobbers (registers the inline assembly block changes)
}

void pic_init() {
    outb(0x20, 0x11); // RESET command to primary PIC
    outb(0xA0, 0x11); // RESET comand to secondary PIC

    // Remapping the PIC
    outb(0x21, 32);
    outb(0xA1, 40);

    outb(0x21, 4); // Primary PIC cascade interrupts
    outb(0xA1, 2); // Secondary PIC - "slave" mode

    outb(0x21, 1); // 8086 mode
    outb(0xA1, 1); // 8086 mode

    outb(0x21, 0xFC); // Unmask interrupts
    outb(0xA1, 0xFF); // Unmask interrupts
}

__attribute__ ((interrupt)) void keyboard_handler(void*p) {
    // Read & print scancode
    uint8_t scancode = inb(0x60);
    if(!(scancode & 0x80)) {
        last_key = keyboard_map[scancode];
    }
    outb(0x20, 0x20); // Acknowledges the interrupt
}

__attribute__ ((interrupt)) void pit_handler(void*p) {
   ticks++;
   outb(0x20, 0x20);
}

__attribute__ ((interrupt)) void dummy_handler(void*p) {
}

int setupIDT() {
	// IDT setup
    for (int k = 0; k < 256; k++)
        set_idt_gate(dummy_handler, k);
    set_idt_gate(keyboard_handler, 33);
    set_idt_gate(pit_handler, 32);

    struct idt_descriptor idt_desc;
    idt_desc.limit = sizeof(interrupt_descriptor_table);
    idt_desc.base  = (uint32_t)interrupt_descriptor_table;
    idt_flush(&idt_desc);

    pic_init();
    asm("sti");
    return 1;
}


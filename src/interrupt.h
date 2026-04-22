#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

extern volatile unsigned int ticks;
extern volatile char last_key;

uint8_t inb(uint16_t _port);

static inline void outb(uint16_t _port, uint8_t val) {
	__asm__ __volatile__ ("outb %0, %1" : : "a" (val), "dN" (_port));
}

struct idt_gate {
    uint16_t offset_1;    // LS 16bits of interrupt handler's address
    uint16_t cs_selector; // 0x10 code segment selector
    uint8_t zero;         // must be zero
    uint8_t attributes;   // 0x8E tells processor what type of gate
    uint16_t offset_2;    // MS 16bits of interrupt handler's address
};

struct idt_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

extern struct idt_gate interrupt_descriptor_table[256];

void set_idt_gate(void *handler_address, unsigned int gate_number);

void idt_flush(struct idt_descriptor *idt);

void pic_init(void);

__attribute__ ((interrupt)) void keyboard_handler(void *p);
__attribute__ ((interrupt)) void pit_handler(void*p);
__attribute__ ((interrupt)) void dummy_handler(void*p);

int setupIDT(void);


#endif

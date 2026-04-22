#include "interrupt.h"

static unsigned int lcg_state = 42069;


unsigned int lcg_rand() {
    lcg_state = lcg_state * 1664525 + 1013904223;
    return lcg_state;
}

void seed_rng() {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));

    // XORing is good—it preserves existing state entropy
    lcg_state ^= (lo ^ ticks ^ 0xDFBACDFE);

    // Use the raw generator to scramble the state
    for(int i = 0; i < 10; i++) {
        lcg_rand();
    }
}

// get a value in range [0, max)
unsigned int rand(unsigned int max) {
    return lcg_rand() % max;
}

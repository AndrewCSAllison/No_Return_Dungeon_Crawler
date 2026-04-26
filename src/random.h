#ifndef RANDOM_H
#define RANDOM_H

extern unsigned int lcg_state;

void seed_rng();
unsigned int lcg_rand();
unsigned int rand(unsigned int max);

#endif

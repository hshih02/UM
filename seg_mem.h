/*seg_mem.h */
#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <seq.h>
#include "mem.h"
#include <inttypes.h>

struct mem {
        Seq_T segments;
        Seq_T unmapped;
} mem;

void init_mem(Seq_T program_words);

Seq_T get_seg(int index);

uint32_t get_word(int s_ind, int w_ind);

#endif
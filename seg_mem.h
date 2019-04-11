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
void free_seg_mem();
int mem_len();
int word_seq_len(int seg_index);
uint32_t get_word(int s_ind, int w_ind);
void set_word(int s_ind, int w_ind, uint32_t value);

#endif
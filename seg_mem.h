/*seg_mem.h */
#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <seq.h>
#include <inttypes.h>


void init_mem(Seq_T program_words);
void free_seg_mem();
int mem_len();
int word_seq_len(uint32_t seg_index);
uint32_t get_word(uint32_t seg_index, uint32_t word_index);
void set_word(uint32_t seg_index, uint32_t word_index, uint32_t value);
void map_new_seg(uint32_t *reg, uint32_t word_seq_len);
void unmap_seg(uint32_t seg_index);
Seq_T duplicate_seg(uint32_t seg_index);
void free_seg(uint32_t seg_index);
void set_seg(Seq_T pending_set, uint32_t seg_index);

#endif
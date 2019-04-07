/* seg_mem.c */

#include "seg_mem.h"

void init_mem(Seq_T program_words)
{
        mem.segments = Seq_new(0);
        mem.unmapped = Seq_new(0); 
        Seq_addhi(mem.segments, program_words);
}

Seq_T get_seg(int index)
{
        return (Seq_T)Seq_get(mem.segments, index);
}

uint32_t get_word(int s_ind, int w_ind)
{
        return *((uint32_t *)Seq_get(get_seg(s_ind), w_ind));
}
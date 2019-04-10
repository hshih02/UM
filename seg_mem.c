/* seg_mem.c */

#include "seg_mem.h"

void init_mem(Seq_T program_words)
{
        mem.segments = Seq_new(0);
        mem.unmapped = Seq_new(0); 
        Seq_addhi(mem.segments, program_words);
}

void free_seg_mem()
{
        int length;
        Seq_T pending_free;

        length = Seq_length(mem.segments);
        while(length != 0)
        {
                pending_free = (Seq_T)(Seq_remhi(mem.segments));
                Seq_free(&pending_free);
                length = Seq_length(mem.segments);
        }
        Seq_free(&(mem.segments));

        length = Seq_length(mem.unmapped);
        while(length != 0)
        {
                pending_free = (Seq_T)(Seq_remhi(mem.unmapped));
                Seq_free(&pending_free);
                length = Seq_length(mem.unmapped);
        }
        Seq_free(&(mem.unmapped));
}

int mem_len()
{
        return Seq_length(mem.segments);
}

static Seq_T get_seg(int index)
{
        return ((Seq_T)Seq_get(mem.segments, index));
}


int word_seq_len(int seg_index)
{
        Seq_T current_word_seq = get_seg(seg_index);
        return Seq_length(current_word_seq);
}

uint32_t get_word(int s_ind, int w_ind)
{
        Seq_T theseg = get_seg(s_ind);
        uint32_t target_word = (uint32_t)(uintptr_t)(Seq_get(theseg, w_ind));

        return target_word;
}


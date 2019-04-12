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

        // length = Seq_length(mem.unmapped);
        // while(length != 0)
        // {
        //         pending_free = (Seq_T)(Seq_remhi(mem.unmapped));
        //         Seq_free(&pending_free);
        //         length = Seq_length(mem.unmapped);
        // }
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

uint32_t get_word(int seg_index, int word_index)
{
        Seq_T target_seg = get_seg(seg_index);
        uint32_t target_word = (uint32_t)(uintptr_t)
                               (Seq_get(target_seg, word_index));
        return target_word;
}

void set_word(int seg_index, int word_index, uint32_t value)
{        
        Seq_put(get_seg(seg_index), word_index, (void *)(uintptr_t)value);
}

void map_new_seg(uint32_t *reg, uint32_t word_seq_len)
{
        int unmapped_length = Seq_length(mem.unmapped);
        Seq_T new_seg = Seq_new(word_seq_len);
        uint32_t init_word = 0;
        uint32_t i;

        for (i = 0; i < word_seq_len; i++) {
                Seq_addhi(new_seg, (void *)(uintptr_t)init_word);
        }

        if (unmapped_length != 0) {
                uint32_t unmapped_index;
                unmapped_index = (uint32_t)(uintptr_t)
                                 (Seq_remhi(mem.unmapped));
                *reg = unmapped_index;
                Seq_put(mem.segments, unmapped_index,
                         (void *)(uintptr_t)new_seg);
        } else {
                Seq_addhi(mem.segments, new_seg);
                int new_seg_index = Seq_length(mem.segments);
                *reg = new_seg_index - 1;
        }
}

void unmap_seg(uint32_t seg_index)
{
        Seq_T pending_unmap;
        pending_unmap = get_seg(seg_index);

        Seq_free(&pending_unmap);
        Seq_addhi(mem.unmapped, (void*)(uintptr_t)seg_index);
}
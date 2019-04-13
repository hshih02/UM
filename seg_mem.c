/**************************************************************
 *
 *                     seg_mem.c
 *
 *     Assignment: HW6
 *     Authors:  Henning Shih, Ryan Hoff 
 *     Date:     Apr 11, 2019
 *     
 *     Segmented memory module utilizing Hanson sequences
 *
 **************************************************************/

#include "seg_mem.h"

static struct mem {
        Seq_T segments;
        Seq_T unmapped;
} mem;


static Seq_T get_seg(uint32_t index)
{
        return ((Seq_T)Seq_get(mem.segments, index));
}

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
                if (pending_free != NULL) {
                        Seq_free(&pending_free);
                }
                length = Seq_length(mem.segments);

        }
        Seq_free(&(mem.segments));
        Seq_free(&(mem.unmapped));
}

int mem_len()
{
        return Seq_length(mem.segments);
}

int word_seq_len(uint32_t seg_index)
{
        Seq_T current_word_seq = get_seg(seg_index);
        return Seq_length(current_word_seq);
}

uint32_t get_word(uint32_t seg_index, uint32_t word_index)
{
        Seq_T target_seg = get_seg(seg_index);
        uint32_t target_word = (uint32_t)(uintptr_t)
                               (Seq_get(target_seg, word_index));
        return target_word;
}

void set_word(uint32_t seg_index, uint32_t word_index, uint32_t value)
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
        Seq_put(mem.segments, seg_index, NULL);
        Seq_addhi(mem.unmapped, (void*)(uintptr_t)seg_index);
}

void free_seg(uint32_t seg_index)
{
        Seq_T pending_free = get_seg(seg_index);
        Seq_free(&pending_free);
}

void set_seg(Seq_T pending_set, uint32_t seg_index)
{
        Seq_put(mem.segments, seg_index, (void *)(uintptr_t)pending_set);
}

Seq_T duplicate_seg(uint32_t seg_index)
{
        Seq_T target_seg = get_seg(seg_index);
        int length = Seq_length(target_seg);

        Seq_T duplicate = Seq_new(length);

        int i;

        for(i = 0; i < length; i++) {
                uintptr_t target_element = (uintptr_t)Seq_get(target_seg, i);
                Seq_addhi(duplicate, (void *)target_element);
        }

        return duplicate;
}
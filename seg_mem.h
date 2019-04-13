/**************************************************************
 *
 *                     seg_mem.h
 *
 *     Assignment: HW6
 *     Authors:  Henning Shih, Ryan Hoff 
 *     Date:     Apr 11, 2019
 *     
 *     Contains all relevant functions needed for segmented
 *     memory manipulation of a UM
 *
 **************************************************************/

#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <seq.h>
#include <inttypes.h>

/*
*       init_mem
*       Takes in a Seq_T corresponding to the UM program's instructions
*       and initializes memory and registers to initial run state
*/
void init_mem(Seq_T program_words);

/*
*       free_seg_mem
*       Calls Seq_free on segmented memory and the Seq of unmapped segments
*/
void free_seg_mem();

/*
*       mem_len
*       Returns an int corresponding to total memory segments      
*/
int mem_len();

/*
*       word_seq_len
*       Takes in a uint32_t seg_index and returns int corresponding
*       to the length of the word sequence at $m[seg_index]
*/
int word_seq_len(uint32_t seg_index);

/*
*       get_word
*       Returns a uint32_t word located at $m[seg_index][word_index]
*       
*/
uint32_t get_word(uint32_t seg_index, uint32_t word_index);

/*
*       set_word
*       Calls Seq_put and inserts specified uint32_t value
*       at $m[seg_index][word_index] and 
*/
void set_word(uint32_t seg_index, uint32_t word_index, uint32_t value);

/*
*       map_new_seg
*       Creates a new memory segment via Seq_addhi with a total of
*       (word_seq_len) words, initialized to 0. Index of new segment
*       stored in register passed in from *reg
*/
void map_new_seg(uint32_t *reg, uint32_t word_seq_len);

/*
*       unmap_seg
*       Frees elements in word sequence at (seg_index) and sets 
*       $m[seg_index] to point to NULL, adds a new element to
*       unmapped sequence holding the value equivalent to seg_index
*/
void unmap_seg(uint32_t seg_index);

/*
*       duplicate_seg
*       Returns a deep copy of $m[seg_index] and returns it as a
*       Seq_T
*/
Seq_T duplicate_seg(uint32_t seg_index);

/*
*       free_seg
*       Calls Seq_free on $m[seg_index]
*/
void free_seg(uint32_t seg_index);

/*
*       set_seg
*       Calls Seq_put on $m[seg_index] and inserts a Seq_T        
*/
void set_seg(Seq_T pending_set, uint32_t seg_index);

#endif
/* process.h */

#include <stdlib.h>
#include <stdio.h>
#include <uarray.h>
#include "seg_mem.h"
#include "bitpack.h"
#include <inttypes.h>

#define num_regs 8

uint32_t regs[num_regs];

void init_reg();

void initialize(Seq_T program_words);

void um_run(Seq_T program_words);

uint32_t parse_op(uint32_t word);
uint32_t parse_regA(uint32_t word);
uint32_t parse_regB(uint32_t word);
uint32_t parse_regC(uint32_t word);
uint32_t parse13_regA(uint32_t word);
uint32_t parse13_value(uint32_t word);
void run_instruction(uint32_t word);
uintptr_t set_ctr(int curr_seg_index, int curr_word_index, uintptr_t prg_ctr);
/* process.h */

#include <stdlib.h>
#include <stdio.h>
#include <uarray.h>
#include "seg_mem.h"
#include "bitpack.h"
#include <inttypes.h>

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

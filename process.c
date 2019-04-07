/* process.c */

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "seg_mem.h"

void init_reg()
{
        int num_registers = 8;
        int size = 4;
        UArray_new(num_registers, size);
}

void initialize(Seq_T program_words)
{
        init_mem(program_words);
        init_reg();
}

void um_run(Seq_T program_words)
{
        initialize(program_words);
        uint32_t *prg_ctr = (uint32_t *)get_word(0, 0);
        (void)*prg_ctr;
}
/* process.c */

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "seg_mem.h"
#include <bitpack.h>

static uint32_t regs[num_regs];

static inline uint32_t parse_op(uint32_t word)
{
        return Bitpack_getu(word, 4, 28);
}

static inline uint32_t parse_regA(uint32_t word)
{
        return Bitpack_getu(word, 3, 6);
}

static inline uint32_t parse_regB(uint32_t word)
{
        return Bitpack_getu(word, 3, 3);
}

static inline uint32_t parse_regC(uint32_t word)
{
        return Bitpack_getu(word, 3, 0);
}

static inline uint32_t parse13_regA(uint32_t word)
{
        return Bitpack_getu(word, 3, 25);
}

static inline uint32_t parse13_value(uint32_t word)
{
        return Bitpack_getu(word, 25, 0);
}


static void set_ctr(uint32_t tgt_seg_index,   uint32_t tgt_word_index,
                    uint32_t *curr_seg_index, uint32_t *curr_word_index, 
                                                      uint32_t *prg_ctr)
{
        *curr_seg_index = tgt_seg_index;
        *curr_word_index = tgt_word_index;
        *prg_ctr = (uint32_t)(uintptr_t)get_word(*curr_seg_index, 
                                                 *curr_word_index);
}

static void adv_ctr(uint32_t curr_seg_index, uint32_t *curr_word_index, 
                                                     uint32_t *prg_ctr)
{
        (*curr_word_index)++;
        *prg_ctr = (uintptr_t)get_word(curr_seg_index, *curr_word_index);
}

static void init_reg()
{
        uint32_t i;
        for (i = 0; i < num_regs; i++) {
                regs[i] = 0;
                // printf("reg %i: %x\n", i, regs[i]);
        }
}

static void initialize(Seq_T program_words)
{
        init_mem(program_words);
        init_reg();
}

static uint32_t run_instruction(uint32_t *prg_ctr, uint32_t *curr_seg_index, 
                                              uint32_t *curr_word_index)
{
        uint32_t ctr_flag = 0;
        uint32_t word = *prg_ctr;
        uint32_t opcode = parse_op(word);
        switch (opcode) {
                case 0: /* conditional move */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 0 cond move: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        if (regs[regC] != 0) {
                                regs[regA] = regs[regB];
                        }
                        break;
                }
                case 1: /* segmented load */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 1 seg load: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);

                        regs[regA] = get_word(regs[regB], regs[regC]);

                        break;
                }
                case 2: /* segmented store */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 2 seg store: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        set_word(regs[regA], regs[regB], regs[regC]);

                        break;
                }
                case 3: /* addition */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 3 add: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        regs[regA] = (regs[regB] + regs[regC]);

                        break;
                }
                case 4: /* multiplication */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 4 multiply: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        regs[regA] = (regs[regB] * regs[regC]);
                        break;
                }
                case 5: /* division */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 5 div: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        regs[regA] = (regs[regB] / regs[regC]);
                        break;
                }
                case 6: /* bitwise NAND */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 6 NAND: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        regs[regA] = ~(regs[regB] & regs[regC]);
                        break;
                }
                case 7: /* halt */
                {
                        free_seg_mem();
                        exit(EXIT_SUCCESS);
                        break;
                }
                case 8: /* map segment */
                {
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        // printf("OP 8 map: regB: %x, regC: %x\n", regB, regC);

                        map_new_seg(&(regs[regB]), regs[regC]);

                        break;
                }
                case 9: /* unmap segment */
                {
                        uint32_t regC = parse_regC(word);
                        // printf("OP 9 unmap: regC: %x\n", regC);

                        unmap_seg(regs[regC]);
                        
                        break;
                }
                case 10: /* output */
                {
                        uint32_t regC = parse_regC(word);
                        // printf("%u", regs[regC]); /* for actual output */
                        putchar(regs[regC]);
                        break;
                }
                case 11: /* input */
                {
                        uint32_t regC = parse_regC(word);

                        int input; 
                        input = fgetc(stdin);
                        
                        if (input == EOF) {
                                regs[regC] = (uint32_t)input;
                        } else {
                                regs[regC] = (char)input;
                        }
                        break;
                }
                case 12: /* load prog */
                {
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);

                        if(regs[regB] != 0) {
                                Seq_T pending_load = duplicate_seg(regs[regB]);
                                free_seg(0);
                                set_seg(pending_load, 0);
                        }

                        set_ctr(0, regs[regC], 
                                curr_seg_index, 
                                curr_word_index, 
                                prg_ctr);

                        ctr_flag = 1;
                        // printf("OP 12 loadprog: regB: %x, regC: %x\n", regB, regC);

                        break;
                }
                case 13: /* load val */
                {
                        uint32_t regA = parse13_regA(word);
                        uint32_t value = parse13_value(word);
                        // printf("OP 13 loadval: regA: %x, value = %u\n", regA, value);
                        
                        regs[regA] = value;

                        break;
                }
        }

        return ctr_flag;
}

void um_run(Seq_T program_words)
{
        uint32_t curr_seg_index;
        uint32_t curr_word_index;
        uint32_t total_segments;
        uint32_t curr_word_seq_len;
        uint32_t prg_ctr;
        uint32_t ctr_modded;

        initialize(program_words);

        curr_seg_index = 0;
        total_segments = mem_len();
        prg_ctr = (uint32_t)(uintptr_t)get_word(0, 0);
        curr_word_index = 0;
        ctr_modded = 0;

        while (curr_seg_index < total_segments) {
                curr_word_seq_len = word_seq_len(curr_seg_index);

                while (curr_word_index < curr_word_seq_len) {
                        ctr_modded = run_instruction(&prg_ctr, 
                                                     &curr_seg_index, 
                                                     &curr_word_index);
                        total_segments = mem_len();

                        if (curr_word_index < curr_word_seq_len - 1 
                                                 && ctr_modded == 0) {
                                // curr_word_index++;
                                // set_ctr(curr_seg_index, curr_word_index);
                                adv_ctr(curr_seg_index, &curr_word_index,
                                                        &prg_ctr);
                        } else {
                                curr_word_seq_len = 
                                                  word_seq_len(curr_seg_index);
                                ctr_modded = 0;
                                break;
                        }
                }
                curr_seg_index++;
        }
}



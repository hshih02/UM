/* process.c */

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "seg_mem.h"
#include <bitpack.h>

void um_run(Seq_T program_words)
{
        int curr_seg_index;
        int total_segments;
        uintptr_t prg_ctr;
        int curr_word_index;
        int curr_word_seq_len;

        initialize(program_words);
        // printf("seg0w0: %x\n", get_word(0, 0));
        // printf("seg0w1: %x\n", get_word(0, 1));
        // printf("seg0w2: %x\n", get_word(0, 2));
        // printf("seg0w3: %x\n", get_word(0, 3));
        // printf("seg0w4: %x\n", get_word(0, 4));

        curr_seg_index = 0;
        total_segments = mem_len();
        prg_ctr = (uintptr_t)get_word(0, 0);
        curr_word_index = 0;

        while (curr_seg_index < total_segments) {
                curr_word_seq_len = word_seq_len(curr_seg_index);

                while (curr_word_index < curr_word_seq_len) {
                        run_instruction((uint32_t)prg_ctr);
                        total_segments = mem_len();
                        prg_ctr = adv_ctr(curr_seg_index, &curr_word_index, prg_ctr);
                }
                curr_seg_index++;
        }

        // prg_ctr = (uintptr_t)get_word(0, 0);
        // run_instruction((uint32_t)prg_ctr);
        // prg_ctr = (uintptr_t)get_word(0, 1);
        // run_instruction((uint32_t)prg_ctr);
        // prg_ctr = (uintptr_t)get_word(0, 2);
        // run_instruction((uint32_t)prg_ctr);
        // prg_ctr = (uintptr_t)get_word(0, 3);
        // run_instruction((uint32_t)prg_ctr);
        // prg_ctr = (uintptr_t)get_word(0, 4);
        // run_instruction((uint32_t)prg_ctr);
}

void initialize(Seq_T program_words)
{
        init_mem(program_words);
        init_reg();
}

void init_reg()
{
        int num_registers = 8;
        uint32_t registers[num_registers];
        int i;
        for (i = 0; i < num_registers; i++) {
                registers[i] = 0;
                printf("reg %i: %x\n", i, registers[i]);
        }
}

void run_instruction(uint32_t word)
{
        printf("RUNNING INSTRUCTION\n");
        uint32_t opcode = parse_op(word);
        printf("parsed opcode: %x\n", opcode);
        switch (opcode) {
                case 0: /* conditional move */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 0 cond move: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 1: /* segmented load */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 1 seg load: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 2: /* segmented store */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 2 seg store: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 3: /* addition */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 3 add: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 4: /* multiplication */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 4 multiply: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 5: /* division */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 5 div: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 6: /* bitwise NAND */
                {
                        uint32_t regA = parse_regA(word);
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 6 NAND: regA: %x, regB: %x, regC: %x\n", regA, regB, regC);
                        break;
                }
                case 7: /* halt */
                {
                        printf("HALT\n");
                        exit(EXIT_SUCCESS);
                }
                case 8: /* map segment */
                {
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 8 map: regB: %x, regC: %x\n", regB, regC);
                        break;
                }
                case 9: /* unmap segment */
                {
                        uint32_t regC = parse_regC(word);
                        (void)regC;
                        break;
                }
                case 10: /* output */
                {
                        uint32_t regC = parse_regC(word);
                        printf("OP 10 output: regC: %x\n", regC);
                        break;
                }
                case 11: /* input */
                {
                        uint32_t regC = parse_regC(word);
                        (void)regC;
                        break;
                }
                case 12: /* load prog */
                {
                        uint32_t regB = parse_regB(word);
                        uint32_t regC = parse_regC(word);
                        printf("OP 12 loadprog: regB: %x, regC: %x\n", regB, regC);
                        break;
                }
                case 13: /* load val */
                {
                        uint32_t regA = parse13_regA(word);
                        uint32_t value = parse13_value(word);
                        printf("OP 13 loadval: regA: %x, value = %u\n", regA, value);
                        break;
                }
        }
}

uint32_t parse_op(uint32_t word)
{
        return Bitpack_getu(word, 4, 28);
}

uint32_t parse_regA(uint32_t word)
{
        return Bitpack_getu(word, 3, 6);
}

uint32_t parse_regB(uint32_t word)
{
        return Bitpack_getu(word, 3, 3);
}

uint32_t parse_regC(uint32_t word)
{
        return Bitpack_getu(word, 3, 0);
}

uint32_t parse13_regA(uint32_t word)
{
        return Bitpack_getu(word, 3, 25);
}

uint32_t parse13_value(uint32_t word)
{
        return Bitpack_getu(word, 25, 0);
}

uintptr_t adv_ctr(int curr_seg_index, int *curr_word_index, uintptr_t prg_ctr)
{
        (*curr_word_index)++;
        prg_ctr = (uintptr_t)get_word(curr_seg_index,
                                      *curr_word_index);
        return prg_ctr;
}
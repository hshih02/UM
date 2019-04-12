/* um.c */

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include <assert.h>
#include <inttypes.h>

// #define next_lsb(lsb) (lsb <= 0 ? 24 : (lsb - 8))

// Seq_T read_file(FILE *fp)
// {
//         Seq_T program_words = Seq_new(0);
//         int byte = fgetc(fp);
//         int width = 8;
//         int lsb = 0;
//         uint32_t instr = 0;
//         while (byte != EOF) {
//                 lsb = next_lsb(lsb);
//                 instr = Bitpack_newu(instr, width, lsb, (uint32_t)byte);
//                 if (lsb == 0)
//                         Seq_addhi(program_words, (void *)(uintptr_t)instr);
//                 byte = fgetc(fp);
//         }
//         if (lsb != 0) {
//                 fprintf(stderr, "Invalid UM file!\n");
//                 exit(EXIT_FAILURE);
//         }
//         return program_words;
// }


Seq_T read_file(FILE *fp)
{
        Seq_T program_words = Seq_new(0);
        while (1) {
                uint32_t instruction = 0;
                uint32_t byte = fgetc(fp);
                if ((int)byte == EOF) {
                        break;
                }
                instruction = Bitpack_newu(instruction, 8, 24, byte);
                byte = fgetc(fp);
                if ((int)byte == EOF) {
                        break;
                }
                instruction = Bitpack_newu(instruction, 8, 16, byte);
                byte = fgetc(fp);
                if ((int)byte == EOF) {
                        break;
                }
                instruction = Bitpack_newu(instruction, 8, 8, byte);
                byte = fgetc(fp);
                if ((int)byte == EOF) {
                        break;
                }
                instruction = Bitpack_newu(instruction, 8, 0, byte);
                // printf ("read word: %u\n", instruction);
                Seq_addhi(program_words, (void *)(uintptr_t)instruction);
        }
        return program_words;
        // printf("Inst 0: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 0))));
}


int main(int argc, char *argv[])
{
        assert(argc < 3);    /* at most one file on command line */
        Seq_T program_words;
        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                program_words = read_file(fp);
                fclose(fp);
        } else {
                program_words = read_file(stdin);
        }
        um_run(program_words);
}
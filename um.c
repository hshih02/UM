/* um.c */

#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include <bitpack.h>
#include <assert.h>
#include <inttypes.h>

void run(FILE *fp)
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
        printf("Inst 0: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 0))));
        printf("Inst 1: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 1))));
        printf("Inst 2: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 2))));
        printf("Inst 3: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 3))));
        printf("Inst 4: %x\n", (uint32_t)((uintptr_t)(Seq_get(program_words, 4))));
}

int main(int argc, char *argv[])
{
        assert(argc < 3);    /* at most one file on command line */
        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                run(fp);
                fclose(fp);
        } else {
                run(stdin);
        }
}
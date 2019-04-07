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
                instruction = Bitpack_newu(instruction, 8, 16, byte);
                instruction = Bitpack_newu(instruction, 8, 8, byte);
                instruction = Bitpack_newu(instruction, 8, 0, byte);
                Seq_addhi(program_words, (void *)(uintptr_t)instruction);
        }
}

int main(int argc, char *argv[])
{
        int i = 0;
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                run(fp);
                fclose(fp);
        } else {
                run(stdin);
        }
}
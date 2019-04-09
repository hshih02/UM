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
/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 6, ra);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 0, rc);

        // printf("%x\n", word);
        
        return word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, LV);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);

        // printf("loadval word: %x\n", word);

        return word;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        // printf("HALT word:\n");
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction map(Um_register b, Um_register c) 
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction loadprog(Um_register b, Um_register c) 
{
        return three_register(LOADP, 0, b, c);
}

static inline Um_instruction unmap(Um_register c) 
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mult(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

Um_instruction output(Um_register c)
{
        // printf("OUTPUT word:\n");
        return three_register(OUT, 0, 0, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream)
{
        while (Seq_length(stream) != 0)
        {
                Um_instruction word = (uintptr_t)Seq_remlo(stream);
                char BE_byte4 = Bitpack_getu(word, 8, 0);  /*1st byte in LE*/
                char BE_byte3 = Bitpack_getu(word, 8, 8);  /*2nd byte in LE*/
                char BE_byte2 = Bitpack_getu(word, 8, 16); /*3rd byte in LE*/
                char BE_byte1 = Bitpack_getu(word, 8, 24); /*4th byte in LE*/

                fprintf(output, "%c", BE_byte1);
                fprintf(output, "%c", BE_byte2);
                fprintf(output, "%c", BE_byte3);
                fprintf(output, "%c", BE_byte4);
        }
}

/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_add_test(Seq_T stream)
{
        emit(stream, add(r1, r2, r3));
        emit(stream, halt());
}

void emit_print_six_test(Seq_T stream)
{
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r2, 6));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_output_test(Seq_T stream)
{
        emit(stream, loadval(r1, 58));
        emit(stream, loadval(r2, 41));
        emit(stream, loadval(r3, 45));
        emit(stream, output(r1));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_cmov_test(Seq_T stream)
{
        emit(stream, loadval(r1, 61));
        emit(stream, loadval(r2, 79));
        emit(stream, loadval(r3, 95));
        emit(stream, three_register(CMOV, r1, r2 ,r3));
        emit(stream, output(r1));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_sstore_sload_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 81));
        emit(stream, loadval(r3, 77));
        emit(stream, loadval(r4, 0));
        emit(stream, loadval(r5, 13));
        emit(stream, three_register(SLOAD, r1, r4, r5));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, loadval(r6, 6));
        emit(stream, loadval(r2, 12));
        emit(stream, three_register(SLOAD, r7, r4, r6));
        emit(stream, three_register(SSTORE, r4, r2, r7));
        emit(stream, output(r7));
        emit(stream, halt());
}

void emit_multiply_test(Seq_T stream)
{
        emit(stream, loadval(r1, 10));
        emit(stream, loadval(r2, 6));
        emit(stream, mult(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_divide_test(Seq_T stream)
{
        emit(stream, loadval(r1, 200));
        emit(stream, loadval(r2, 2));
        emit(stream, div(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        emit(stream, loadval(r1, 65502));
        emit(stream, loadval(r2, 65463));
        emit(stream, nand(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_mapunmap_test(Seq_T stream)
{
        emit(stream, loadval(r1, 0));  //r1 = 0
        emit(stream, loadval(r2, 4));  //r2 = 4
        emit(stream, map(r5, r2));     //map len4 word seq to $m[1], r5 = 1
        emit(stream, output(r5));  //should be 1
        emit(stream, loadval(r3, 1));  //r3 = 1
        emit(stream, unmap(r3));   //unmap $m[1]
        emit(stream, map(r4, r2));  //map len4 word seq back to $m[1], r4 = 1
        emit(stream, output(r4));  //r4 = 1
        emit(stream, map(r5, r2));  //map len4 word seq to $m[2], r5 = 2
        emit(stream, output(r5));  //r5 = 2 
        emit(stream, map(r5, r2)); //map len4 word seq to $m[3], r5 = 3
        emit(stream, output(r5));  //r5 = 3 
        emit(stream, map(r5, r2)); //map len4 word seq to $m[4], r5 = 4
        emit(stream, output(r5));  //r5 = 4 
        emit(stream, loadval(r3, 4));  //r3 = 4
        emit(stream, unmap(r3));   //unmap $m[4]
        emit(stream, map(r5, r2)); //map len4 word seq to $m[4], r5 = 4
        emit(stream, map(r5, r2)); //map len4 word seq to $m[5], r5 = 5
        emit(stream, output(r5));  //r5 = 5
        emit(stream, unmap(r3));   //unmap $m[4]
        emit(stream, loadval(r3, 2)); //r3 = 2
        emit(stream, unmap(r3));   //unmap $m[2]
        emit(stream, map(r5, r2)); //map len4 word seq to $m[2], r5 = 2
        emit(stream, output(r5));  //r5 = 2
        emit(stream, map(r5, r2)); //map len4 word seq to $m[4], r5 = 4
        emit(stream, output(r5));  //r5 = 4
        emit(stream, map(r5, r2)); //map len4 word seq to $m[6], r5 = 6
        emit(stream, output(r5));  //r5 = 6
        emit(stream, halt());   //expected output: 112345246
}

void emit_loadprog_test(Seq_T stream)
{
        emit(stream, loadval(r1, 0));  //r1 = 0
        emit(stream, loadval(r2, 12));  //r2 = 12
        emit(stream, loadval(r3, 0));  //r3 = 0
        emit(stream, map(r5, r2));     //map len11 word seq to $m[1], r5 = 1
        emit(stream, loadprog(r3, r2));
        emit(stream, output(r5));  //should be 1
        emit(stream, loadval(r3, 1));  //r3 = 1
        emit(stream, unmap(r3));   //unmap $m[1]
        emit(stream, map(r4, r2));  //map len4 word seq back to $m[1], r4 = 1
        emit(stream, loadval (r5, 64));
        emit(stream, loadprog(r3, r2));
        emit(stream, halt());
        emit(stream, loadval (r5, 126));
        emit(stream, output(r5)); 
        emit(stream, halt());
}
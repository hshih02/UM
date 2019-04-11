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

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
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
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 3));
        emit(stream, output(r1));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_cmov_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 3));
        emit(stream, three_register(CMOV, r1, r2 ,r3));
        emit(stream, output(r1));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, halt());
}

void emit_sstore_sload_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 77));
        emit(stream, loadval(r4, 0));
        emit(stream, loadval(r5, 14));
        emit(stream, three_register(SLOAD, r1, r4, r5));
        emit(stream, output(r1));
        emit(stream, output(r3));
        emit(stream, output(r2));
        emit(stream, loadval(r6, 7));
        emit(stream, loadval(r2, 13));
        emit(stream, three_register(SLOAD, r7, r4, r6));
        emit(stream, three_register(SSTORE, r4, r2, r7));
        emit(stream, output(r7));
        emit(stream, halt());
}
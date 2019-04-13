/**************************************************************
 *
 *                       process.h
 *
 *     Assignment: HW6
 *     Authors:  Henning Shih, Ryan Hoff 
 *     Date:     Apr 11, 2019
 *     
 *     Includes all needed libraries for the UM
 *
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "seg_mem.h"
#include <inttypes.h>
#include <bitpack.h>

/*
*       um_run
*       primary UM function, takes in a Seq_T corresponding
*       to all UM instructions 
*/
void um_run(Seq_T program_words);
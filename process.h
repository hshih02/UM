/* process.h */

#include <stdlib.h>
#include <stdio.h>
#include <uarray.h>
#include "seg_mem.h"
#include <inttypes.h>

void init_reg();

void initialize(Seq_T program_words);

void um_run(Seq_T program_words);
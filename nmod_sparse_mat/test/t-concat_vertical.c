/*
    Copyright (C) 2015 Elena Sergeicheva

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "nmod_vec.h"
#include "nmod_sparse_mat.h"
#include "ulong_extras.h"

int main(void)
{
    nmod_sparse_mat_t A, B, C;
    nmod_sparse_mat_t window1, window2;
    slong i;
    FLINT_TEST_INIT(state);


    flint_printf("concat_vertical....");
    fflush(stdout);

    for (i = 0; i < 100; i++)
    {
        slong r1, r2, c1, mod;

        r1 = n_randint(state, 50);
        r2 = n_randint(state, 50);
        do
            mod = n_randlimb(state);
        while(mod <= 1);

        nmod_sparse_mat_init(A, r1, mod);
        nmod_sparse_mat_init(B, r2, mod);
        nmod_sparse_mat_init(C, (r1+r2), mod);

        nmod_sparse_mat_randtest(A, state);
        nmod_sparse_mat_randtest(B, state);
        nmod_sparse_mat_randtest(C, state);

        nmod_sparse_mat_concat_vertical(C, A, B);
        
        nmod_sparse_mat_window_init(window1, C, 0, 0, r1, C->c);
        nmod_sparse_mat_window_init(window2, C, r1, 0, (r1+r2), C->c);

        if (!(nmod_sparse_mat_equal(window1, A) && nmod_sparse_mat_equal(window2, B)))
        {
            flint_printf("A = \n");
            nmod_sparse_mat_print_pretty(A);
            flint_printf("B = \n");
            nmod_sparse_mat_print_pretty(B);
            flint_printf("A concat_vertical B = \n");
            nmod_sparse_mat_print_pretty(C);
            flint_printf("FAIL: results not equal\n");
            abort();
        }
        
        nmod_sparse_mat_clear(A);
        nmod_sparse_mat_clear(B);
        nmod_sparse_mat_clear(C);

        nmod_sparse_mat_window_clear(window1);
        nmod_sparse_mat_window_clear(window2);
    }


    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
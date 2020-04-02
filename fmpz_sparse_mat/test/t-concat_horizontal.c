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
#include <limits.h>
#include "flint.h"
#include "fmpz_sparse_mat.h"

int main(void)
{
    slong rep, bits, r, c1, c2, nreps = 100;
    fmpz_sparse_mat_t A, B, C;
    fmpz_sparse_mat_t window1, window2;
    FLINT_TEST_INIT(state);

    flint_printf("concat_horizontal....");
    fflush(stdout);


    for (rep = 0; rep < nreps; rep++)
    {
        do bits = n_randint(state, 200);
        while (bits < UWORD(2));
        r = n_randint(state, 20);
        c1 = n_randint(state, 20);
        c2 = n_randint(state, 20);
        fmpz_sparse_mat_init(A, r, c1);
        fmpz_sparse_mat_init(B, r, c2);
        fmpz_sparse_mat_init(C, r, c1+c2);

        fmpz_sparse_mat_randtest(A, state, 0, c1, bits);
        fmpz_sparse_mat_randtest(B, state, 0, c2, bits);
        fmpz_sparse_mat_randtest(C, state, 0, c1+c2, bits);

        fmpz_sparse_mat_concat_horizontal(C, A, B);        
        
        fmpz_sparse_mat_window_init(window1, C, 0, 0, r, c1);
        fmpz_sparse_mat_window_init(window2, C, 0, c1, r, c1+c2);

        if (!fmpz_sparse_mat_equal(window1, A) || !fmpz_sparse_mat_equal(window2, B))
        {
            flint_printf("A = \n");
            fmpz_sparse_mat_print_pretty(A);
            flint_printf("B = \n");
            fmpz_sparse_mat_print_pretty(B);
            flint_printf("A | B = \n");
            fmpz_sparse_mat_print_pretty(C);
            flint_printf("window1 = \n");
            fmpz_sparse_mat_print_pretty(window1);
            flint_printf("window2 = \n");
            fmpz_sparse_mat_print_pretty(window2);
            flint_printf("FAIL: window 2 not equal\n");
            abort();
        }

        fmpz_sparse_mat_window_clear(window1);
        fmpz_sparse_mat_window_clear(window2);

        fmpz_sparse_mat_init(window1, r, c1);
        fmpz_sparse_mat_init(window2, r, c2);
        fmpz_sparse_mat_split_horizontal(window1, window2, C, c1);

       if (!(fmpz_sparse_mat_equal(window1, A) && fmpz_sparse_mat_equal(window2, B)))
        {
            flint_printf("A = \n");
            fmpz_sparse_mat_print_pretty(A);
            flint_printf("B = \n");
            fmpz_sparse_mat_print_pretty(B);
            flint_printf("A | B = \n");
            fmpz_sparse_mat_print_pretty(C);
            flint_printf("window1 = \n");
            fmpz_sparse_mat_print_pretty(window1);
            flint_printf("window2 = \n");
            fmpz_sparse_mat_print_pretty(window2);
            flint_printf("FAIL: results not equal\n");
            abort();
        }

        fmpz_sparse_mat_clear(window1);
        fmpz_sparse_mat_clear(window2);

        fmpz_sparse_mat_clear(A);
        fmpz_sparse_mat_clear(B);
        fmpz_sparse_mat_clear(C);
    }


    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
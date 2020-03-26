/*
    Copyright (C) 2011 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <gmp.h>
#include "flint.h"
#include "nmod_sparse_mat.h"
#include "nmod_vec.h"
#include "ulong_extras.h"

int
main(void)
{
    slong m, n, mod, rep;
    FLINT_TEST_INIT(state);
    

    flint_printf("scalar_mul....");
    fflush(stdout);

    for (rep = 0; rep < 1000; rep++)
    {
        nmod_sparse_mat_t A, B, C, D;
        mp_limb_t c;

        m = n_randint(state, 200);
        do
            mod = n_randtest_not_zero(state);
        while(mod <= UWORD(1));
        
        c = n_randint(state, mod);
        nmod_sparse_mat_init(A, m, mod);
        nmod_sparse_mat_init(B, m, mod);
        nmod_sparse_mat_init(C, m, mod);
        nmod_sparse_mat_init(D, m, mod);

        nmod_sparse_mat_randtest(A, state);

        nmod_sparse_mat_scalar_mul(B, A, c);
        nmod_sparse_mat_scalar_mul(C, A, nmod_sub(c, UWORD(1), A->mod));

        /* c*A - (c-1)*A == A */
        nmod_sparse_mat_sub(D, B, C);

        if (!nmod_sparse_mat_equal(A, D))
        {
            flint_printf("FAIL\n");
            abort();
        }

         /* Aliasing */
        nmod_sparse_mat_scalar_mul(C, A, c);
        nmod_sparse_mat_scalar_mul(A, A, c);

        if (!nmod_sparse_mat_equal(A, C))
        {
            flint_printf("FAIL\n");
            abort();
        }

        nmod_sparse_mat_clear(A);
        nmod_sparse_mat_clear(B);
        nmod_sparse_mat_clear(C);
        nmod_sparse_mat_clear(D);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
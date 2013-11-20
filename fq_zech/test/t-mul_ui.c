/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Mike Hansen

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fq_zech.h"
#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int j, i, result;
    flint_rand_t state;
    fq_zech_ctx_t ctx;

    flint_printf("mul_ui... ");
    fflush(stdout);

    flint_randinit(state);

    for (j = 0; j < 50; j++)
    {
        fq_zech_ctx_randtest(ctx, state);

        for (i = 0; i < 200; i++)
        {
            mp_limb_t x;
            fq_nmod_t aa, bb;
            fq_zech_t a, b, c;

            fq_nmod_init(aa, ctx->fq_nmod_ctx);
            fq_nmod_init(bb, ctx->fq_nmod_ctx);

            x = z_randtest(state);

            fq_nmod_randtest(aa, state, ctx->fq_nmod_ctx);
            fq_zech_set_fq_nmod(a, aa, ctx);

            fq_nmod_mul_ui(bb, aa, x, ctx->fq_nmod_ctx);
            fq_zech_set_fq_nmod(b, bb, ctx);

            fq_zech_mul_ui(c, a, x, ctx);

            result = (fq_zech_equal(b, c, ctx));
            if (!result)
            {
                flint_printf("FAIL:\n\n");
                fq_zech_ctx_print(ctx);
                flint_printf("\nx = %wu\n", x);
                flint_printf("aa = ");
                fq_nmod_print_pretty(aa, ctx->fq_nmod_ctx);
                flint_printf("\nbb = ");
                fq_nmod_print_pretty(bb, ctx->fq_nmod_ctx);
                flint_printf("\n");
                flint_printf("a = ");
                fq_zech_print_pretty(a, ctx);
                flint_printf("\n");
                flint_printf("b = ");
                fq_zech_print_pretty(b, ctx);
                flint_printf("\n");
                flint_printf("c = ");
                fq_zech_print_pretty(c, ctx);
                flint_printf("\n");
                abort();
            }

            fq_nmod_clear(bb, ctx->fq_nmod_ctx);
            fq_nmod_clear(aa, ctx->fq_nmod_ctx);
        }

        for (i = 0; i < 200; i++)
        {
            mp_limb_t x;
            fq_nmod_t aa, bb;
            fq_zech_t a, b;

            fq_nmod_init(aa, ctx->fq_nmod_ctx);
            fq_nmod_init(bb, ctx->fq_nmod_ctx);

            x = z_randtest(state);

            fq_nmod_randtest(aa, state, ctx->fq_nmod_ctx);
            fq_zech_set_fq_nmod(a, aa, ctx);

            fq_nmod_mul_ui(bb, aa, x, ctx->fq_nmod_ctx);
            fq_zech_set_fq_nmod(b, bb, ctx);

            fq_zech_mul_ui(a, a, x, ctx);

            result = (fq_zech_equal(b, a, ctx));
            if (!result)
            {
                flint_printf("FAIL:\n\n");
                fq_zech_ctx_print(ctx);
                flint_printf("\n");
                flint_printf("aa = ");
                fq_nmod_print_pretty(aa, ctx->fq_nmod_ctx);
                flint_printf("\n");
                flint_printf("a = ");
                fq_zech_print_pretty(a, ctx);
                flint_printf("\n");
                flint_printf("b = ");
                fq_zech_print_pretty(b, ctx);
                flint_printf("\n");
                abort();
            }

            fq_nmod_clear(bb, ctx->fq_nmod_ctx);
            fq_nmod_clear(aa, ctx->fq_nmod_ctx);
        }

        fq_zech_ctx_clear(ctx);
    }


    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

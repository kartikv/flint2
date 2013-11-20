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

    Copyright (C) 2012 Sebastian Pancratz 
    Copyright (C) 2012 Andres Goens
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

#include <stdio.h>
#include <stdlib.h>

#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("pow... ");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing  */
    for (i = 0; i < 200; i++)
    {
        long len;
        TEMPLATE(T, ctx_t) ctx;

        TEMPLATE(T, poly_t) a, b, c;
        ulong exp;

        len = n_randint(state, 15) + 1;
        TEMPLATE(T, ctx_randtest)(ctx, state);
        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(c, ctx);

        TEMPLATE(T, poly_randtest)(a, state, len, ctx);
        exp = n_randtest(state) % 20UL;
        TEMPLATE(T, poly_set)(b, a, ctx);
        TEMPLATE(T, poly_pow)(c, b, exp, ctx);
        TEMPLATE(T, poly_pow)(b, b, exp, ctx);

        result = (TEMPLATE(T, poly_equal)(b, c, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), TEMPLATE(T, poly_print_pretty)(a, "X", ctx), flint_printf("\n");
            flint_printf("b = "), TEMPLATE(T, poly_print_pretty)(b, "X", ctx), flint_printf("\n");
            flint_printf("c = "), TEMPLATE(T, poly_print_pretty)(c, "X", ctx), flint_printf("\n");
            flint_printf("exp = %wu\n", exp);
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(c, ctx);

        TEMPLATE(T, ctx_clear)(ctx);
    }

    /* Compare with repeated multiplications by the base */
    for (i = 0; i < 1000; i++)
    {
        long len;
        TEMPLATE(T, ctx_t) ctx;

        TEMPLATE(T, poly_t) a, b, c;
        ulong exp;

        len = n_randint(state, 15) + 1;
        TEMPLATE(T, ctx_randtest)(ctx, state);
        TEMPLATE(T, poly_init)(a, ctx);
        TEMPLATE(T, poly_init)(b, ctx);
        TEMPLATE(T, poly_init)(c, ctx);

        TEMPLATE(T, poly_randtest)(b, state, len, ctx);
        exp = n_randtest(state) % 20UL;

        TEMPLATE(T, poly_pow)(a, b, exp, ctx);

        if (exp == 0)
        {
            TEMPLATE(T, poly_one)(c, ctx);
        }
        else
        {
            long j;

            TEMPLATE(T, poly_set)(c, b, ctx);
            for (j = 1; j < exp; j++)
                TEMPLATE(T, poly_mul)(c, c, b, ctx);
        }

        result = (TEMPLATE(T, poly_equal)(a, c, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), TEMPLATE(T, poly_print_pretty)(a, "X", ctx), flint_printf("\n");
            flint_printf("b = "), TEMPLATE(T, poly_print_pretty)(b, "X", ctx), flint_printf("\n");
            flint_printf("c = "), TEMPLATE(T, poly_print_pretty)(c, "X", ctx), flint_printf("\n");
            flint_printf("exp = %wu\n", exp);
            TEMPLATE(T, ctx_print)(ctx);
            abort();
        }

        TEMPLATE(T, poly_clear)(a, ctx);
        TEMPLATE(T, poly_clear)(b, ctx);
        TEMPLATE(T, poly_clear)(c, ctx);

        TEMPLATE(T, ctx_clear)(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}



#endif

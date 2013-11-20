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
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

/*
    TODO:  When op has only one non-zero term, this 
    function would omit the parentheses.
 */

static void
__TEMPLATE(T, print)(FILE * file, const TEMPLATE(T, struct) * op, const TEMPLATE(T, ctx_t) ctx)
{
    fputc('(', file);
    TEMPLATE(T, fprint_pretty)(file, op, ctx);
    fputc(')', file);
}

int
_TEMPLATE(T, poly_fprint_pretty)(FILE * file, const TEMPLATE(T, struct) * poly, long len,
                       const char *x, const TEMPLATE(T, ctx_t) ctx)
{
    if (len == 0)
    {
        fputc('0', file);
    }
    else if (len == 1)
    {
        TEMPLATE(T, fprint_pretty)(file, poly + 0, ctx);
    }
    else if (len == 2)
    {
        if (TEMPLATE(T, is_one)(poly + 1, ctx))
            flint_fprintf(file, "%s", x);
        else
        {
            __TEMPLATE(T, print)(file, poly + 1, ctx);
            flint_fprintf(file, "*%s", x);
        }
        if (!TEMPLATE(T, is_zero)(poly + 0, ctx))
        {
            fputc('+', file);
            __TEMPLATE(T, print)(file, poly + 0, ctx);
        }
    }
    else
    {
        long i = len - 1;

        {
            if (TEMPLATE(T, is_one)(poly + i, ctx))
                flint_fprintf(file, "%s^%wd", x, i);
            else
            {
                __TEMPLATE(T, print)(file, poly + i, ctx);
                flint_fprintf(file, "*%s^%wd", x, i);
            }
            --i;
        }

        for (; i > 1; --i)
        {
            if (TEMPLATE(T, is_zero)(poly + i, ctx))
                continue;

            if (TEMPLATE(T, is_one)(poly + i, ctx))
                flint_fprintf(file, "+%s^%wd", x, i);
            else
            {
                fputc('+', file);
                __TEMPLATE(T, print)(file, poly + i, ctx);
                flint_fprintf(file, "*%s^%wd", x, i);
            }
        }

        if (!TEMPLATE(T, is_zero)(poly + 1, ctx))
        {
            if (TEMPLATE(T, is_one)(poly + 1, ctx))
            {
                fputc('+', file);
                fputs(x, file);
            }
            else
            {
                fputc('+', file);
                __TEMPLATE(T, print)(file, poly + 1, ctx);
                fputc('*', file);
                fputs(x, file);
            }
        }
        if (!TEMPLATE(T, is_zero)(poly + 0, ctx))
        {
            fputc('+', file);
            __TEMPLATE(T, print)(file, poly + 0, ctx);
        }
    }

    return 1;
}

int
TEMPLATE(T, poly_fprint_pretty)(FILE * file, const TEMPLATE(T, poly_t) poly, const char *x,
                      const TEMPLATE(T, ctx_t) ctx)
{
    return _TEMPLATE(T, poly_fprint_pretty)(file, poly->coeffs, poly->length, x, ctx);
}


#endif

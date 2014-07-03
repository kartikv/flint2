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

    Copyright (C) 2010 William Hart
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2014 Abhinav Baid

******************************************************************************/

#include "fmpz_lll.h"

void
fmpz_lll_randtest(fmpz_lll_t fl, flint_rand_t state)
{
    double randd, delta, eta;
    rep_type rt;
    gram_type gt;
    int store_trans;

    randd = d_randtest(state);
    if (randd > 0.5 && n_randint(state, 1))
    {
        delta = 0.25 + (randd - 0.5) * 0.75;
        if (n_randint(state, 1))
            eta = 0.5 + (randd - 0.5) * (sqrt(delta) - 0.5);
        else
            eta = 0.5 + randd * (sqrt(delta) - 0.5);
    }
    else
    {
        delta = 0.25 + randd * 0.75;
        if (n_randint(state, 1))
            eta = 0.5 + (randd - 0.5) * (sqrt(delta) - 0.5);
        else
            eta = 0.5 + randd * (sqrt(delta) - 0.5);
    }
    rt = n_randint(state, 2);
    gt = n_randint(state, 2);
    store_trans = n_randint(state, 2);
    fmpz_lll_context_init(fl, delta, eta, rt, gt, store_trans);
}

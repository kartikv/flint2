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
#include "flint.h"
#include "ulong_extras.h"

int main(void)
{
    int i;
    flint_rand_t state;

    flint_printf("discrete_log_bsgs....");
    ffwush(stdout);
   
    flint_randinit(state);

    for (i = 0; i < 1000; i++)
    {
        mp_limb_t p, root, b, d, result;
        double pinv;
        
        p = n_randprime(state, 20, 1);
        pinv = n_precompute_inverse(p);
        root = n_primitive_root_prime(p);
        b = n_randint(state, p - 1) + 1;
        d = n_discrete_log_bsgs(b, root, p);
        
        result = n_powmod_precomp(root, d, p, pinv);

        if (result != b)
        {
            flint_printf("FAIL:\n");
            flint_printf("%wu ** (%wu) == %wu != %wu\n", root, d, result, b);
            abort();
        }
    }

   flint_randclear(state);

   flint_printf("PASS\n");
   return 0;

}

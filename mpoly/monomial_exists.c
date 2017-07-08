/*
    Copyright (C) 2017 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include <stdlib.h>
#include "flint.h"
#include "mpoly.h"

int mpoly_monomial_exists1(slong * index, const ulong * poly_exps,
                                                    const ulong exp, slong len)
{
   slong n = len;
   slong i = 0;

   if (exp > poly_exps[0]) /* greater than first term */
   {
      (*index) = 0;
      return 0;
   }

   while (n > 1) /* do binary search */
   {
      slong half = n/2;

      /* if in first half */
      if (exp > poly_exps[i + half]) 
         n = half;
      else /* in second half */
      {
         n -= half;
         i += half;
      }
   }

   /* if equal to term at index i */
   if (exp == poly_exps[i])
   {
      (*index) = i;
      return 1;
   } else /* less than term at index i, but doesn't exist */
   {
      (*index) = i + 1;
      return 0;
   } 
}

int mpoly_monomial_exists(slong * index, const ulong * poly_exps,
                                         const ulong * exp, slong len, slong N)
{
   slong n = len;
   slong i = 0;

   if (len == 0) /* no terms to search */
   {
      (*index) = 0;
      return 0;
   }

   /* specialised version if exponent vectors are one word */
   if (N == 1)
      return mpoly_monomial_exists1(index, poly_exps, *exp, len);

   if (mpoly_monomial_gt(poly_exps, exp, N)) /* greater than first term */
   {
      (*index) = 0;
      return 0;
   }

   while (n > 1) /* do binary search */
   {
      slong half = n/2;

      /* if in first half */
      if (mpoly_monomial_gt(poly_exps + (i + half)*N, exp, N)) 
         n = half;
      else /* in second half */
      {
         n -= half;
         i += half;
      }
   }

   /* if equal to term at index i */
   if (mpoly_monomial_equal(poly_exps + i*N, exp, N))
   {
      (*index) = i;
      return 1;
   } else /* less than term at index i, but doesn't exist */
   {
      (*index) = i + 1;
      return 0;
   } 
}
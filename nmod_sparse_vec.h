/*
    Copyright (C) 2010 William Hart
    Copyright (C) 2010,2011 Fredrik Johansson
    Copyright (C) 2014 Ashish Kedia

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef NMOD_SPARSE_VEC_H
#define NMOD_SPARSE_VEC_H

#ifdef NMOD_SPARSE_VEC_INLINES_C
#define NMOD_SPARSE_VEC_INLINE FLINT_DLL
#else
#define NMOD_SPARSE_VEC_INLINE static __inline__
#endif

#undef ulong
#define ulong ulongxx /* interferes with system includes */
#include <stdlib.h>
#include <string.h>
#undef ulong
#include <gmp.h>
#define ulong mp_limb_t

#include "flint.h"
#include "longlong.h"
#include "ulong_extras.h"
#include "nmod_vec.h"
#include "fmpz.h"
#include "thread_support.h"

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct {
    slong ind;
    mp_limb_t val;
} nmod_sparse_entry_struct;

typedef nmod_sparse_entry_struct nmod_sparse_entry_t[1];

typedef struct {
    nmod_sparse_entry_struct *entries;
    slong nnz;
} nmod_sparse_vec_struct;

typedef nmod_sparse_vec_struct nmod_sparse_vec_t[1];

/* Memory management */
NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_init(nmod_sparse_vec_t vec) 
{
    memset(vec, 0, sizeof(*vec));
}
NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_clear(nmod_sparse_vec_t vec) 
{
    flint_free(vec->entries);
    memset(vec, 0, sizeof(*vec));
}
NMOD_SPARSE_VEC_INLINE 
void nmod_sparse_vec_swap(nmod_sparse_vec_t vec1, nmod_sparse_vec_t vec2) 
{
    nmod_sparse_vec_t tmp;
    *tmp = *vec1, *vec1 = *vec2, *vec2 = *tmp;
}

/* One-time instantiation */
NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_zero(nmod_sparse_vec_t vec) 
{
    nmod_sparse_vec_clear(vec);
}

NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_one(nmod_sparse_vec_t vec, slong ind) 
{
    vec->entries = realloc(vec->entries, sizeof(*vec->entries));
    vec->entries[0].ind = ind;
    vec->entries[0].val = UWORD(1);
}

FLINT_DLL
void nmod_sparse_vec_set(nmod_sparse_vec_t vec, const nmod_sparse_vec_t src);

FLINT_DLL
void nmod_sparse_vec_from_entries(nmod_sparse_vec_t vec, slong * inds, mp_limb_t * vals, slong nnz);


/* Vector comparison */
NMOD_SPARSE_VEC_INLINE
int nmod_sparse_vec_is_zero(const nmod_sparse_vec_t vec) 
{
    return vec->nnz == 0;
}

FLINT_DLL
int nmod_sparse_vec_equal(const nmod_sparse_vec_t vec1, const nmod_sparse_vec_t vec2, slong ioff);

/* Vector indexing */
FLINT_DLL 
mp_limb_t nmod_sparse_vec_at(nmod_sparse_vec_t vec, slong i);

/* Convert from/to dense vector */
FLINT_DLL
void nmod_sparse_vec_from_dense(nmod_sparse_vec_t vec, mp_srcptr src, slong len);

NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_to_dense(mp_ptr vec, const nmod_sparse_vec_t src, slong len) {
    slong i;
    _nmod_vec_zero(vec, len);
    for(i=0; i<src->nnz; ++i) vec[src->entries[i].ind] = src->entries[i].val;
}

/* Windows and concatenation */
FLINT_DLL
void nmod_sparse_vec_window_init(nmod_sparse_vec_t window, const nmod_sparse_vec_t vec, slong i1, slong i2);


NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_window_clear(nmod_sparse_vec_t window) 
{
    memset(window, 0, sizeof(*window));
}

FLINT_DLL
void nmod_sparse_vec_concat(nmod_sparse_vec_t res, const nmod_sparse_vec_t vec1,  const nmod_sparse_vec_t vec2, slong len1);

/* Vector permutation */
NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_permute_inds(nmod_sparse_vec_t vec, slong *P) 
{
    slong i;
    for(i=0; i<vec->nnz; ++i) vec->entries[i].ind = P[vec->entries[i].ind];
}

/* Random vector generation */
FLINT_DLL 
void nmod_sparse_vec_randtest(nmod_sparse_vec_t vec, flint_rand_t state, slong nnz, slong len, nmod_t mod);

/* Vector display */
FLINT_DLL
void nmod_sparse_vec_print_pretty(const nmod_sparse_vec_t vec, slong ioff, slong maxi, nmod_t mod);

/* Vector operations */
NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_neg(nmod_sparse_vec_t v, const nmod_sparse_vec_t u, nmod_t mod) {
    slong i;
    nmod_sparse_vec_set(v, u);
    for(i=0; i<v->nnz; ++i) v->entries[i].val = nmod_neg(v->entries[i].val, mod);
}

FLINT_DLL
void nmod_sparse_vec_scalar_mul(nmod_sparse_vec_t v, const nmod_sparse_vec_t u, mp_limb_t c, nmod_t mod);

NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_scalar_mul_fmpz(nmod_sparse_vec_t v, const nmod_sparse_vec_t u, const fmpz_t c, nmod_t mod) {
    fmpz_t d;
    fmpz_init(d);
    fmpz_mod_ui(d, c, mod.n);
    nmod_sparse_vec_scalar_mul(v, u, fmpz_get_ui(d), mod);
    fmpz_clear(d);
}

FLINT_DLL 
void nmod_sparse_vec_scalar_addmul(nmod_sparse_vec_t w, const nmod_sparse_vec_t u, const nmod_sparse_vec_t v, mp_limb_t c, nmod_t mod);

NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_add(nmod_sparse_vec_t w, const nmod_sparse_vec_t u, const nmod_sparse_vec_t v, nmod_t mod) {
    nmod_sparse_vec_scalar_addmul(w, u, v, UWORD(1), mod);
}

NMOD_SPARSE_VEC_INLINE
void nmod_sparse_vec_sub(nmod_sparse_vec_t w, const nmod_sparse_vec_t u, const nmod_sparse_vec_t v, nmod_t mod) {
    nmod_sparse_vec_scalar_addmul(w, u, v, mod.n-1, mod);
}

FLINT_DLL
mp_limb_t nmod_sparse_vec_dot(const nmod_sparse_vec_t u, const nmod_sparse_vec_t v, nmod_t mod);

FLINT_DLL
mp_limb_t nmod_sparse_vec_dot_dense(const nmod_sparse_vec_t u, mp_srcptr v, nmod_t mod);

#ifdef __cplusplus
}
#endif

#endif


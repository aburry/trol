/* Copyright 2008 Adam Burry
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef CONS_IMPL_H
#define CONS_IMPL_H

/*! \file cons_impl.h
 *
 * \brief Primitive lisp data structures.
 *
 * This module defines the lisp atom, cons and symbolic expression data
 * structures. Operations on those structures are defined in cons_impl.c.
 *
 * \note Use this module when you need to access details beyond
 * the ::sexp typedef facade in cons.h. In general, however, you
 * should prefer to include cons.h.
 */

#include "cons.h"


/*! \brief Symbolic expression types.
 *
 * A lisp symbolic expression may be one of two types: a cons pair or
 * an atom.
 *
 * A cons is a container with left and right storage cells. The left
 * is called the car or first. The right is called the cdr or rest.
 * A cons cell may contain another cons pair, or an atom.
 *
 * An atom is a character string.
 */
typedef enum { CONS, ATOM } expr_type;


/*! \brief Symbolic expression.
 *
 * This is a run-time type information (RTTI) container. The members
 * are const because expressions are immutable once constructed.
 */
struct sexp_impl {
    /*! Type ID for #v.
     *
     * CONS or ATOM.
     */
    const expr_type t;
    /*! Generic pointer to cons pair or atom.
     *
     * Has type char* for atoms and struct ::cons_impl* for cons
     * pairs.
     */
    const void* const v;
};


/*! \brief Cons pair.
 *
 * A cons pair can hold two things, called car and cdr. Car is
 * sometimes called first, and cdr ("could-er") is sometimes called
 * rest. Pairs can be used to build arbitrary data structures,
 * simple examples are lists and trees. The members are const
 * because cons pairs are immutable once constructed.
 */
struct cons_impl {
    /*! \brief Car or first.
     *
     * Car or first.
     */
    const sexp l;
    /*! \brief Cdr or rest.
     *
     * Cdr or rest.
     */
    const sexp r;
};

#endif
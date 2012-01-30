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

/*! \file constants.c
 *
 * \brief Symbolic constants for common atoms.
 *
 * The following atoms have symbolic constants: t, nil, quote,
 * ., atom, eq, car, cdr, cons, cond, lambda and label. Strictly
 * speaking, dot (.) is not an atom. However, the parser treats it
 * as one while scanning the text before it filters it out.
 */

#include "constants.h"

#include "cons_impl.h"

/*! \internal
 * \brief Generate a factory function for symbolic constants.
 */
#define CONST_ATOM(f,str) \
    sexp f() { \
        static struct sexp_impl r = { ATOM, str }; \
        return &r; \
    }


/*!
 * \fn sexp ATOM_T()
 * \brief 't
 *
 * \fn sexp ATOM_NIL()
 * \brief 'nil
 *
 * \fn sexp ATOM_QUOTE()
 * \brief 'quote
 *
 * \fn sexp ATOM_DOT()
 * \brief '.
 * \note Not an atom strictly speaking. This constant is used by
 * the parser.
 *
 * \fn sexp ATOM_ATOM()
 * \brief 'atom
 *
 * \fn sexp ATOM_EQ()
 * \brief 'eq
 *
 * \fn sexp ATOM_CAR()
 * \brief 'car
 *
 * \fn sexp ATOM_CDR()
 * \brief 'cdr
 *
 * \fn sexp ATOM_CONS()
 * \brief 'cons
 *
 * \fn sexp ATOM_COND()
 * \brief 'cond
 *
 * \fn sexp ATOM_LAMBDA()
 * \brief 'lambda
 *
 * \fn sexp ATOM_LABEL()
 * \brief 'label
 */

CONST_ATOM(ATOM_T, "t");
CONST_ATOM(ATOM_NIL, "nil");
CONST_ATOM(ATOM_QUOTE, "quote");
CONST_ATOM(ATOM_DOT, ".");
CONST_ATOM(ATOM_ATOM, "atom");
CONST_ATOM(ATOM_EQ, "eq");
CONST_ATOM(ATOM_CAR, "car");
CONST_ATOM(ATOM_CDR, "cdr");
CONST_ATOM(ATOM_CONS, "cons");
CONST_ATOM(ATOM_COND, "cond");
CONST_ATOM(ATOM_LAMBDA, "lambda");
CONST_ATOM(ATOM_LABEL, "label");

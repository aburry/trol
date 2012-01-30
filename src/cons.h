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

#ifndef CONS_H
#define CONS_H

/*! \file cons.h
 */

#include <stdbool.h>


/*! \brief Symbolic expression.
 *
 * The symbolic expression (sexp) is the basic abstract data type of
 * lisp. Expressions may be atoms or cons pairs. The atom() predicate
 * can be used to tell them apart. Sexp's are immutable.
 *
 * \note This typedef is a facade, if you need access to the
 * underlying structure, include cons_impl.h.
 */
typedef const struct sexp_impl* sexp;


sexp symbol(const char* str, int strlen);
sexp cons(sexp car, sexp cdr);
void gc_sexp(sexp expr);
sexp car(sexp cons);
sexp cdr(sexp cons);
sexp atom(sexp expr);
sexp eq(sexp expr_a, sexp expr_b);
const char* c_str(sexp atom);
bool c_bool(sexp expr);


#endif

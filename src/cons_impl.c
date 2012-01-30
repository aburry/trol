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

/*! \file cons_impl.c
 *
 * \brief Primitive lisp functions.
 *
 * This module defines operations on lisp data structures. Dynamic
 * behaviour is defined in eval.c.
 *
 * \note These methods differ from normal lisp methods in that they
 * do not evaluate their arguments. (atom (quote a)) is not the same
 * as atom(cons(ATOM_QUOTE(), cons(symbol("a", 1), ATOM_NIL()))).
 * Instead, it is (atom '(quote a)).
 */

#include "cons_impl.h"

#include "constants.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/*! \internal
 * \brief const_cast for initialisation of struct const members.
 */
#define CONST_CAST(type, expr) \
    (*(type*)(&(expr)))


/*! \brief Convert a lisp expression to a C bool.
 *
 * In general, you should prefer to use atom() and eq() as they are.
 * However, from time to time it will be necesssary to wrap them as
 * c_bool(eq(a,b)) or c_bool(atom(p)).
 *
 * \param expr Arbitrary lisp.
 * \return \c true if \a expr is \c 't, \c false otherwise.
 */
bool c_bool(sexp expr) {
    return expr && (expr->t == ATOM) && !strcmp((char*)(expr->v), "t");
}


/*! \brief Get an atom's string.
 *
 * This method allows things like printing an atom.
 *
 * \param atom A lisp atom.
 * \return '\\0' terminated C string.
 */
const char* c_str(sexp atom) {
    return atom->t == ATOM ? (char*)(atom->v) : 0;
}


/*! \brief Predicate to test if \a expr is an atom.
 *
 * \param expr Arbitrary lisp expression.
 * \return \c 't if \a expr is an atom, \c 'nil otherwise.
 */
sexp atom(sexp expr) {
    return (expr && expr->t == ATOM) ? ATOM_T() : ATOM_NIL();
}


/*! \brief Compare two atoms.
 *
 * \param expr_a Arbitrary lisp expression.
 * \param expr_b Arbitrary lisp expression.
 * \return \c 't if \a expr_a and \a expr_b are the same atom,
 * \c 'nil otherwise.
 */
sexp eq(sexp expr_a, sexp expr_b) {
    if(c_bool(atom(expr_a)) && c_bool(atom(expr_b))){
        if(expr_a == expr_b) {        /* optimisation heuristic */
            return ATOM_T();
        }
        if(!strcmp(c_str(expr_a), c_str(expr_b))) {
            return ATOM_T();
        }
    }
    return ATOM_NIL();
}


/*! \brief Get the first element of a cons pair.
 *
 * The name car comes from "Contents of Address Register". Steve
 * Russell, the original lisp implementer, admits it was an
 * uninspired name and suggests, in retrosect, that "first" would
 * have been a better choice.
 *
 * \param cons A cons pair, for example the head of a list.
 * \return The first part of the pair.
 */
sexp car(sexp cons) {
    return ((struct cons_impl*)(cons->v))->l;
}


/*! \brief Get the second element of a cons pair.
 *
 * The name cdr comes from "Contents of Decrement Register". Steve
 * Russell, the original lisp implementer, suggests that "rest"
 * would have been a better name.
 *
 * \param cons A cons pair, for example the head of a list.
 * \return The second part of the pair, often the tail of a list.
 */
sexp cdr(sexp cons) {
    return ((struct cons_impl*)(cons->v))->r;
}


/*! \brief Create an atom of a given string.
 *
 * Atoms are so called because they do not have any sub-parts
 * you can inspect, they cannot be further decomposed.
 *
 * \a len is used to allow flexibility when parsing atoms from
 * character buffers. Admittedly, it is a bit annoying when
 * you have a null terminated \a str.
 *
 * \param str A character string buffer.
 * \param len Length of \a str in characters.
 * \return The atom representing \a str.
 *
 * \note I wanted to call this method atom() to parallel cons(),
 * but I was already using it for the predicate.
 */
sexp symbol(const char* str, int len) {
    struct sexp_impl* r = malloc(sizeof *r);
    CONST_CAST(int, r->t) = ATOM;
    char* sym = malloc(len+1);
    strncpy(sym, str, len);
    sym[len] = 0;
    CONST_CAST(char*, r->v) = sym;
    return r;
}


/*! \brief Create a cons pair.
 *
 * Because cons's may contain other cons's, they can be used to build
 * quite complex data structures. The most common data structure in
 * lisp is the list which is a simple cons chain. In a list, the
 * first element of a cons, the car, is an atom, the second element
 * of a cons, the cdr, is the next cons.
 *
 * \param expr_a Arbitrary lisp.
 * \param expr_b arbitrary lisp.
 * \return The newly constructed cons.
 */
sexp cons(sexp expr_a, sexp expr_b) {
    struct sexp_impl* r = malloc(sizeof *r);
    CONST_CAST(int, r->t) = CONS;
    struct cons_impl* pcons = malloc(sizeof *pcons);
    CONST_CAST(struct sexp_impl*, pcons->l)
        = CONST_CAST(struct sexp_impl*, expr_a);
    CONST_CAST(struct sexp_impl*, pcons->r)
        = CONST_CAST(struct sexp_impl*, expr_b);
    CONST_CAST(struct cons_impl*,r->v) = pcons;
    return r;
}


/*! \brief Garbage collect memory.
 *
 * If \a expr is a cons, then the car and cdr will be recursively
 * deleted as well if they are no longer needed.
 *
 * \param expr The expression to free.
 *
 * \todo Implement garbage collection.
 */
void gc_sexp(sexp expr) {
    /* not implemented */
}

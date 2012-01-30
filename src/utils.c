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

/*! \file utils.c
 *
 * \brief Non-primitive functions required by eval().
 *
 * These functions can be written in terms of the lisp understood
 * by eval() and they make writing eval() simpler. They are used
 * for handling the environment and function arguments.
 */

#include "utils.h"

#include "constants.h"


/*! \brief Test for \c 'nil.
 *
 * Predicate to test for \c 'nil atom. In lisp, \c 'nil is used for
 * false and the empty list.
 *
 * \param expr Arbitrary lisp symbolic expression.
 * \return \c 't if \a expr is the \c 'nil atom, \c 'nil otherwise.
 */
sexp null(sexp expr) {
    return eq(expr,ATOM_NIL());
}


/*! \brief Concatenate two lists.
 *
 * Create a list by joining two lists together.
 *
 * \param list_a A list to become the head of new list.
 * \param list_b A list to become the tail of new list.
 * \return A list with the elements of \a list_a followed by the
 * elements of \a list_b.
 */
sexp append(sexp list_a, sexp list_b) {
    if (c_bool(null(list_a))) { return list_b; }
    return cons(car(list_a), append(cdr(list_a), list_b));
}


/*! \brief Compare two lisp expressions.
 *
 * Expressions \a expr_a and \a expr_b are equal if they have the
 * same structure.
 *
 * \param expr_a A symbolic lisp expression.
 * \param expr_b A symbolic lisp expression.
 * \return \c 't if \a expr_a and \a expr_b are isomorphic, \c 'nil
 * otherwise.
 *
 * \note This function is not used by eval(). Instead, it is
 * useful for testing.
 */
sexp equal(sexp expr_a, sexp expr_b) {
    if (c_bool(atom(expr_a))){
        if (c_bool(atom(expr_b))) {
            return eq(expr_a,expr_b);
        }
        return ATOM_NIL();
    }
    if (c_bool(atom(expr_b))) {
        return ATOM_NIL();
    }
    return c_bool(equal(car(expr_a),car(expr_b))) &&
        c_bool(equal(cdr(expr_a),cdr(expr_b)))
        ? ATOM_T() : ATOM_NIL();
}


/*! \brief Build a dictionary.
 *
 * Build a dictionary from a list of keys \a list_a and a list
 * of values \a list_b. The dictionary is in the same order as
 * \a list_a; it is not sorted. An example in lisp syntax:
 *
 * \code
 * > (pair '(value key) '(3 definition))
 * ((value . 3) (key . definition))
 * \endcode
 *
 * \param list_a A list of keys.
 * \param list_b A list of values.
 * \return A list of (key . value) pairs.
 */
sexp pair(sexp list_a, sexp list_b) {
    if (c_bool(null(list_a)) && c_bool(null(list_b))) {
        return ATOM_NIL();
    }
    if (!c_bool(atom(list_a)) && !c_bool(atom(list_b))) {
        return cons(
            cons(car(list_a),car(list_b)),
            pair(cdr(list_a),cdr(list_b)));
    }
    /* TRoL has this implied */
    return ATOM_NIL();
}


/*! \brief Look up \a key in \a map.
 *
 * \param key An atom.
 * \param map A dictionary in the form created by pair().
 * \return The value associated with \a key if found, \a key
 * otherwise.
 */
sexp assoc(sexp key, sexp map) {
    /* TRoL missing the '() case */
    if (c_bool(eq(map, ATOM_NIL()))) { return key; }
    /* return car(cdr(car ? */
    if (c_bool(eq(car(car(map)), key))) { return cdr(car(map)); }
    return assoc(key, cdr(map));
}

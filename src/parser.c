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

/*! \file parser.c
 *
 * \brief Reading and printing expressions.
 *
 * This module defines methods for converting a C string to an ::sexp,
 * and converting an ::sexp to a C string. Printing and reading dot
 * notation and list notation are supported.
 */

#include "parser.h"

#include "constants.h"

#include <stdio.h>


static sexp parse_atom(const char** p);
static sexp parse_cons(const char** p);
static sexp parse_list(const char** p);
static sexp parse_list_elem(const char** p);
static sexp parse_quote(const char** p);
static void parse_ws(const char** p);
static int print_list_notation_rest(char* str, size_t len, sexp p);


/*! \brief Print an expression in dot notation.
 *
 * Dot notation uses a dot between the members of a cons pair. A cons
 * is written with parentheses around it. For example: (a . (b . c)).
 *
 * \param str Buffer.
 * \param len Storage capacity of \a str.
 * \param expr Arbitrary lisp expression.
 * \return Same as snprintf().
 */
int print_dot_notation(char* str, size_t len, sexp expr) {
    size_t r = 0;
    if (expr) {
        if (c_bool(atom(expr)) && c_str(expr)) {
            return snprintf(str, len, "%s", c_str(expr));
        } else {
            if ( r >= len ) { return r; }
            r += snprintf(str+r, len-r, "(");
            if ( r >= len ) { return r; }
            r += print_dot_notation(str+r, len-r, car(expr));
            if ( r >= len ) { return r; }
            r += snprintf(str+r, len-r, " . ");
            if ( r >= len ) { return r; }
            r += print_dot_notation(str+r, len-r, cdr(expr));
            if ( r >= len ) { return r; }
            r += snprintf(str+r, len-r, ")");
        }
    }
    return r;
}


/*! \internal
 * \brief Print the tail of a list.
 */
static int print_list_notation_rest(char* str, size_t len, sexp p) {
    size_t r = 0;
    /* cdr */
    if (p) {
        if (c_bool(atom(p))) {
            if (!c_bool(eq(p,ATOM_NIL()))) {
                if ( r >= len ) { return r; }
                r += snprintf(str+r, len-r, " . ");
                if ( r >= len ) { return r; }
                r += print_list_notation(str+r, len-r, p);
            }
        } else {
            if ( r >= len ) { return r; }
            r += snprintf(str+r, len-r, " ");
            if ( r >= len ) { return r; }
            r += print_list_notation(str+r, len-r, car(p));
            if ( r >= len ) { return r; }
            r += print_list_notation_rest(str+r, len-r, cdr(p));
        }
    }
    return r;
}


/*! \brief Print an expression in list notation.
 *
 * List notation is the usual way of writing lisp. It allows
 * (quote expr) to be written as 'expr. Cons chains can be simplified
 * as (a b c) for (a . (b . (c . nil))). If the \c 'nil in the
 * previous example were a \c 'd, it would be written (a b (c . d)).
 *
 * \param str Buffer.
 * \param len Storage capacity of \a str.
 * \param expr Arbitrary lisp expression.
 * \return Same as snprintf().
 */
int print_list_notation(char* str, size_t len, sexp expr) {
    size_t r = 0;
    if (expr) {
        if (c_bool(atom(expr))) {
            if (c_str(expr)) {
                if ( r >= len ) { return r; }
                r += snprintf(str+r, len-r, "%s", c_str(expr));
            }
            return r;
        } else {
            /* parens */
            if (!c_bool(eq(ATOM_QUOTE(),car(expr)))
                    || (c_bool(eq(ATOM_QUOTE(),car(expr)))
                    && c_bool(atom(cdr(expr))))) {
                if ( r >= len ) { return r; }
                r += snprintf(str+r, len-r, "(");
            }

            /* car */
            if (!c_bool(eq(ATOM_QUOTE(),car(expr)))
                    || (c_bool(eq(ATOM_QUOTE(),car(expr)))
                    && c_bool(atom(cdr(expr))))) {
                if ( r >= len ) { return r; }
                r += print_list_notation(str+r, len-r, car(expr));
            } else {
                if ( r >= len ) { return r; }
                r += snprintf(str+r, len-r, "'");
                sexp qp = cdr(expr);
                if ( r >= len ) { return r; }
                r += print_list_notation(str+r, len-r, car(qp));
                if ( r >= len ) { return r; }
                r += print_list_notation_rest(str+r, len-r, cdr(qp));
            }

            /* cdr */
            if (!c_bool(eq(ATOM_QUOTE(),car(expr)))
                    || (c_bool(eq(ATOM_QUOTE(),car(expr)))
                    && c_bool(atom(cdr(expr))))) {
                if ( r >= len ) { return r; }
                r += print_list_notation_rest(str+r, len-r, cdr(expr));
            }

            /* parens */
            if (!c_bool(eq(ATOM_QUOTE(),car(expr)))
                    || (c_bool(eq(ATOM_QUOTE(),car(expr)))
                    && c_bool(atom(cdr(expr))))) {
                if ( r >= len ) { return r; }
                r += snprintf(str+r, len-r, ")");
            }
        }
    }
    return r;
}


/*! \internal
 * \brief Eat whitespace.
 */
static void parse_ws(const char** p) {
    /* skip whitespace */
    while ( **p == ' ' || **p == '\t' || **p == '\r'
            || **p == '\n' ) { ++(*p); }
}


/*! \internal
 * \brief Extract a symbol from the buffer.
 */
static sexp parse_atom(const char** p) {
    /* atom or . */
    const char* s = *p;
    /* skip valid symbol characters */
    while (**p != ' ' && **p != '\t' && **p != '\r' && **p != '\n'
            && **p != '(' && **p != ')' && **p != '\0') { ++(*p); }
    if (0 == (*p)-s) { return 0; }
    return symbol(s, (*p)-s);
}


/*! \internal
 * \brief Parse ' (quote) list notation shorthand.
 */
static sexp parse_quote(const char** p) {
    ++(*p);
    const sexp e = parse(p);
    if (0 == e) { return 0; }
    return cons(ATOM_QUOTE(),cons(e,ATOM_NIL()));
}


/*! \internel
 * \brief Parse a dot pair.
 *
 * For example: (a . b)
 */
static sexp parse_cons(const char** p) {
    ++(*p); /* '(' */
    const sexp car = parse(p);
    if (0 == car) { return 0; }
    parse(p); /* '.' */
    const sexp t = parse(p);
    if (0 == t) { return 0; }
    const sexp r = cons(car,t);
    parse_ws(p);
    if (**p == ')') { ++(*p); }
    return r;
}


/*! \internal
 * \brief Parse part of a list notation list.
 */
static sexp parse_list_elem(const char** p) {
    const sexp t = parse(p);
    if (0 == t) { return 0; }
    if (c_bool(eq(ATOM_DOT(),t))) {
        return t;
    }
    parse_ws(p);
    if (**p == ')' || *p == '\0') {
        return cons(t, ATOM_NIL());
    }
    const sexp r = parse_list_elem(p);
    if (0 == r) { return 0; }
    if (c_bool(eq(ATOM_DOT(),r))) {
        return r;
    }
    return cons(t,r);
}


/*! \internal
 * \brief Parse a list notation list or dotted pair.
 */
static sexp parse_list(const char** p) {
    const char* bookmark = *p; /* in case this turns out to be a cons */

    sexp r = ATOM_NIL();

    ++(*p); /* '(' */
    parse_ws(p);

    if (**p != ')' && *p != '\0') {
        r = parse_list_elem(p);
	if (0 == r) { return 0; }
        parse_ws(p);
        if (c_bool(eq(ATOM_DOT(),r))) {
            *p = bookmark;
            return parse_cons(p);
        }
    }

    if (**p == ')') { ++(*p); }
    return r;
}


/*! \brief Convert a char buffer into an expression.
 *
 * Parse a string into a lisp expression. Handles list and dot
 * notation. Handles quote shorthand.
 *
 * \param iter_ref Address of a pointer to start of buffer.
 * *iter_ref points to the character after the last successfully
 * parsed character at return time.
 * \return Lisp expression.
 */
sexp parse(const char** iter_ref) {
    parse_ws(iter_ref);

    /* end of input */
    if (**iter_ref == '\0') { return 0; }

    /* quote */
    if (**iter_ref == '\'') { return parse_quote(iter_ref); }

    /* cons or list */
    if (**iter_ref == '(') { return parse_list(iter_ref); }

    /* atom or . */
    return parse_atom(iter_ref);
}

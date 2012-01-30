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

/*! \file main.c
 *
 * \brief Interactive lisp interpreter.
 */

#include "constants.h"
#include "eval.h"
#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/*!
 * \brief Interactive lisp read-eval-print loop.
 *
 * To exit the interpreter, type "(quit)".
 *
 * Example session (taken from TRoL):
 * \code
 * > (quote a)
 * a
 * > (quote (a b c))
 * (a b c)
 * > (atom 'a)
 * t
 * > (atom '(a b c))
 * nil
 * > (atom '())
 * t
 * > (atom (atom 'a))
 * t
 * > (atom '(atom 'a))
 * nil
 * > (eq 'a 'a)
 * t
 * > (eq 'a 'b)
 * nil
 * > (eq '() '())
 * t
 * > (car '(a b c))
 * a
 * > (cdr '(a b c))
 * (b c)
 * > (cons 'a '(b c))
 * (a b c)
 * > (cons 'a (cons 'b (cons 'c '())))
 * (a b c)
 * > (car (cons 'a '(b c)))
 * a
 * > (cdr (cons 'a '(b c)))
 * (b c)
 * > (cond ((eq 'a 'b) 'first) ((atom 'a) 'second))
 * second
 * > ((lambda (x) (cons x '(b))) 'a)
 * (a b)
 * > ((lambda (x y) (cons x (cdr y))) 'z '(a b c))
 * (z b c)
 * > ((lambda (f) (f '(b c))) '(lambda (x) (cons 'a x)))
 * (a b c)
 * > ((label subst (lambda (x y z) (cond ((atom z) (cond ((eq z y) x) ('t z))) ('t (cons (subst x y (car z)) (subst x y (cdr z))))))) 'm 'b '(a b (a b c) d))
 * (a m (a m c) d)
 * > (quit)
 * $
 * \endcode
 *
 * Lisp may also be read from a file, for example:
 *
 * \code
 * ./lisp < ../test/sample.lisp
 * \endcode
 *
 * \param argc Argument count.
 * \param argv Vector of argument strings.
 * \return Process error code.
 *
 * \sa parse(), eval(), print_list_notation()
 */
int main(int argc, char* argv[]) {
    sexp env = ATOM_NIL();
    char in_str[1000] = "";
    char out_str[1000] = "";
    const char* prompt = "> ";
    const char* quit = "(quit)\n";
    const char* p = &in_str[0];

    printf("%s", prompt); fflush(0);
    while (true) {
        const char* s = fgets(p, sizeof(in_str)/sizeof(char)-(p-&in_str[0]), stdin);
	if (!s) { break; }
	p = &in_str[0];
        if (0 == strcmp(p, quit)) { break; }
        sexp e = parse(&p);
        if (e) {
	    p = &in_str[0];
            sexp r = eval(e, env);
            print_list_notation(out_str, sizeof(out_str)/sizeof(char), r);
            printf("%s\n", out_str); fflush(0);
            printf("%s", prompt); fflush(0);
        }
    }
    return 0;
}

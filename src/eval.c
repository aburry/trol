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

/*! \file eval.c
 *
 * \brief Lisp interpreter.
 *
 * The star of the show.
 */

#include "eval.h"

#include "constants.h"
#include "utils.h"


/*! \mainpage
 *
 * \section s1 Introduction
 *
 * This eval() is based on Paul Graham's paper, "The Roots of Lisp,"
 * (TRoL) available at http://www.paulgraham.com/rootsoflisp.html .
 * Graham's paper, in turn, is based on John McCarthy's, "Recursive
 * Functions of Symbolic Expressions and Their Computation by Machine,
 * Part I," available at
 * http://www-formal.stanford.edu/jmc/recursive.pdf
 *
 * I wrote this interpreter to help me understand the core of lisp.
 * This is a popular exercise and there is nothing particularly
 * special about this attempt. But I decided to make it available in
 * case someone else might find it useful.
 *
 * If you do find it useful, I'd welcome a postcard sent to:
 *
 *     ADAM BURRY                     \n
 *     32 CRAIGMILLAR AVE             \n
 *     ST. JOHN'S NL  A1E 1Z8         \n
 *     CANADA                         \n
 *
 * \section s2 Data Structures
 *
 * Lisp data may be one of two types: an atom or a cons pair.
 *
 * An atom is simply a character string.
 *
 * A cons is a container with two storage cells. The left is called
 * the car or first. The right is called the cdr or rest. A cons cell
 * may contain either another cons pair, or an atom. Among other
 * things, cons pairs make it easy to build linked lists and trees.
 *
 * An important note is that lisp expressions map naturally into
 * lisp data.
 *
 * \section s3 Built-in Functions
 *
 * The interpreter has the following built-in functions: cons, car,
 * cdr, atom, eq, quote, cond, lambda and label.
 *
 * See TRoL for a description.
 *
 * \section s4 Notation
 *
 * The interpreter can parse both dot notation and list notation. The
 * output is always in list notation. Dot notation makes every cons
 * and atom explicit, for example: (quote . (a . nil)) is the same as
 * '(a) in list notation. This interpreter is case-sensitive, so calls
 * to the built-in functions must be in lowercase.
 *
 * \section s5 Known Issues
 *
 * There is no garbage collection. Therefore, it leaks like a sieve.
 *
 * There is no detection of cyclic data structures. Therefore, some
 * functions may find themselves evaluating an infinite recursion.
 * This could easily happen to equal(), for example.
 *
 * \section s7 Further Reading
 *
 * There are many excellent free resources available for studying
 * lisp. For example:
 * \li http://mitpress.mit.edu/sicp/
 * \li http://gigamonkeys.com/book/
 * \li http://www.paulgraham.com/onlisp.html
 * \li http://www.cs.cmu.edu/~dst/LispBook/
 * \li http://www.faqs.org/faqs/lisp-faq/
 * \li http://groups.google.com/group/comp.lang.lisp/topics
 *
 * \section s8 Contact
 *
 * aburry@ieee.org
 */


static sexp eval_cond(sexp e, sexp env) ;
static sexp eval_list(sexp m, sexp env) ; 

/*! \internal
 * \brief Eval function arguments.
 *
 * \return List of argument values.
 */
static sexp eval_list(sexp m, sexp env) {
    if (c_bool(null(m))) {
        return ATOM_NIL();
    }
    return cons(eval(car(m), env), eval_list(cdr(m), env));
}


/*! \internal
 * \brief Eval cond arguments (short-circuit).
 *
 * \return Result of chosen expression. \c 'nil if no condition
 * was true.
 *
 * \note Define a cond with no true cases to return nil as though cond
 * had a default \c 't like so: (cond (p1 e1) ... (pn en) ('t '()))
 * TRoL glosses over this case.
 */
static sexp eval_cond(sexp e, sexp env) {
    if(c_bool(null(e))) {
        return ATOM_NIL();
    }
    if(c_bool(eq(ATOM_T(),eval(car(car(e)), env)))) {
        return eval(car(cdr(car(e))), env);
    }
    return eval_cond(cdr(e), env);
}


/*! \brief Interpret a lisp expression.
 *
 * TRoL implements eval in lisp. This implementation is not
 * entirely in lisp because I did not implement quote or cond;
 * they are implemented here by eval.
 *
 * \param expr Lisp expression.
 * \param env Dictionary of variables in scope.
 * \return Result of evaluation.
 *
 * \note TRoL adds the entire label expression to the env,
 * I don't know why. This implementation only adds the lambda
 * part.
 */
sexp eval(sexp expr, sexp env) {
    if(c_bool(atom(expr))) {
        return assoc(expr,env);
    }
    if(c_bool(atom(car(expr)))) {
        if(c_bool(eq(car(expr),ATOM_QUOTE()))) {
            return car(cdr(expr));
        }
        if(c_bool(eq(car(expr),ATOM_ATOM()))) {
            return atom(eval(car(cdr(expr)),env));
        }
        if(c_bool(eq(car(expr),ATOM_EQ()))) {
            return eq(
                eval(car(cdr(expr)),env),
                eval(car(cdr(cdr(expr))),env) );
        }
        if(c_bool(eq(car(expr),ATOM_CAR()))) {
            return car(eval(car(cdr(expr)),env));
        }
        if(c_bool(eq(car(expr),ATOM_CDR()))) {
            return cdr(eval(car(cdr(expr)),env));
        }
        if(c_bool(eq(car(expr),ATOM_CONS()))) {
            return cons(
                eval(car(cdr(expr)),env),
                eval(car(cdr(cdr(expr))),env));
        }
        if(c_bool(eq(car(expr),ATOM_COND()))) {
            return eval_cond(cdr(expr),env);
        }
        return eval(cons(assoc(car(expr), env), cdr(expr)), env);
    }
    if(c_bool(eq(car(car(expr)),ATOM_LABEL()))) {
        /* Compare to TRoL */
        sexp entry = cons(car(cdr(car(expr))),
                car(cdr(cdr(car(expr)))));
        return eval(
            cons(car(cdr(cdr(car(expr)))),cdr(expr)),
            cons(entry,env));
    }
    if(c_bool(eq(car(car(expr)),ATOM_LAMBDA()))) {
        return eval(
            car(cdr(cdr(car(expr)))),
            append(
                pair(
                    car(cdr(car(expr))),
                    eval_list(cdr(expr), env)),
                env));
    }
    return ATOM_NIL();
}

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

#include "test.h"

#include "cons.h"
#include "constants.h"
#include "eval.h"
#include "parser.h"
#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_eval();

int main(int argc, char* argv[]) {
    test_eval();
    printf("\n");

    return 0;
}


void test_eval() {

    char str[1000];
    const char* a = "(key t)"; const char* b = "(quote t)";
    sexp env = pair(parse(&a),parse(&b));

    char* test[] = {
        "((label subst (lambda (x y z) (cond ((atom z)(cond ((eq z y) x) ('t z))) ('t (cons (subst x y (car z)) (subst x y (cdr z))))))) 'm 'b '(a b (a b c) d))",
        "'(a b c d)",
        "(cond . (((eq . ((quote . (x . nil)) . ((quote . (z . nil)) . nil))) . ((quote . (first . nil)) . nil)) . (((atom . ((quote . (a . nil)) . nil)) . ((quote . (second . nil)) . nil)) . nil)))",
        "(cond ((eq 'x 'z) 'first) ((atom 'a) 'second))",
        "(eq . ((quote . (a . nil)) . ((quote . (a . nil)) . nil)))",
        "(quote . (a . nil))",
        "key",
        "(quote . ((a . (b . (c . nil))) . nil))",
        "(atom . ((quote . (a . nil)) . nil))",
        "(eq . ((quote . (a . nil)) . ((quote . (b . nil)) . nil)))",
        "(car . ((quote . ((car . cdr) . nil)) . nil))",
        "(cdr . ((quote . ((car . cdr) . nil)) . nil))",
        "(cons . ((quote . (a . nil)) . ((quote . (b . nil)) . nil)))",
        "(key . ((a . (b . (c . nil))) . nil))",
        "(cond . (((quote . (t . nil)) . ((quote . (b . nil)) . nil)) . nil))",
        "(cond . (((eq . ((quote . (a . nil)) . ((quote . (b . nil)) . nil))) . ((quote . (first . nil)) . nil)) . nil))",
        "(quote quote)",
        "(quote nil)",
        "'()",
        "'(a)",
        "'t",
        "'nil",
        "'a",
        "'(nil quote nil nil (nil . d))",
        "'(quote . quote)",
        "'(nil . nil)",
        "((lambda () 3))",
        "((lambda (a) a) 4)",
        "((label f (lambda () 42)))",
        "f"
    };

    char* result[] = {
        "(a m (a m c) d)",
        "(a b c d)",
        "second",
        "second",
        "t",
        "a",
        "quote",
        "(a b c)",
        "t",
        "nil",
        "car",
        "cdr",
        "(a . b)",
        "(a b c)",
        "b",
        "nil",
        "quote",
        "nil",
        "nil",
        "(a)",
        "t",
        "nil",
        "a",
        "(nil quote nil nil (nil . d))",
        "(quote . quote)",
        "(nil)",
        "3",
        "4",
        "42",
        "f"
    };

    TEST(sizeof(test) == sizeof(result));

    int i = 0;

    for (i = 0; i < sizeof(test)/sizeof(char*); ++i) {
        const char* p = test[i];

        sexp e = parse(&p);
        sexp r = eval(e,env);

        print_list_notation(str, sizeof(str)/sizeof(char), r);
        TEST(0 == strcmp(str, result[i]));
    }
}

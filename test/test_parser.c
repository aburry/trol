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


#include "constants.h"
#include "parser.h"
#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


void test_parse();
void test_print();
void test_print_list_notation();

int main(int argc, char* argv[]) {
    test_parse();
    test_print();
    test_print_list_notation();

    printf("\n");

    return 0;
}

void test_parse() {
    const char* str = "()";
    sexp t = parse(&str);
    TEST(c_bool(eq(t,ATOM_NIL())));

    str = "nil";
    t = parse(&str);
    TEST(c_bool(eq(t,ATOM_NIL())));

    str = "t";
    t = parse(&str);
    TEST(c_bool(eq(t,ATOM_T())));

    str = "'a";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(ATOM_QUOTE(),cons(symbol("a",1),ATOM_NIL())) )));

    str = "(quote a)";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(ATOM_QUOTE(),cons(symbol("a",1),ATOM_NIL())) )));

    str = "(a . b)";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(symbol("a",1),symbol("b",1)))));

    str = "(a b)";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(symbol("a",1),cons(symbol("b",1),ATOM_NIL())))));

    str = "(a b c)";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(symbol("a",1),cons(symbol("b",1), cons(symbol("c",1),ATOM_NIL()))))));

    str = "(a b c d)";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(symbol("a",1),cons(symbol("b",1), cons(symbol("c",1),cons(symbol("d",1),ATOM_NIL())))))));

    str = "(a . b )";
    t = parse(&str);
    TEST(c_bool(equal(t,cons(symbol("a",1),symbol("b",1)))));
}

void test_print() {
    const char* const str = "(eq . ((quote . (a . nil)) . ((quote . (b . nil)) . nil)))";
    const char* p = str;
    sexp t = parse(&p);
    char buf[200];

    int t1 = print_dot_notation(buf, sizeof(buf)/sizeof(char), t);
    TEST(t1 == strlen(str) && 0 == strcmp(str,buf));

    int t2 = print_dot_notation(buf, 5, t);
    TEST(5 <= t2);
}

void test_print_list_notation() {
    const char* const str = "(cond ((eq 'x 'z) 'first) ((atom 'a) 'second))";
    const char* p = str;
    sexp t = parse(&p);
    char buf[200];

    int t1 = print_list_notation(buf, sizeof(buf)/sizeof(char), t);
    TEST(t1 == strlen(str) && 0 == strcmp(str,buf));

    int t2 = print_list_notation(buf, 5, t);
    TEST(5 <= t2);
}

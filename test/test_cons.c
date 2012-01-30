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

#include <stdio.h>
#include <string.h>


void test_symbol();
void test_cons();
void test_atom();
void test_eq();

int main(int argc, char* argv[]) {
    test_symbol();
    test_cons();
    test_atom();
    test_eq();

    printf("\n");

    return 0;
}

void test_symbol() {
    const char* const str = "b-i-n-g-o";
    sexp t = symbol(str, strlen(str));

    TEST(0 == strcmp(c_str(t), str));
}

void test_cons() {
    const char* const strcar = "car";
    const char* const strcdr = "cdr";
    sexp pcar = symbol(strcar, strlen(strcar));
    sexp pcdr = symbol(strcdr, strlen(strcdr));
    sexp pcons = cons(pcar, pcdr);

    TEST(car(pcons) == pcar);
    TEST(cdr(pcons) == pcdr);
}

void test_atom() {
    const char* const strcar = "car";
    const char* const strcdr = "cdr";
    sexp pcar = symbol(strcar, strlen(strcar));
    sexp pcdr = symbol(strcdr, strlen(strcdr));
    sexp pcons = cons(pcar, pcdr);

    TEST(c_bool(atom(pcar)));
    TEST(false == c_bool(atom(pcons)));
}

void test_eq() {
    const char* const strcar = "car";
    const char* const strcdr = "cdr";
    sexp pcar = symbol(strcar, strlen(strcar));
    sexp pcar2 = symbol(strcar, strlen(strcar));
    sexp pcdr = symbol(strcdr, strlen(strcdr));
    sexp pcons = cons(pcar, pcdr);

    TEST(c_bool(eq(pcar,pcar)));
    TEST(c_bool(eq(pcar,pcar2)));
    TEST(false == c_bool(eq(pcar,pcdr)));
    TEST(false == c_bool(eq(pcons,pcar)));
}

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

#ifndef CONSTANTS_H
#define CONSTANTS_H

/*! \file constants.h
 */


#include "cons.h"

sexp ATOM_T();
sexp ATOM_NIL();
sexp ATOM_QUOTE();
sexp ATOM_DOT();	/* . */
sexp ATOM_ATOM();
sexp ATOM_EQ();
sexp ATOM_CAR();
sexp ATOM_CDR();
sexp ATOM_CONS();
sexp ATOM_COND();
sexp ATOM_LAMBDA();
sexp ATOM_LABEL();

#endif
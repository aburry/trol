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

#ifndef UTILS_H
#define UTILS_H

/*! \file  utils.h
 */

#include "cons.h"

sexp null(sexp c) ;
sexp append(sexp a, sexp b) ;
sexp pair(sexp a, sexp b) ;
sexp assoc(sexp key, sexp map) ;
sexp equal(sexp a, sexp b) ;

#endif
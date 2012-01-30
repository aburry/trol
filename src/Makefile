CFLAGS=-I. -g -W -Wall

all : lisp

lisp : main
	mv main lisp

main : main.c cons_impl.c constants.c eval.c parser.c utils.c

html :
	doxygen Doxyfile

clean :
	rm -f lisp *~

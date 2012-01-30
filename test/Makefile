CFLAGS=-I../src

all : test_cons test_parser test_eval
	./test_cons
	./test_parser
	./test_eval

test_cons : test_cons.c ../src/cons_impl.c ../src/constants.c

test_parser : test_parser.c ../src/cons_impl.c ../src/constants.c ../src/parser.c ../src/utils.c

test_eval : test_eval.c ../src/cons_impl.c ../src/constants.c ../src/parser.c ../src/utils.c ../src/eval.c

clean :
	rm -f test_cons test_parser test_eval

(quote a)

(quote (a b c))

(atom 'a)

(atom '(a b c))

(atom '())

(atom (atom 'a))

(atom '(atom 'a))

(eq 'a 'a)

(eq 'a 'b)

(eq '() '())

(car '(a b c))

(cdr '(a b c))

(cons 'a '(b c))

(cons 'a (cons 'b (cons 'c '())))

(car (cons 'a '(b c)))

(cdr (cons 'a '(b c)))

(cond ((eq 'a 'b) 'first) ((atom 'a) 'second))

((lambda (x) (cons x '(b))) 'a)

((lambda (x y) (cons x (cdr y))) 'z '(a b c))

((lambda (f) (f '(b c))) '(lambda (x) (cons 'a x)))

((label subst
  (lambda (x y z)
   (cond ((atom z) (cond ((eq z y) x) ('t z)))
    ('t (cons (subst x y (car z)) (subst x y (cdr z)))))))
 'm 'b '(a b (a b c) d))

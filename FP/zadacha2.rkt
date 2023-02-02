#lang racket

(define (prod l) (apply * l))        (define (sum l) (apply + l))

(define (foldr1 op op2 l)
  (if (null? (cdr l)) (op2 (car l)) (op (op2 (car l)) (foldr1 op op2 (cdr l)))
      )
  )

(define (max-metric op l)
  (define (calcFor func l)
    (cons (foldr1 + func l) func)
    )

  (define (findMaxPair a b)
    (if (> (car a) (car b)) a b)  
    )
  (cdr (foldr1 findMaxPair (lambda(x) x) (map (lambda(x) (calcFor x l)) op)))
  )

(max-metric (list sum prod) '((0 1 2) (3 4 5) (1337 0)))
(max-metric (list car sum)  '((1000 -1000) (29 1) (42)))
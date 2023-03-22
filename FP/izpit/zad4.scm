#lang racket

(define (foldr1 op curr)
  (if (null? (cdr curr)) (car curr)
      (op (car curr) (foldr1 op (cdr curr)))
      )
  )

(define (triple x y z)
  (cons x (cons y z)))

(define name car)
(define exp cadr)
(define real cddr)

(define (abs x)
  (if (< x 0) (- 0 x) x)
  )

(define (calcAbs tr)
  (map (lambda(x) (cons x (abs (- (exp x) (real x))))) tr)
  )

(define (findMin x y)
  (if (< (cdr x) (cdr y))
      x
      y
      )
  )

(define (rem x tr)
  (filter (lambda(y) (not (equal? y x))) tr)
  )

(define (sort tr)
  (if (null? tr)
      '()
      (cons (foldr1 findMin tr) (sort (rem (foldr1 findMin tr) tr)))
      )
  )
  
(define (finalScores tr)
  (sort (calcAbs tr))
  )

(define test (list (triple "d" 10 14) (triple "e" 5 8) (triple "m" 11 10) (triple "m2" 1 6)))
#lang racket


(define (accumulate op nv a b term next)
  (if (> a b) nv
      (op (term a)
          (accumulate op nv (next a) b term next))))
(define (accumulate-i op nv a b term next)
    (if (> a b) nv
        (accumulate-i op (op nv (term a)) (next a) b term next)))

(define (x+ x)
  (+ 1 x)
  )

(define (x= x)
  (x)
  )

(define (argmin f a b)
  (accumulate (lambda(x y) (if (< (f x) (f y)) x y)) 1000000 a b x= x+)
  )
;(argmin (lambda(x) (modulo x 7)) 45 50)


(define (best-pair a b)
  (define (isDivisiable a b)
    (if (= (remainder a b) 0) 1 0)
    )

  (define (smallerPair x y)
    (if (>= (car x) (car y)) x y)
    )
  (define (divs-count a)
    (accumulate + 0 2 a (lambda(x) (isDivisiable a x)) x+)
    )
  
  (define (best-pair-one-fixed first)
    (accumulate smallerPair (list 0 0 0) a b (lambda(x) (list (divs-count (+ first x)) first x))  x+)
    )
  
  (cdr (accumulate smallerPair (list 0 0 0) a b (lambda(x) (best-pair-one-fixed x))  x+))
  )
;(best-pair 10 20)


(define (integrate2 f a b c d dx dy)
  (define (integrate currY)
    (accumulate + 0 a b (lambda(x) (* (f x currY) dx)) (lambda(x) (+ x dx)))
    )
 
  (accumulate + 0 c d (lambda(y) (* (integrate y) dy)) (lambda(y) (+ y dy)))
  )
;(define pi 3.14159265359) (define (f x y) (+ x (sin y) 1)) (let ((res (integrate2 f 0 2 (- pi) pi 0.01 0.01))) (/ res pi))

;variant 1
(define (n-rooks board n)
  (define (checkRow row)
    (if (= (accumulate + 0 0 (- n 1) (lambda(y) (if (board row y n) 1 0)) x+) 1) 0 1)
    )
  (define (checkColumn column)
    (if (= (accumulate + 0 0 (- n 1) (lambda(x) (if (board x column n) 1 0)) x+) 1) 0 1)
    )

  (if (> (accumulate + 0 0 (- n 1) (lambda(x) (+ (checkColumn x) (checkRow x))) x+) 0) #f #t)
  )

;(define (board1 x y n)(= (remainder (+ x 2) n) y)) (n-rooks board1 5)
;(define (board2 x y n)(= (min (+ x 2) (- n 1)) y)) (n-rooks board2 5)
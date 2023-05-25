#lang racket

(define (accumulate op nv a b term next)
  (if (> a b) nv
      (op (term a) (accumulate op nv (next a) b term next))))

(define (accumulate-i op nv a b term next)
  (if (> a b) nv
      (accumulate-i op (op nv (term a)) (next a) b term next)))

(define (filter p l)
  (cond ((null? l) l)
        ((p (car l)) (cons (car l) (filter p (cdr l))))
        (else (filter p (cdr l)))))

(define (foldr op nv l)
  (if (null? l) nv
      (op (car l) (foldr op nv (cdr l)))))

(define (foldrMap op op2 nv l)
  (if (null? l) nv
      (op (op2 (car l)) (foldrMap op op2 nv (cdr l)))))

(define (foldl op nv l)
  (if (null? l) nv
      (foldl op (op nv (car l)) (cdr l))))

(define (foldr1 op l)
  (if (null? (cdr l)) (car l)
      (op (car l) (foldr1 op (cdr l)))))

(define (foldl1 op l)
  (foldl op (car l) (cdr l)))

(define firstElement car)

(define (1+ x)
  (+ 1 x))

(define (myOr x y)
  (if x x y))

(define (boolToNum x)
  (if x 1 0))

(define (sizeList l)
  (foldrMap + (lambda(x) 1) 0 l))

(define (removeLastEl l)
  (if (null? (cdr l)) '()
      (cons (car l) (removeLastEl (cdr l)))))

(define removeFirstEl cdr)

(define (removeFirstAndLastEl l)
  (removeLastEl (removeFirstEl l))
  )

(define (findFirstTerm abstraction)
  ;(display (cons "First: " abstraction))
  ;(display "\n")
  
  (define (recursion abstraction num starting)
    (define curr (car abstraction))
    
    (if (and (eqv? num 0) (not starting))
      '()
      (cond
        [(eqv? curr #\() (cons curr (recursion (cdr abstraction) (+ num 1) #f))]
        [(eqv? curr #\)) (cons curr (recursion (cdr abstraction) (- num 1) #f))]
        [else (cons curr (recursion (cdr abstraction) num #f))]
        )
      )
    )
  
   (removeFirstAndLastEl (recursion abstraction 0 #t))
  )

(define (findLastTerm abstraction)
  ;(display (cons "Last: " abstraction))
  ;(display "\n")

  (define (recursion abstraction num starting)
    (define curr (car abstraction))
    
    (if (and (eqv? num 0) (not starting))
        abstraction
        (cond
          [(eqv? curr #\() (recursion (cdr abstraction) (+ num 1) #f)]
          [(eqv? curr #\)) (recursion (cdr abstraction) (- num 1) #f)]
          [else (recursion (cdr abstraction) num #f)]
          )
        )
    )
  
  (removeFirstAndLastEl (recursion abstraction 0 #t))
  )

(define (isLeaf abstraction)
  ;(display (cons (cons "Leaf: " abstraction) (eqv? (sizeList abstraction) 1)))
  ;(display "\n")
  
  (eqv? (sizeList abstraction) 1)
  )

(define skip1 cdr)
(define skip2 cddr)

(define (createTree abstraction)
  (define curr (car abstraction))
  
  (if (null? abstraction)
      '()
      (cond
        [(eqv? curr #\\) (cons (cadr abstraction) (createTree (skip2 abstraction)))]
        [(eqv? curr #\.) (createTree (skip1 abstraction))]
        [(isLeaf abstraction) (cons (car abstraction) '())]
        [else (cons (cons (createTree (findFirstTerm abstraction)) (createTree (findLastTerm abstraction))) '())]
        )
      )
  )



(define mapKey car)
(define mapValue cdr)

(define (mapKeyExists? key myMap)
  (not (empty? (filter (lambda(x) (equal? key (mapKey x))) myMap))))

(define (mapValueExists? key myMap)
  (not (empty? (filter (lambda(x) (equal? key (mapValue x))) myMap))))

(define (mapFindValue key myMap)
  (if (null? myMap)
      '()
      (mapValue (firstElement
                 (filter (lambda(x) (equal? key (mapKey x))) myMap)))))

(define (mapGetKeys myMap)
  (if (null? myMap)
      '()
      (map mapKey myMap)))

(define (mapGetValues myMap)
  (map mapValue myMap))

(define (mapAppendElement key value myMap)
  (cons (cons key value) myMap))

(define (findNewType typeMap)
  (firstElement (accumulate append '() 1 26
              (lambda(x) (if (mapValueExists? x typeMap) '() (list x))) 1+)))

(define (appendVariable node typeMap)
  (if (mapKeyExists? node typeMap)
      typeMap
      (mapAppendElement node (findNewType typeMap) typeMap)))

(define (mapChangeValue key newValue typeMap)
  (map (lambda(x)
         (if (eqv? (mapKey x) key)
             (cons key newValue)
             (x)))))

(define type car)
(define which cadr) ; 0 - leaf, 1 - var, 2 - func
(define variable caddr) 
(define variable1 caddr)
(define variable2 cadddr)

(define (createType0)
  (list 0))
(define (appendType0 type typeMap)
  (mapAppendElement type (createType0) typeMap))

(define (createType1 variable)
  (list 1 variable))
(define (appendType1 type variable typeMap)
  (mapAppendElement type (createType1 variable) typeMap))

(define (createType2 variable1 variable2)
  (list 2 variable1 variable2))
(define (appendType2 type variable1 variable2 typeMap)
  (mapAppendElement type (createType2 variable1 variable2) typeMap))

(define (mapTypesOfTerms node)
  (define (recursion  node variableMap)
    (define curr (car node))
  
    (if (null? node)
        variableMap
        (cond
          [(cons? curr) (appendVariable node (recursion (cdr curr) (recursion (car curr) (appendVariable curr variableMap))))]
          [(isLeaf node) (appendVariable curr variableMap)]
          [else (appendVariable node (recursion (cdr node) (appendVariable curr variableMap)))]
          )
        )
    )

  (recursion node '())
  )


(define (applyLambda type term1 term2 typeMap)
  (appendType2 type term1 term2 typeMap)
  )

(define (unification type1 type2 variableMap)
  ;;TO TEST

  (define term1 (mapFindValue type1 variableMap))
  (define term2 (mapFindValue type2 variableMap))
  
  (define wh1 (which term1))
  (define wh2 (which term2))
  
  
  (cond
    [(eqv? wh1 0) (mapChangeValue type1 (createType1 term2) variableMap)]
    [(eqv? wh2 0) (mapChangeValue type2 (createType1 type1) variableMap)]
    
    [(eqv? wh1 1) (unification (variable term1) type2 variableMap)]
    [(eqv? wh2 1) (unification term1 (variable type2) variableMap)]

    [else (unification (variable2 term1) (variable2 term2) (unification (variable1 term1) (variable1 term2) variableMap))]
    )
     
  )

(define (createTreeWithTypes node)
  (define variableMap (mapTypesOfTerms node))
  
  (define (recursion node typeMap)
    (define curr (car node))
    (define nodeType (mapFindValue node variableMap))
    (define currType (mapFindValue curr variableMap))
  
    (if (null? node)
        '()
        (cond
          [(cons? node)
           (define tempTypeMap (recursion (cdr node) (recursion (car node) typeMap)))
           (define newTypeMap (appendType2 nodeType (cdr node) currType tempTypeMap)) ;;TO DO
           (unification (mapFindValue (car node) variableMap) (mapFindValue (car node) variableMap) newTypeMap)
           ]
          [(isLeaf node) (list (createType0 currType))]
          [else (applyLambda currType )]
          )
        )
    )

  (recursion node '())
  )


;;tests
;\xy.x
(define lambdaExpr1 (list #\\ #\x #\\ #\y #\. #\x))
(define treeLambdaExpr1 (createTree lambdaExpr1))
;\xy.x
(define lambdaExpr4 (list #\\ #\x #\\ #\y #\. #\( #\x #\) #\( #\x #\)))
(define treeLambdaExpr4 (createTree lambdaExpr4))

;\xy.xyz
;\x\y.((x)(y))(z)
(define lambdaExpr2 (list #\\ #\x #\\ #\y #\. #\( #\( #\x #\) #\( #\y #\) #\) #\( #\z #\)))
(define treeLambdaExpr2 (createTree lambdaExpr2))

;\xy.xyxy
;\x\y.((x)(y))((x)(y))
(define lambdaExpr6 (list #\\ #\x #\\ #\y #\. #\( #\( #\x #\) #\( #\y #\) #\) #\( #\( #\x #\) #\( #\y #\) #\)))
(define treeLambdaExpr6 (createTree lambdaExpr6))

;\xy.x\z.yz
;\x\y.(x)((\z.y)(z))
(define lambdaExpr3 (list #\\ #\x #\\ #\y #\. #\( #\x #\) #\( #\( #\\ #\z #\. #\y #\) #\( #\z #\) #\)))
(define treeLambdaExpr3 (createTree lambdaExpr3))

;(display treeLambdaExpr3)

;(createTreeWithTypes treeLambdaExpr3)


;(giveNewType (list 1 2 5))

;(define testMap (list (cons "b" 1) (cons "c" 7) (cons "a" 6)))
;(findTypeOfVariable "d" (appendVariable "d" testMap))

;(removeLastEl lambdaExpr1)

;(findFirstTerm lambdaAbstr)
;(findLastTerm lambdaAbstr)

;(display lambdaExpr3)

;(display (mapTypesOfTerms treeLambdaExpr3))

(unification 1 2 )
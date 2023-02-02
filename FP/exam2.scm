#lang racket

;general functions
(define (accumulate op nv a b term next)
  (if (> a b) nv (op (term a) (accumulate op nv (next a) b term next)))
  )
(define (foldr op nv l)
  (if (null? l) nv
      (op (car l) (foldr op nv (cdr l))))
  )
(define (foldr1 op l)
  (if (null? (cdr l)) (car l) (op (car l) (foldr1 op (cdr l))))
  )

(define (filter op l)
  (if (null? l) '()
      (if (op (car l))
          (cons (car l) (filter op (cdr l)))
          (filter op (cdr l))
          )
      )
  )

(define (atom? x)
  (not (list? x))
  )

;problem 1


;; [otbor1, otbor2, golove1, golove2]

(define team1 car)
(define team2 cadr)
(define goals1 caddr)
(define goals2 cadddr)


(define (listOfTeamNames2 tour)
  (define (findIfAdded search l)
    (foldr (lambda (x prev) (if (eq? x search) #t prev)) #f l) 
    )
  (define (listOfTeamNames1)
    (foldr (lambda(x y) (if (findIfAdded (team1 x) y) y (cons (team1 x) y))) '() tour)
    )
  
  (foldr (lambda(x y) (if (findIfAdded (team2 x) y) y (cons (team2 x) y))) (listOfTeamNames1) tour)
  )

(define (calcGoodGoalsForTeam team tour)
  (foldr + 0 (map (lambda(x) (if (eq? (team1 x) team) (goals1 x) (if (eq? (team2 x) team) (goals2 x) 0))) tour))
  )

(define (calcBadGoalsForTeam team tour)
  (foldr + 0 (map (lambda(x) (if (eq? (team1 x) team) (goals2 x) (if (eq? (team2 x) team) (goals1 x) 0))) tour))
  )

(define (filterGamesForTeam team tour)
  (filter (lambda(x) (or (eq? (team1 x) team) (eq? (team2 x) team))) tour)
  )

(define (filterGamesForTeamAndSwap team tour)
  (map (lambda(x) (if (eq? (team2 x) team) (list (team2 x) (team1 x) (goals2 x) (goals1 x)) x)) (filterGamesForTeam team tour))
  )

(define (calcPointsForTeam team tour)
  (foldr + 0 (map (lambda(x) (if (> (goals1 x) (goals2 x)) 3 (if (eq? (goals1 x) (goals2 x)) 1 0))) (filterGamesForTeamAndSwap team tour)))
  )

(define (createTeamsMap tour)
  (map (lambda (x) (list x (- (calcGoodGoalsForTeam x tour) (calcBadGoalsForTeam x tour)) (calcPointsForTeam x tour))) (listOfTeamNames2 tour))
  )

(define team car)
(define goals cadr)
(define points caddr)

(define (findMaxGoals teams)
  (foldr1 max (map (lambda(x) (goals x)) teams))
  )

(define (findMinPointsForMaxGoals teams)
  (foldr1 min (map (lambda(x) (points x)) (filter (lambda(x) (eq? (findMaxGoals teams) (goals x))) teams)))
  )

(define (maxGoalMinPoints tour)
  (define teams (createTeamsMap tour))
  (team (car (filter (lambda(x) (and (eq? (findMinPointsForMaxGoals teams) (points x)) (eq? (findMaxGoals teams) (goals x)))) teams)))
  )





(define (doesItSurpass t1 t2)
  (define findGames (filter (lambda(x) (eq? t2 (team2 x))) (filterGamesForTeamAndSwap t1)))

  (if (null? findGames)
      #f
      (not (null? (filter (lambda(x) (<= (goals1 x) (goals2 x))) findGames)))
      )
  )

;(define (surpassSelf tour))

;(define tournament (list '("A" "B" 1 0) '("B" "C" 4 1) '("C" "B" 3 3) '("B" "A" 1 2) '("A" "C" 0 1)))
;(maxGoalMinPoints tournament)







;problem 2
;imame spisyk ot spisyci koito za vseki vryh, 1vo pazim koi e vyrha, 2ro kakva mu e funkciqta, i sled tova decata mu
;dyrvoto e korenuvano ot 0
;ima primer otdolu

(define Node car)
(define Func cadr)
(define Children caddr)


(define (mapTree tree n)
  (define (findConForNode node)
    (car (filter (lambda(x) (eq? node (Node x))) tree))
    )

  (define (findIfAdded search l)
    (foldr (lambda (x prev) (if (eq? x search) #t prev)) #f l) 
    )

  (define (mapTreeDFS currNode currVal)
    (define newVal ((Func (findConForNode currNode)) currVal))

    (if (null? (Children (findConForNode currNode))) newVal
        (foldr cons '() (map (lambda(x) (mapTreeDFS x newVal)) (Children (findConForNode currNode))))
        )
    )

  (define (flattenList lst)
    (if (null? (cdr lst))
        (if (atom? (car lst))
            (list (car lst))
            (flattenList (car lst))
            )
     
        (if (atom? (car lst))
            (append (list (car lst)) (flattenList (cdr lst)))
            (append (flattenList (car lst)) (flattenList (cdr lst)))
            )
        )
    )

  (define (removeRepetitions lst newLst)
    (if (null? lst)
        newLst
        (if (findIfAdded (car lst) newLst)
            (removeRepetitions (cdr lst) newLst)
            (removeRepetitions (cdr lst) (cons (car lst) newLst))
            )
        )
    )

  (removeRepetitions (flattenList (mapTreeDFS 0 n)) '())
  )

;(define tree (list (list 0 (lambda(x) (+ 1 x)) '(1 2))
;                   (list 1 (lambda(x) (* 2 x)) '(3 4))
;                   (list 2 (lambda(x) (+ 1 x)) '())
;                   (list 3 (lambda(x) (* 2 x)) '())
;                   (list 4 (lambda(x) (- x 3)) '(5))
;                   (list 5 (lambda(x) (- x 3)) '())))
;(mapTree tree 2)







;problem3

(define head stream-first)
(define tail stream-rest)

(define (createBraidStreams a b c curr)
  (stream-cons (list (head a) (head b) (head c))
               (if (eq? (- curr (* (quotient curr 2) 2)) 0)
                   (createBraidStreams (tail b) (tail a) (tail c) (+ 1 curr))
                   (createBraidStreams (tail a) (tail c) (tail b) (+ 1 curr))
                   )
               )
  )

(define (braidStreams a b c)
  (createBraidStreams a b c 0)
  )


;(define (natural x)(stream-cons x (natural (+ 3 x))))
;(define test (braidStreams (natural 0) (natural 1) (natural 2)))
;(stream-first test)
;(stream-first (stream-rest test))
;(stream-first (tail (stream-rest test)))

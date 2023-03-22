member x l = any (== x) l


-- 1 --

genPrimes :: Integral a => [a] -> [a]
genPrimes currPrimes = next : (genPrimes (next:currPrimes))
    where next = head [x | x <- [2..], not $ any (\y -> 0 == (rem x y)) currPrimes]

primes = genPrimes []

genPrimitive pr = val : genPrimitive nextPr
    where first0 = head [(x, x*2) | (x, y) <- pr, y == 0]
          next = first0 : [(x, x * (primes !! y)) | (x, y) <- pr, y /= 0] 
          val = minimum $ map snd next
          which = head [x | (x, y) <- next, y == val]
          nextPr = map (\(x, y) -> if (x == which) then (x, y+1) else (x, y)) pr

primitive 1 = primes
primitive k = genPrimitive (zip (primitive (k - 1)) [0, 0..])



-- 2 --

data NDA = NDA {set :: Set, beg :: Q, allDelta :: AllDelta, fin :: Set} 
type Set = [Q]
type Delta = (Q, Alphabet, Q)
type AllDelta = [Delta]
type Q = Int
type Alphabet = Char

genCheckForCycles nda visited curr
    | hasCycle = True
    | curr == [] = False
    | otherwise = genCheckForCycles nda (curr ++ next) next
    where next = [y | (x, _, y) <- (allDelta nda), member x curr, member y (reachingFinal nda)]
          hasCycle = or [x == y | x <- visited, y <- next]

checkForCycles nda = genCheckForCycles nda [beg1] [beg1]
    where fins = (fin nda)
          beg1 = (beg nda)


genReachingFinal nda visited [] = []
genReachingFinal nda visited curr = next ++ (genReachingFinal nda (curr ++ next) next)
    where next = [x | (x, _, y) <- (allDelta nda), member y curr, not $ member x visited]

reachingFinal nda = (genReachingFinal nda fins fins) ++ fins
    where fins = (fin nda)

findWords nda curr = if isFinal then ("" : next) else next
    where isFinal = member curr (fin nda)
          next = [letter : words | (x, letter, y) <- allDelta nda, x == curr, member y (reachingFinal nda), words <- (findWords nda y)]
    
detectedWords :: NDA -> Maybe [[Alphabet]]
detectedWords nda 
    | checkForCycles nda = Nothing
    | otherwise = Just $ findWords nda (beg nda)

testQ = [0, 1, 2, 3]
testDelta = [(0, 'a', 1) , (1, 'a', 1), (0, 'a', 2), (0, 'b', 2), (2, 'a', 3)]--, (1, 'a', 2)]
testFin = [0, 2, 3]
testBeg = 0
testNda = NDA {set = testQ, beg = testBeg, allDelta = testDelta, fin = testFin} 



-- 3 --

-- calcCurr (f:fs) args curr 
--     |fs == [] = (args !! curr) `f` (args !! next)   
--     |otherwise = (args !! curr) `f` (calcCurr fs args next)
--     where next = (rem (curr+1) (length args))

    
calcCurr [f] args curr = (args !! curr) `f` (args !! next)   
    where next = (rem (curr+1) (length args))
    
calcCurr (f:fs) args curr = (args !! curr) `f` (calcCurr fs args next)
    where next = (rem (curr+1) (length args))

findMax [] args = 0
findMax funcs [] = 0
findMax funcs args = maximum [calcCurr funcs args x | x <- [0..(length args - 1)]]

testFuncs = [(+), (*), (-)]
testArgs = [2, 5]
testFindMax = findMax testFuncs testArgs
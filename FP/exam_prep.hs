getSegment :: (Ord a) => [a] -> [a]
getSegment (x:y:sh) 
    | x <= y = [x]
    | otherwise = x : getSegment (y:sh)

segments :: (Ord a) => [a] -> [[a]]
segments s = first : segments rest
    where 
        first = getSegment s
        rest = drop (length first) s

fillSegment :: (Num a, Enum a) => [a] -> [a]
fillSegment [x] = [x..0]
fillSegment (s:sh) = s:(fillSegment sh)


fillSegments :: (Num a, Enum a, Ord a) => [a] -> [a]
fillSegments s = real ++ fillSegments rest
    where 
        first = getSegment s
        real = first ++ (reverse [0..(last first-1)])
        rest = drop (length first) s
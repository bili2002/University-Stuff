--module Main where
main1 = "Hello, World!"


addition :: Integer -> Integer -> Integer
addition i n
    | i < n = i + addition (i + 1) n
    | otherwise = i

import Control.Monad

-- UTILITY ---
revMapTuple2 arg (func1, func2) = (func1 arg, func2 arg)
revMapTuple3 (func1, func2, func3) arg = (func1 arg, func2 arg, func3 arg)

mapTuple2 func (t1, t2) = (func t1, func t2)
mapTuple3 func (t1, t2, t3) = (func t1, func t2, func t3)

mtTuple3 (a1, a2, a3) (b1, b2, b3) = ((a1 > b1), (a2 > b2), (a3 > b3)) --more than

leTuple3 (a1, a2, a3) (b1, b2, b3) = ((a1 <= b1), (a2 <= b2), (a3 <= b3)) --less or equal

andTuple3 (a1, a2, a3) = a1 && a2 && a3

zipWithTuple3 func (a1, a2, a3) (b1, b2, b3) = ((a1 `func` b1), (a2 `func` b2), (a3 `func` b3))

find p = head . filter p

average :: [Float] -> Float
average l = (sum l) / (fromIntegral $ length l)

--- TYPES ---

type Agency = [Customer] 
data Customer = Customer { customerName :: Name, trips :: [Trip], points :: Float }
    deriving (Eq, Show)

data Trip = Trip { destination :: Destination, price :: Float, status :: Status, activity :: Activity, policy :: Policy}
    deriving (Eq, Show)

data Activity = Activity { activityName :: Name, period :: Word }
    deriving (Eq, Show)

data Status = Upcoming | Canceled | InProgress | Finished  
    deriving (Eq, Show)    

data Policy = Flexible | Moderate | Strict
    deriving (Eq, Show)


data Thresholds = Thresholds { moneyThreshold :: Float, durationThreshold :: Word, loyaltyPointsThreshold :: Float }
    deriving (Eq, Show)

type Destination = String
type Name = String



--- MAIN A) ---

getTotalMoney :: Customer -> Float
getTotalMoney = sum . map price . trips

getTotalDuration :: Customer -> Word
getTotalDuration = sum . map (period . activity) . trips

compareWithThresholds :: Thresholds -> Customer -> Bool
compareWithThresholds Thresholds { moneyThreshold, durationThreshold, loyaltyPointsThreshold }
    = andTuple3 . leTuple3 (moneyThreshold, durationThreshold, loyaltyPointsThreshold) . revMapTuple3 (getTotalMoney, getTotalDuration, points) 
                                 
notCancelingTrips :: Customer -> Bool
notCancelingTrips = not . any (( == Canceled ) . status) . trips 

notDuplicatingDestination :: Customer -> Bool
notDuplicatingDestination customer = (length $ trips customer) == 
                                      sum [if (destination x == destination y) then 1 else 0 | x <- trips customer, y <- trips customer]
    
freeTripEligibleCustomers :: Agency -> Thresholds -> [Customer] 
freeTripEligibleCustomers agency thresholds = 
    filter (andTuple3 . revMapTuple3 (notCancelingTrips, notDuplicatingDestination, (compareWithThresholds thresholds))) agency




--- MAIN B) ---

findTripByDestination :: Destination -> Customer -> Trip
findTripByDestination currDestination = (find (((== currDestination) .) destination)) . trips

getPolicy :: Trip -> Float
getPolicy trip
    | policy trip == Flexible = 0.7
    | policy trip == Moderate = 0.4
    | policy trip == Strict = 0.1

getPriceByPolicy :: Destination -> Customer -> Float 
getPriceByPolicy currDestination = liftM2 (*) (price . (findTripByDestination currDestination)) (getPolicy . (findTripByDestination currDestination))

getNotCanceledTrips :: Customer -> Int
getNotCanceledTrips = length . filter (((== Canceled) . ) status) . trips

getK :: Customer -> Float
getK = liftM2 (/) (fromIntegral . length . trips) (fromIntegral . getNotCanceledTrips)

mapStatusByDestination :: Destination -> Agency -> [Status]
mapStatusByDestination currDestination = map (status . (findTripByDestination currDestination)) . filter (any (((== currDestination) .) destination) . trips) 

getQuotient :: Destination -> Agency -> Float
getQuotient destination = liftM2 (/) (fromIntegral . length . filter (== Canceled) . mapStatusByDestination destination) (fromIntegral . length . mapStatusByDestination destination)

getCanceledAverage :: Customer -> Float
getCanceledAverage = average . map (price) . filter (((== Canceled) . ) status) . trips

getFee :: Customer -> Destination -> Agency -> Float
getFee customer currDestination agency = (getCanceledAverage (find (((== currDestination) .) destination))) * (getQuotient currDestination agency)

getRefund :: Agency -> Name -> Destination -> Float
getRefund agency customerName destination = 0




--- TESTS ---

trip1 = Trip "sofia" 1.2 Upcoming (Activity "volei" 3) Flexible
trip2 = Trip "ruse" 2.2 InProgress (Activity "futbol" 4) Flexible
trip3 = Trip "pleven" 3.8 Canceled (Activity "basketbol" 3) Flexible
trip4 = Trip "sofia" 3.8 Finished (Activity "igri" 8) Moderate
trip5 = Trip "varna" 10.8 Finished (Activity "more" 1) Moderate
trip6 = Trip "shumen" 1 Finished (Activity "programirane" 8) Strict
trip7 = Trip "london" 100.10 Finished (Activity "razhodka" 100) Strict

customer1 = Customer "ivan" [trip1, trip2] 5.2
customer2 = Customer "moni" [trip7] 5
customer3 = Customer "gosho" [trip1, trip2, trip4] 8 --duplicating
customer4 = Customer "maria" [trip1, trip2, trip3] 9 --canceled
customer5 = Customer "ivana" [trip5] 9 --low duration
customer6 = Customer "woro" [trip1, trip2] 2.3 --low points
customer7 = Customer "vili" [trip1, trip6] 10.1 --low money
customer8 = Customer "aleks" [trip1, trip2, trip5, trip6] 15

agency = [customer1, customer2, customer3, customer4, customer5, customer6, customer7, customer8]

thresholds = Thresholds 2.5 2 4.1

testGetTotalMoney :: Bool
testGetTotalMoney = getTotalMoney customer1 == 3.4

testGetTotalDuration :: Bool
testGetTotalDuration = getTotalDuration customer1 == 6

testFreeTripEligibleCustomers :: Bool
testFreeTripEligibleCustomers = (freeTripEligibleCustomers agency thresholds) == [customer1, customer2, customer8]

testEverything :: Bool
testEverything = testGetTotalMoney && testGetTotalDuration && testFreeTripEligibleCustomers

printAnswerA = [customerName x | x <- (freeTripEligibleCustomers agency thresholds)]
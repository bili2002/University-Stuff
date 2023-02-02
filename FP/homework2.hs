import Control.Monad

-- UTILITY ---

revMapTuple3 (func1, func2, func3) arg = (func1 arg, func2 arg, func3 arg)
mapTuple3 func (t1, t2, t3) = (func t1, func t2, func t3)

mtTuple3 (a1, a2, a3) (b1, b2, b3) = ((a1 > b1), (a2 > b2), (a3 > b3)) --more than
leTuple3 (a1, a2, a3) (b1, b2, b3) = ((a1 <= b1), (a2 <= b2), (a3 <= b3)) --less or equal
andTuple3 (a1, a2, a3) = a1 && a2 && a3

zipWithTuple3 func (a1, a2, a3) (b1, b2, b3) = ((a1 `func` b1), (a2 `func` b2), (a3 `func` b3))

find :: (a -> Bool) -> [a] -> a --must exist
find p = head . filter p

average :: [Float] -> Float
average [] = 0
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

findDestination :: Destination -> Customer -> Trip
findDestination currDestination = (find (((== currDestination) . ) destination)) . trips

findIfDestinationExists :: Destination -> Customer -> Bool
findIfDestinationExists currDestination = (any (((== currDestination) . ) destination)) . trips

eqTrip :: Trip -> Trip -> Bool
eqTrip trip1 trip2 = (destination trip1 == destination trip2) && (price trip1 == price trip2) && (activity trip1 == activity trip2)

findTrip :: Trip -> Customer -> Trip
findTrip trip = (find (eqTrip trip)) . trips

findIfTripExists :: Trip -> Customer -> Bool
findIfTripExists trip = (any (eqTrip trip)) . trips

findCustomerByName :: Name -> Agency -> Customer
findCustomerByName currName = find (((== currName) .) customerName)

getPolicy :: Trip -> Float
getPolicy trip
    | policy trip == Flexible = 0.7
    | policy trip == Moderate = 0.4
    | policy trip == Strict = 0.1

getPriceByPolicy :: Trip -> Float 
getPriceByPolicy = liftM2 (*) price getPolicy

filterNotCanceledTrips :: Customer -> [Trip]
filterNotCanceledTrips = filter (((/= Canceled) . ) status) . trips

filterCanceledTrips :: Customer -> [Trip]
filterCanceledTrips = filter (((== Canceled) . ) status) . trips

getNotCanceledTripsCnt :: Customer -> Int
getNotCanceledTripsCnt = length . filterNotCanceledTrips

getK :: Customer -> Float
getK = liftM2 (/) (fromIntegral . getNotCanceledTripsCnt) (fromIntegral . length . trips)

mapStatusByTrip :: Trip -> Agency -> [Status]
mapStatusByTrip trip = map (status . (findTrip trip)) . filter (findIfTripExists trip) 

getQuotient :: Trip -> Agency -> Float
getQuotient trip = liftM2 (/) ((+1) . fromIntegral . length . filter (== Canceled) . mapStatusByTrip trip) (fromIntegral . length . mapStatusByTrip trip)

getCanceledAverage :: Customer -> Float
getCanceledAverage = average . map (price) . filterCanceledTrips

getFee :: Customer -> Trip -> Agency -> Float
getFee customer trip agency = (getCanceledAverage customer) * (getQuotient trip agency)

getRefundByCustomer :: Agency -> Customer -> Destination -> Maybe Float
getRefundByCustomer agency customer currDestination 
    | tripExists == False = Nothing
    | otherwise = case (status trip) of
        InProgress -> Just 0
        Upcoming -> Just (max ((getK customer) * (getPriceByPolicy trip) - (getFee customer trip agency))  0.0)
        _ -> Nothing
    where 
        tripExists = findIfDestinationExists currDestination customer
        trip = findDestination currDestination customer

getRefund :: Agency -> Name -> Destination -> Maybe Float
getRefund agency currName currDestination = getRefundByCustomer agency (findCustomerByName currName agency) currDestination



--- TESTS ---
--- A) ---

trip1 = Trip "sofia" 1.2 Upcoming (Activity "volei" 3) Flexible
trip2 = Trip "ruse" 2.2 InProgress (Activity "futbol" 4) Flexible
trip3 = Trip "ivanovo" 4.2 Canceled (Activity "basketbol" 3) Flexible
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
testGetTotalDuration = getTotalDuration customer1 == 7

testFreeTripEligibleCustomers :: Bool
testFreeTripEligibleCustomers = (freeTripEligibleCustomers agency thresholds) == [customer1, customer2, customer8]

printAnswerA = [customerName x | x <- (freeTripEligibleCustomers agency thresholds)]


--- B) ---

tripB1 = Trip "pleven" 15.8 InProgress (Activity "igri" 8) Moderate
tripB2 = Trip "pleven" 15.8 Canceled (Activity "igri" 8) Moderate
tripB3 = Trip "pleven" 15.8 Upcoming (Activity "igri" 8) Moderate

customerB1 = Customer "ivan1" [trip1, trip2, tripB1] 5.2
customerB2 = Customer "ivan2" [trip1, trip4, tripB2] 5
customerB3 = Customer "ivan3" [trip1, trip3, tripB2] 5.2
customerB4 = Customer "ivan4" [trip1, trip5, tripB3] 6

agencyB = [customerB1, customerB2, customerB3, customerB4]


testEqTrip :: Bool
testEqTrip = (eqTrip trip1 trip2) == False && (eqTrip tripB1 tripB2) == True

testFindIfTripExists :: Bool
testFindIfTripExists = (findIfTripExists trip3 customerB1) == False && (findIfTripExists trip2 customerB1) == True

testFindTrip :: Bool
testFindTrip = status (findTrip tripB1 customerB1) == (status tripB1)

testFindCustomerByName :: Bool
testFindCustomerByName = points (findCustomerByName (customerName customerB1) agencyB) == (points customerB1)

testGetPriceByPolicy :: Bool
testGetPriceByPolicy = (getPriceByPolicy tripB1) == ((price tripB1) * (getPolicy tripB1)) 

testFilterNotCanceledTrips :: Bool
testFilterNotCanceledTrips = filterNotCanceledTrips customerB2 == [trip1, trip4]

testGetK :: Bool
testGetK = (getK customerB2 > 0.66) && (getK customerB2 < 0.67)

testMapStatusByTrip :: Bool
testMapStatusByTrip = (mapStatusByTrip tripB2 agencyB) == [InProgress, Canceled, Canceled, Upcoming]

testGetQuotient :: Bool
testGetQuotient = (getQuotient tripB1 agencyB) == 0.5

testGetCanceledAverage :: Bool
testGetCanceledAverage = getCanceledAverage customerB3 == 10

testGetFee :: Bool
testGetFee = getFee customerB3 tripB3 agencyB == 5

testGetRefund :: Bool
testGetRefund = (getRefund agencyB "ivan3" "pleven" == Nothing) && (getRefund agencyB "ivan3" "shumen" == Nothing) 
             && (getRefund agencyB "ivan1" "pleven" == Just 0.0) && (getRefund agencyB "ivan4" "pleven" == Just 6.32)



testEverything :: Bool
testEverything = testGetTotalMoney && testGetTotalDuration && testFreeTripEligibleCustomers && testEqTrip && testFindIfTripExists 
                                   && testFindTrip && testFindCustomerByName && testGetPriceByPolicy && testFilterNotCanceledTrips
                                   && testGetK && testMapStatusByTrip && testGetQuotient && testGetCanceledAverage && testGetFee
                                   && testGetRefund
                                   

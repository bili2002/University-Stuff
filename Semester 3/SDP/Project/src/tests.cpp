#include "tests.h"

void Tests::testTrip() {
    int n = 9;
    std::vector<std::string> citiesString = {"Sofia", "Pazardzhik", "Plovdiv", "Dimitrovgrad", "StaraZagora", "NovaZagora", "Yambol", "Karnobat", "Burgas"};
    std::vector<std::pair<std::string, std::string>> directLinks = {
        {"Sofia", "Plovdiv"},
        {"Plovdiv", "NovaZagora"},
        {"Dimitrovgrad", "NovaZagora"},
        {"StaraZagora", "Yambol"},
        {"NovaZagora", "Burgas"}
    };

    std::vector<std::string> wantedCities = {"Plovdiv", "StaraZagora", "Yambol"};

    Trip trip(n, citiesString, directLinks, wantedCities);
    trip.printAnswer();
}

void Tests::testCity() {    
    City city("testCity");
    city.printAnswer();
}

void Tests::testBox() {
    Box box;
    box.printAnswer();
}
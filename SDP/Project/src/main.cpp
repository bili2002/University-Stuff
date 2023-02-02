#include <iostream>
#include "trip.h"
#include "city.h"
#include "box.h"

void findTrip() {
    int n;
    std::cout<<"Enter City Count: ";
    std::cin>>n;

    std::vector<std::string> cities(n);
    std::cout<<"Enter Cities: \n";
    for (int i=0; i<n; i++) {
        std::cin>>cities[i];
    }

    
    int m;
    std::cout<<"Enter Direct Link Count: ";
    std::cin>>m;

    std::vector<std::pair<std::string, std::string>> directLinks(m);
    std::cout<<"Enter Direct LinkS: \n";
    for (int i=0; i<m; i++) {
        std::cin>>directLinks[i].first>>directLinks[i].second;
    }
    

    int l;
    std::cout<<"Enter Wanted Cities Count: ";
    std::cin>>l;

    std::vector<std::string> wantedCities(l);
    std::cout<<"Enter Wanted Cities Link: \n";
    for (int i=0; i<l; i++) {
        std::cin>>wantedCities[i];
    }

    Trip trip(n, cities, directLinks, wantedCities);
    trip.printAnswer();
}

void findPathInCity(const std::string& name) {
    City city(name);
    city.printAnswer();
}

Box createBox() {
    Box box;
    return box;
}

void minimizeBoxes(Box& box) {
    box.minimizeBoxes();
    box.printAnswer();
}

int main() {
    //findTrip();
    //findPathInCity("testCity.txt");
    //Box box = createBox();
    //minimizeBoxes(box);

    return 0;
}
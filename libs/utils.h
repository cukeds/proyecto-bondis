//
// Created by Juan on 10/17/2023.
//

#ifndef PROYECTO_BONDI_UTILS_H
#define PROYECTO_BONDI_UTILS_H
#include <cmath>

// Function to convert degrees to radians
constexpr double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Function to calculate the Haversine distance between two sets of coordinates
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // Radius of the Earth in kilometers (mean value)
    constexpr double earthRadiusKm = 6371.0;

    // Convert latitude and longitude from degrees to radians
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);

    // Haversine formula
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    // Calculate the distance
    double distance = earthRadiusKm * c;

    return distance;
}

#endif //PROYECTO_BONDI_UTILS_H

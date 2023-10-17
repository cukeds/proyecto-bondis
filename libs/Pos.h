//
// Created by Juan on 10/17/2023.
//

#ifndef PROYECTO_BONDI_POS_H
#define PROYECTO_BONDI_POS_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utils.h"


struct pos {
    double lat;
    double lon;

    pos() : lat(0), lon(0) {}

    friend std::ostream &operator<<(std::ostream &os, const pos &p) {
        os << "lat: " << p.lat << " lon: " << p.lon;
        return os;
    }

    // read from stream
    friend std::istream &operator>>(std::istream &is, pos &p) {
        std::string line;
        getline(is, line);
        std::stringstream ss(line);
        std::string token;
        getline(ss, token, ',');
        p.lat = stod(token);
        getline(ss, token, ',');
        p.lon = stod(token);
        return is;
    }

    // read from string
    friend void operator>>(const std::string &s, pos &p) {
        std::stringstream ss(s);
        std::string token;
        getline(ss, token, ',');
        p.lat = stod(token);
        getline(ss, token, ',');
        p.lon = stod(token);
    }

    friend bool operator<(const pos &p1, const pos &p2) {
        return p1.lat < p2.lat && p1.lon < p2.lon;
    }

    friend bool operator>(const pos &p1, const pos &p2) {
        return p1.lat > p2.lat && p1.lon > p2.lon;
    }

    friend bool operator==(const pos &p1, const pos &p2) {
        return p1.lat == p2.lat && p1.lon == p2.lon;
    }

    friend bool operator!=(const pos &p1, const pos &p2) {
        return !(p1 == p2);
    }

    friend bool operator<=(const pos &p1, const pos &p2) {
        return p1 < p2 || p1 == p2;
    }

    friend bool operator>=(const pos &p1, const pos &p2) {
        return p1 > p2 || p1 == p2;
    }



};

// Returns the closest position to p in list
pos closest(pos &p, std::vector<pos> &list) {
    pos closest = list[0];
    double minDistance = haversineDistance(p.lat, p.lon, closest.lat, closest.lon);
    for (auto &pos : list) {
        double distance = haversineDistance(p.lat, p.lon, pos.lat, pos.lon);
        if (distance < minDistance) {
            minDistance = distance;
            closest = pos;
        }
    }
    return closest;
}

// normalize the coordinates based on bounding box
void normalize(pos &p, pos &min, pos &max) {
    p.lat = (p.lat - min.lat) / (max.lat - min.lat);
    p.lon = (p.lon - min.lon) / (max.lon - min.lon);
}

void denormalize(pos &p, pos &min, pos &max) {
    p.lat = p.lat * (max.lat - min.lat) + min.lat;
    p.lon = p.lon * (max.lon - min.lon) + min.lon;
}

#endif //PROYECTO_BONDI_POS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "../libs/Grafo.h"
#include <SFML/Graphics.hpp>


using namespace std;
struct person{
    float speed = 2.0f;
    float x = 0;
    float y = 0;
    int current_node = 0;
};

// move person to a point (x, y)
void move(person &p, float x, float y){
    float dx = x - p.x;
    float dy = y - p.y;
    float distance = sqrt(dx * dx + dy * dy);
    if(distance > 0){
        float step = p.speed / distance;
        p.x += dx * step;
        p.y += dy * step;
    }
}

//check if person is close enough to node (5.0f)
bool closeEnough(person &p, float x, float y){
    float dx = x - p.x;
    float dy = y - p.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < 5.0f;
}

// Structure to represent geographic coordinates
struct pos {
    double lat;
    double lon;

    pos() : lat(0), lon(0) {}

    friend ostream &operator<<(ostream &os, const pos &p) {
        os << "lat: " << p.lat << " lon: " << p.lon;
        return os;
    }

    // read from stream
    friend istream &operator>>(istream &is, pos &p) {
        string line;
        getline(is, line);
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        p.lat = stod(token);
        getline(ss, token, ',');
        p.lon = stod(token);
        return is;
    }

    // read from string
    friend void operator>>(const string &s, pos &p) {
        stringstream ss(s);
        string token;
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

// takes a position and a list of positions, returns the closest position
// positions are coordinates in the form of (lat, lon)
// harversine distance
pos closest(pos &p, vector<pos> &list) {
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

// Function to render the graph using SFML
template<class T>
void renderGraph(sf::RenderWindow& window, const sf::Font  &font, Grafo<T> myGraph, vector<sf::Vector2f> &nodePositions) {
    // Render your graph here using SFML drawing functions
    window.clear(sf::Color::White);
    auto nodos = myGraph.getNodos();
    for (size_t i = 0; i < myGraph.getN(); i++) {
        std::shared_ptr<NodoGrafo<T>> nodo = nodos[i];

        // Create a circle for the node
        sf::CircleShape nodeShape(nodo->getSize());
        Color color = nodo->getColor();
        nodeShape.setFillColor(sf::Color(color.r, color.g, color.b));
        nodeShape.setPosition(nodePositions[i].x - nodo->getSize(), nodePositions[i].y - nodo->getSize()); // Adjust for circle radius
        // Draw the node
        window.draw(nodeShape);

        sf::Text label;
        label.setFont(font); // Use the loaded font
//        label.setCharacterSize(20);
//        label.setFillColor(sf::Color::Black);
//        label.setString(to_string(nodo->getId())); // Use node index as label (add 1 to start from 1)

        if(nodo->getLabel() != to_string(nodo->getId())){
            sf::Text label;
            label.setFont(font); // Use the loaded font
            label.setCharacterSize(20);
            label.setFillColor(sf::Color::Black);
            label.setString(nodo->getLabel()); // Use node index as label (add 1 to start from 1)
            sf::FloatRect labelBounds = label.getLocalBounds();
            label.setOrigin(labelBounds.width / 2.0f, labelBounds.height * 2.0f); // Center the origin
            label.setPosition(nodePositions[i]);
            window.draw(label);

        }

        // Position the label at the center of the node
        sf::FloatRect labelBounds = label.getLocalBounds();
        label.setOrigin(labelBounds.width / 2.0f, labelBounds.height / 2.0f); // Center the origin
        label.setPosition(nodePositions[i]);
        window.draw(label);
        // Draw connections (edges)
        for (int j = 0; j < nodo->getGrado(); j++) {
            std::shared_ptr<NodoGrafo<T>> connectedNode = nodo->getConexion(j);

            // Calculate positions of connected nodes based on nodePositions
            sf::Vector2f startPos = nodePositions[i];
            sf::Vector2f endPos = nodePositions[connectedNode->getId()];
            sf::Color lineColor = sf::Color::Black;


            // Create a line for the connection
            sf::Vertex line[] = {
                    sf::Vertex(startPos, lineColor),
                    sf::Vertex(endPos, lineColor)
            };

            // Draw the connection (edge)
            window.draw(line, 2, sf::Lines);
        }
    }
}

int main() {
    Grafo<pos> grafo(1000, Type::Directed);
    grafo.read_nodes_from_csv("../data/nodes1000.csv", true);
    grafo.read_edges_from_csv("../data/edges1000.csv", true);

    // Now we get the bounding box from the data
    auto data = grafo.getNodosData();
    pos min = data[0];
    pos max = data[0];
    for (auto &pos : data) {
        if (pos.lat < min.lat) {
            min.lat = pos.lat;
        }
        if (pos.lon < min.lon) {
            min.lon = pos.lon;
        }
        if (pos.lat > max.lat) {
            max.lat = pos.lat;
        }
        if (pos.lon > max.lon) {
            max.lon = pos.lon;
        }
    }

    // normalize all coordinates
//    pos newpos;
//    for (auto &nodo : grafo.getNodos()) {
//        newpos = nodo->getDato();
//        normalize(newpos, min, max);
//        nodo->setDato(newpos);
//    }
//
//    // print bounding box
//    cout << "min: " << min << endl;
//    cout << "max: " << max << endl;
//    // print separator
//    cout << "-----------------" << endl;
//    // print some normalized coordinates (3)
//    for (int i = 0; i < 3; i++) {
//        cout << grafo.getNodo(i)->getDato() << endl;
//    }

    // print separator
    cout << "-----------------" << endl;

    // Get closest node to a random position
    pos random_position;
    random_position.lat = -34.5899925;
    random_position.lon = -58.5604069;
    cout<<"test: "<<random_position<<endl;
    normalize(random_position, min, max);
    cout << "test normalized: " << random_position << endl;
    data = grafo.getNodosData();
    pos closestpos = closest(random_position, data);
    normalize(closestpos, min, max);
    cout<< "closest: "<<closestpos<<endl;
    denormalize(closestpos, min, max);
    cout<<"closest denormalized: "<<closestpos<<endl;

    // print separator
    cout << "-----------------" << endl;

    // Get shortest path between two positions
    data = grafo.getNodosData();
    // sort data

    pos start;
//    start.lat = -34.5899925;
//    start.lon = -58.5604069;
    start.lat =  -34.509172;
    start.lon = -58.565068;
    pos end;
//    end.lat = -34.571577;
//    end.lon = -58.571440;
    end.lat =  -34.627619;
    end.lon = -58.463187;
    // Before normalizing
    cout << "start: " << start << endl;
    cout << "end: " << end << endl;
    start = closest(start, data);
    end = closest(end, data);
    cout << "start closest: " << start << endl;
    cout << "end closest: " << end << endl;
    cout<< "Calculating shortest path..."<<endl;
    vector<int> path = grafo.bfsShortestPath(grafo.getNodo(start)->getId(), grafo.getNodo(end)->getId());
    auto visited = path;
    //vector<int> visited = grafo.bfs(grafo.getNodo(start)->getId(), grafo.getNodo(end)->getId());
    //vector<int> path = {45236, 11995, 20731, 14155, 47838};
    cout << "path: ";
    for (auto &id : path) {
        cout << id << " ";
    }
    cout << endl;

    // print separator
    cout << "-----------------" << endl;

    // visualize path with SFML
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Graph Visualization");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("../assets/SIXTY.ttf")) {
        cout << "Error loading font" << endl;
        return 1;
    }

    // Find relative bounding box from path
    pos pathMin = grafo.getNodo(path[0])->getDato();
    pos pathMax = grafo.getNodo(path[0])->getDato();
    for (int i : visited) {
        pos p = grafo.getNodo(i)->getDato();
        if (p.lat < pathMin.lat) {
            pathMin.lat = p.lat;
        }
        if (p.lon < pathMin.lon) {
            pathMin.lon = p.lon;
        }
        if (p.lat > pathMax.lat) {
            pathMax.lat = p.lat;
        }
        if (p.lon > pathMax.lon) {
            pathMax.lon = p.lon;
        }
    }

    // Normalize path coordinates for node positions and taking into account circle size
    // remember y is inverted in SFML and normalized coordinates go from 0 to 1
    vector<sf::Vector2f> nodePositions;
    for (int i : visited) {
        pos p = grafo.getNodo(i)->getDato();
        normalize(p, pathMin, pathMax);

        // take into account circle size and window size and invert y
        // make sure no two nodes are too close
        nodePositions.emplace_back(p.lon * (window.getSize().x - 40.0f) + 20.0f,
                                             (1.0f - p.lat) * (window.getSize().y - 40.0f) + 20.0f);

    }




    // Create smaller graph with only the nodes in the path
    Grafo<pos> pathGraph((int) visited.size(), Type::Directed);
    for (int i : visited) {
        pos p = grafo.getNodo(i)->getDato();
        pathGraph.agregarNodo(p);
    }


    // Set node colors and sizes
    for (int i = 0; i < visited.size(); i++) {
        // set label to denormalized path value
        pos p = grafo.getNodo(visited[i])->getDato();
        pathGraph.getNodo(i)->setColor(255, 0, 0);

        // set size
        pathGraph.getNodo(i)->setSize(5.0f);
    }
    // Set node colors and sizes for path nodes
    for (int i = 0; i < path.size(); i++) {

        pos p = grafo.getNodo(path[i])->getDato();
        pathGraph.getNodo(i)->setLabel(to_string(p.lat) + "," + to_string(p.lon));
        // set size
        pathGraph.getNodo(i)->setSize(20.0f);
        // set color based on position
        if (i == 0) {
            pathGraph.getNodo(i)->setColor(0, 255, 0);
        } else if (i == path.size() - 1) {
            pathGraph.getNodo(i)->setColor(0, 0, 255);
        } else {
            pathGraph.getNodo(i)->setColor(255, 255, 0);
        }
    }

    // Create edges for the path graph
    for (int i = 0; i < visited.size() - 1; i++) {
        pathGraph.agregarArista(i, i + 1);
    }

    // create person
    person p;
    p.x = nodePositions[0].x;
    p.y = nodePositions[0].y;
    // create circle
    sf::CircleShape person(10.0f);
    person.setFillColor(sf::Color::Black);
    person.setPosition(p.x, p.y);


    // Render graph
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        renderGraph(window, font, pathGraph, nodePositions);

        // move person
        if(p.current_node < visited.size()){
            move(p, nodePositions[p.current_node].x, nodePositions[p.current_node].y);
            person.setPosition(p.x, p.y);
            if(closeEnough(p, nodePositions[p.current_node].x, nodePositions[p.current_node].y)){
                p.current_node++;
            }
        }

        if(!path.empty()){
            sf::Text label;
            label.setFont(font); // Use the loaded font
            label.setCharacterSize(32);
            label.setFillColor(sf::Color::Black);
            label.setString("Path Length: " + to_string(path.size() )); // Use node index as label (add 1 to start from 1)
            label.setPosition(20, 15);
            window.draw(label);
            label.setFont(font); // Use the loaded font
            label.setCharacterSize(32);
            label.setFillColor(sf::Color::Black);
            label.setString("Visited: " + to_string(visited.size() )); // Use node index as label (add 1 to start from 1)
            label.setPosition(220, 15);
            window.draw(label);
        }

        // draw person
        window.draw(person);


        // Render your graph here using SFML drawing functions

        window.display();
    }

    return EXIT_SUCCESS;

}

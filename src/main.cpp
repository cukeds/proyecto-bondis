#include <iostream>
#include <sstream>
#include <fstream>
#include "../libs/Person.h"
#include "../libs/Pos.h"
#include "../libs/Grafo.h"
#include "../libs/utils.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../libs/GraphVisualizer.h"

using namespace std;


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
            std::shared_ptr<NodoGrafo<T>> connectedNode = nodo->getConexion(j).lock();

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

// normalize vector1 based on vector2's index of values (EX v1 {1, 2, 3} v2 {1, 3, 2} -> v1 {0, 2, 1})
vector<int> normalizeVector(vector<int> &v1, vector<int> &v2){
    vector<int> normalized;
    unordered_map<int, int> map;
    for(int i = 0; i < v2.size(); i++){
        map[v2[i]] = i;
    }
    for(int & i : v1){
        normalized.push_back(map[i]);
    }
    return normalized;
}



int main() {
    Grafo<pos> grafo(54000, Type::Directed);
    grafo.read_nodes_from_csv("../data/nodes.csv", true);
    grafo.read_edges_from_csv("../data/edges.csv", true);

    // Now we get the bounding box from the data
    auto data = grafo.getNodosData();

    cout<<"data size: "<<data.size()<<endl;
    cout<<"Getting bounding box..."<<endl;
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
    cout<<"Bounding box: "<<endl;
    cout<<"min: "<<min<<endl;
    cout<<"max: "<<max<<endl;

    // print separator
    cout << "-----------------" << endl;

    // wait for user keypress
    system("pause");

    // Get closest node to a random position
    pos random_position;
    random_position.lat = -34.5899925;
    random_position.lon = -58.5604069;
    cout<<"random: "<<random_position<<endl;
    pos closestpos = closest(random_position, data);
    cout<<"closest: "<<closestpos<<endl;
    normalize(closestpos, min, max);
    cout<< "closest normalized: "<<closestpos<<endl;
    denormalize(closestpos, min, max);
    cout<<"closest denormalized: "<<closestpos<<endl;

    // print separator
    cout << "-----------------" << endl;
    system("pause");

    // Get shortest path between two positions
    data = grafo.getNodosData();
    // sort data

    pos start;

    start.lat =  -34.509172;
    start.lon = -58.565068;
    pos end;

    end.lat =  -34.627619;
    end.lon = -58.463187;

    // user input for start and end, format "lat,lon"
//    cout << "Enter start position (lat,lon): ";
//    cin >> start.lat;
//    cin.ignore();
//    cin >> start.lon;
//    cin.ignore();
//    cout << "Enter end position (lat,lon): ";
//    cin >> end.lat;
//    cin.ignore();
//    cin >> end.lon;
//    cin.ignore();


    // Before normalizing
    cout << "start: " << start << endl;
    cout << "end: " << end << endl;
    start = closest(start, data);
    end = closest(end, data);
    cout << "start closest: " << start << endl;
    cout << "end closest: " << end << endl;
    cout<< "Calculating shortest path..."<<endl;
    vector<int> path = grafo.bfsShortestPath(grafo.getNodo(start)->getId(), grafo.getNodo(end)->getId());
    //vector<int> visited = grafo.bfs(grafo.getNodo(start)->getId(), grafo.getNodo(end)->getId());
    vector<int> visited = path;
    cout << "path: ";
    for (auto &id : path) {
        cout << id << " ";
    }
    cout << endl;

    // print separator
    cout << "-----------------" << endl;
    system("pause");


    // visualize path with SFML
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Graph Visualization");
    window.setFramerateLimit(320);

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
    // copy visited into new vector
    vector<int> normalization = visited;

    auto normalized_visited = normalizeVector(visited, normalization);
    auto normalized_path = normalizeVector(path, normalization);


    // Set node colors and sizes
    for (int i = 0; i < normalized_visited.size(); i++) {
        pathGraph.getNodo(i)->setColor(255, 0, 0);

        // set size
        pathGraph.getNodo(i)->setSize(5.0f);
    }
    // set node labels for path nodes
    for (int i = 0; i < path.size(); i++) {
        pos p = grafo.getNodo(i)->getDato();
        pathGraph.getNodo(normalized_path[i])->setLabel(to_string(p.lat) + "," + to_string(p.lon));
    }


    // Set node colors and sizes for path nodes
    for (int i : normalized_path) {

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
    Person p(nodePositions[0].x, nodePositions[0].y);
    // create circle
    sf::CircleShape person(10.0f);
    person.setFillColor(sf::Color::Black);
    person.setPosition(p.getX(), p.getY());


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
        if(p.getCurrentNode() < visited.size()){
            p.movePersonTo(nodePositions[p.getCurrentNode()].x, nodePositions[p.getCurrentNode()].y);
            person.setPosition(p.getX(), p.getY());
            if(p.closeEnough(nodePositions[p.getCurrentNode()].x, nodePositions[p.getCurrentNode()].y)){
                p.advanceNode();
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

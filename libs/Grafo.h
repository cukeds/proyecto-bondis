#ifndef GRAFOS_H
#define GRAFOS_H

#include <queue>
#include <stack>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "NodoGrafo.h"

enum class Type{
    Directed,
    Undirected
};

template <class T>
class Grafo {
private:
    int n = 0;
    std::vector<std::shared_ptr<NodoGrafo<T>>> nodos;
    std::vector<std::vector<bool>> adjacencyMatrix;
    Type type;

    void agregarAristaUndirected(int id1, int id2) {
        if (id1 >= 0 && id1 < n && id2 >= 0 && id2 < n) {
            nodos[id1]->addConexion(nodos[id2]);
            nodos[id2]->addConexion(nodos[id1]);

            adjacencyMatrix[id1][id2] = true;
            adjacencyMatrix[id2][id1] = true;
        } else {
            std::cerr<<"Node not in graph"<<std::endl;
        }
    }

    void agregarAristaDirected(int id1, int id2) {
        if (id1 >= 0 && id1 < n && id2 >= 0 && id2 < n) {
            nodos[id1]->addConexion(nodos[id2]);
            adjacencyMatrix[id1][id2] = true;
        } else {
            std::cerr<<"Node not in graph"<<std::endl;
        }
    }

public:
    Grafo() : adjacencyMatrix(1000, std::vector<bool>(0, false)), nodos(1000){
        type = Type::Undirected;
    }

    Grafo(int expected_n) : adjacencyMatrix(expected_n, std::vector<bool>(expected_n, false)), nodos(expected_n){
        type = Type::Undirected;
    }

    Grafo(Type type) : adjacencyMatrix(1000, std::vector<bool>(1000, false)), nodos(1000){

        this->type = type;
    }

    Grafo(int expected_n, Type type) : adjacencyMatrix(expected_n, std::vector<bool>(expected_n, false)), nodos(expected_n){
        this->type = type;
    }

    const std::vector<std::shared_ptr<NodoGrafo<T>>> &getNodos() const {
        return nodos;
    }

    std::vector<T> getNodosData() const {
        std::vector<T> data;
        for (const auto& nodo : nodos) {
            data.push_back(nodo->getDato());
        }
        return data;
    }

    int getN() {
        return n;
    }

    void agregarNodo(T dato) {
        auto nuevoNodo = std::make_shared<NodoGrafo<T>>(n, dato);

        // Initialize the new row for the adjacency matrix with 'false' values
        if(n + 1 > nodos.size()) {
            std::vector<bool> newRow(n, false);
            adjacencyMatrix.emplace_back(newRow);
            // Update existing rows to include a 'false' value for the new node
            adjacencyMatrix.back().assign(n, false);

            nodos.push_back(nuevoNodo);
            n++;
            return;
        }
        adjacencyMatrix[n].assign(n, false);
        nodos[n] = nuevoNodo;
        n++;
    }

    void agregarArista(int id1, int id2) {
        type == Type::Undirected ? agregarAristaUndirected(id1, id2) : agregarAristaDirected(id1, id2);
    }


    void imprimirGrafo() {
        for (const auto& nodo : nodos) {
            std::cout << "Nodo " << nodo->getId() << " Grado: " << nodo->getGrado() << " - " << nodo->getDato() << " está conectado a: ";

            if (nodo->getGrado() > 0) {
                for (int i = 0; i < n; i++) {
                    if (adjacencyMatrix[nodo->getId()][i]) {
                        std::cout << nodos[i]->getDato() << ", ";
                    }
                }
                std::cout << '\b' << '\b'; // Remove the last comma and space
            } else {

                std::cout << "ningún otro nodo";
            }

            std::cout << '\n';
        }
    }

    void matrizAdjacencia() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjacencyMatrix[i][j]) {
                    std::cout << "1 ";
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << '\n';
        }
    }

    std::shared_ptr<NodoGrafo<T>> getNodo(int pos){
        return nodos[pos];
    }
    std::vector<int> bfsShortestPath(int id1, int id2){
        std::queue<int> frontier;

        std::vector<int> shortestPath;

        if (id1 < 0 || id1 >= n || id2 < 0 || id2 >= n) {
            std::cerr << "Invalid node indices." << std::endl;
            return shortestPath;
        }

        std::vector<int> distance(n, -1);
        std::vector<int> parent(n, -1);

        distance[id1] = 0;
        frontier.push(id1);

        while (!frontier.empty()) {
            int currentNode = frontier.front();
            frontier.pop();

            if (currentNode == id2) {
                // Reconstruct the shortest path
                int node = currentNode;
                while (node != -1) {
                    shortestPath.push_back(node);
                    node = parent[node];
                }
                std::reverse(shortestPath.begin(), shortestPath.end());
                return shortestPath;
            }

            for (int neighbor = 0; neighbor < n; neighbor++) {
                if (adjacencyMatrix[currentNode][neighbor] && distance[neighbor] == -1) {
                    distance[neighbor] = distance[currentNode] + 1;
                    parent[neighbor] = currentNode;
                    frontier.push(neighbor);
                }
            }
        }

        return shortestPath; // Empty path; no connection found
    }


    // bfs from node 1 to node 2 that returns all visited nodes in the order they were visited
    std::vector<int> bfs(int id1, int id2){
        std::queue<int> frontier;

        std::vector<int> visited;

        if (id1 < 0 || id1 >= n || id2 < 0 || id2 >= n) {
            std::cerr << "Invalid node indices." << std::endl;
            return visited;
        }

        std::vector<int> distance(n, -1);

        distance[id1] = 0;
        frontier.push(id1);

        while (!frontier.empty()) {
            int currentNode = frontier.front();
            frontier.pop();

            if (currentNode == id2) {
                visited.push_back(currentNode);
                return visited;
            }

            for (int neighbor = 0; neighbor < n; neighbor++) {
                if (adjacencyMatrix[currentNode][neighbor] && distance[neighbor] == -1) {
                    visited.push_back(neighbor);
                    distance[neighbor] = distance[currentNode] + 1;
                    frontier.push(neighbor);
                }
            }
        }

        return visited;
    }



    std::vector<int> dfsShortestPath(int id1, int id2){
        std::stack<int> frontier;

        std::vector<int> path;

        if (id1 < 0 || id1 >= n || id2 < 0 || id2 >= n) {
            std::cerr << "Invalid node indices." << std::endl;
            return path;
        }

        std::vector<int> distance(n, -1);
        std::vector<int> parent(n, -1);

        distance[id1] = 0;
        frontier.push(id1);

        while (!frontier.empty()) {
            int currentNode = frontier.top();
            frontier.pop();

            if (currentNode == id2) {
                // Reconstruct the shortest path
                int node = currentNode;
                while (node != -1) {
                    path.push_back(node);
                    node = parent[node];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (int neighbor = 0; neighbor < n; neighbor++) {
                if (adjacencyMatrix[currentNode][neighbor] && distance[neighbor] == -1) {
                    distance[neighbor] = distance[currentNode] + 1;
                    parent[neighbor] = currentNode;
                    frontier.push(neighbor);
                }
            }
        }

        return path; // Empty path; no connection found
    }

    // dfs that returns all visited nodes
    std::vector<int> dfs(int id1, int id2){
        std::stack<int> frontier;

        std::vector<int> visited;

        if (id1 < 0 || id1 >= n || id2 < 0 || id2 >= n) {
            std::cerr << "Invalid node indices." << std::endl;
            return visited;
        }

        std::vector<int> distance(n, -1);

        distance[id1] = 0;
        frontier.push(id1);

        while (!frontier.empty()) {
            int currentNode = frontier.top();
            frontier.pop();

            if (currentNode == id2) {
                return visited;
            }

            for (int neighbor = 0; neighbor < n; neighbor++) {
                if (adjacencyMatrix[currentNode][neighbor] && distance[neighbor] == -1) {
                    distance[neighbor] = distance[currentNode] + 1;
                    visited.push_back(neighbor);
                    frontier.push(neighbor);
                }
            }
        }

        return visited;
    }

    void read_nodes_from_csv(const std::string& filename, bool header = false ){
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the CSV file." << std::endl;
            return;
        }

        std::string line;

        std::cout<<"Reading from file...\n";
        if(header) std::getline(file, line); // Skip the header line
        while (std::getline(file, line)) {
            // format is id, "lat, lon"
            std::istringstream iss(line);
            int id;
            std::string lat_lon;
            T dato;
            // Read data from the CSV line (assuming format: id, "lat, lon")

            char delimiter; // To store the comma
            if (iss >> id >> std::ws >> delimiter && delimiter == ',' && std::getline(iss, lat_lon)) {
                // Remove leading and trailing spaces from lat_lon
                lat_lon = lat_lon.substr(lat_lon.find_first_not_of(' '));
                lat_lon = lat_lon.substr(0, lat_lon.find_last_not_of(' ') + 1);

                // Now you have 'id' and 'lat, lon', and you can add the node

                if (lat_lon.front() == '"' && lat_lon.back() == '"') {
                    lat_lon = lat_lon.substr(1, lat_lon.size() - 2);
                }
                lat_lon >> dato;
                agregarNodo(dato);
            } else {
                std::cerr << "Failed to parse line: " << line << '\n';
            }
        }


        file.close();
    }


    void read_edges_from_csv(const std::string& filename, bool header = false) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the CSV file." << std::endl;
            return;
        }

        std::string line;

        std::cout<<"Reading from file...\n";

        if(header) std::getline(file, line); // Skip the header line
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id1, id2;
            char delimiter;

            // Read data from the CSV line (assuming format: id1, id2)
            if (iss >> id1 >> delimiter >> id2 && iss.eof()) {
                // Add the edge
                agregarArista(id1, id2);
            } else {
                std::cerr << "Failed to parse line: " << line << '\n';
            }
        }

        file.close();
    }

    std::shared_ptr<NodoGrafo<T>> getMax(){
        std::shared_ptr<NodoGrafo<T>> max = nodos[0];
        for (int i = 1; i < n; i++) {
            if (nodos[i]->getDato() > max->getDato()) {
                max = nodos[i];
            }
        }
        return max;
    }

    std::shared_ptr<NodoGrafo<T>> getMin(){
        std::shared_ptr<NodoGrafo<T>> min = nodos[0];
        for (int i = 1; i < n; i++) {
            if (nodos[i]->getDato() < min->getDato()) {
                min = nodos[i];
            }
        }
        return min;
    }

    std::shared_ptr<NodoGrafo<T>> getNodo(T dato){
        for (int i = 0; i < n; i++) {
            if (nodos[i]->getDato() == dato) {
                return nodos[i];
            }
        }
        return nullptr;
    }

};

#endif

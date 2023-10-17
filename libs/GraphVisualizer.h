//
// Created by Juan on 10/17/2023.
//

#ifndef PROYECTO_BONDI_GRAPHVISUALIZER_H
#define PROYECTO_BONDI_GRAPHVISUALIZER_H


#include <SFML/Graphics.hpp>
#include <vector>

// Define a class to encapsulate graph visualization
class GraphVisualizer {
public:
    GraphVisualizer(sf::RenderWindow& _window, const sf::Font& _font, std::vector<sf::Vector2f>& _nodePositions);
    void renderGraph();
    void updatePersonPosition();

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    std::vector<sf::Vector2f>& nodePositions;
    sf::CircleShape person;
    Person p;

    // Constants
    const float kCloseEnoughDistance = 5.0f;

    // Private helper methods
    float calculateDistance(float x1, float y1, float x2, float y2);
    void movePersonTo(float x, float y);
};


#endif //PROYECTO_BONDI_GRAPHVISUALIZER_H

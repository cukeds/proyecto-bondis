//
// Created by Juan on 10/17/2023.
//

#include "GraphVisualizer.h"
GraphVisualizer::GraphVisualizer(sf::RenderWindow& window, const sf::Font& font)
        : window(window), font(font) {
    person.setRadius(10.0f);
    person.setFillColor(sf::Color::Black);
    // Other initialization if needed
}

void GraphVisualizer::renderGraph() {
    window.clear(sf::Color::White);

    // Render the graph nodes, edges, and other elements using SFML drawing functions
    // Add your rendering code here

    window.draw(person);

    window.display();
}

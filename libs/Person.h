//
// Created by Juan on 10/17/2023.
//

#ifndef PROYECTO_BONDI_PERSON_H
#define PROYECTO_BONDI_PERSON_H
#include <cmath>

// Person class
class Person {
private:
    float speed = 2.0f; // Pixels per frame
    float x = 0;
    float y = 0;
    int current_node = 0; // Current node
    const float kCloseEnoughDistance= 5.0f; // Distance to consider the person has arrived to a node
public:
    Person() = default; // Default constructor
    Person(float x, float y){ // Constructor with initial position
        this->x = x;
        this->y = y;
    }
    // Getters and setters
    [[nodiscard]] float getX() const{
        return this->x;
    }
    [[nodiscard]] float getY() const{
        return this->y;
    }
    void setX(float x){
        this->x = x;
    }
    void setY(float y){
        this->y = y;
    }
    [[nodiscard]] int getCurrentNode() const{
        return this->current_node;
    }

    void setCurrentNode(int node){
        this->current_node = node;
    }

    void advanceNode(){
        this->current_node++;
    }

    // Methods
    // Move the person to a given position
    void movePersonTo(float x, float y){
        float dx = x - this->x;
        float dy = y - this->y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if(distance > 0){
            float step = this->speed / distance;
            this->x += dx * step;
            this->y += dy * step;
        }
    }

    // Check if the person is close enough to a given position
    [[nodiscard]] bool closeEnough(float x, float y) const{
        float dx = x - this->x;
        float dy = y - this->y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance < kCloseEnoughDistance;
    }

};

#endif //PROYECTO_BONDI_PERSON_H

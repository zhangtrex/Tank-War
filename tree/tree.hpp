// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>

#include "grammar.hpp"
 
class Turtle {
public:
    glm::mat4 trans;
    float length;
    float scale;
};
 
class Trunk {
public:
    glm::mat4 trans;
};
 
class LSystem  {
private:
    float dx,dy,dz; // rotate value around x,y,z axis
 
    float length;
    float length_factor;
    float scale;
    float scale_factor;
 
    Turtle turtle;
 
    Grammar grammar;
 
public:
 
    std::vector<Trunk> trunks;

    LSystem();
 
    void clearAll();
    void initGrammar(); // initialize default values
    void generateTree(glm::mat4 root_trans); // generate tree trunks
    std::vector<Trunk> getTrunks();
};

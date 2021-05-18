// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <string>
#include <utility>
#include <vector>


class Grammar
{
private:
    std::string name;  

    int level;
    std::string start; 
    std::string result;

    std::vector<std::pair<char,std::vector<std::string>>> generations;
public:

    Grammar(const std::string & name, const std::string & start);

    void clear(); // clear result

    int find(char c);
    std::string search(const char& c);

    void addRule(const char& c,  const std::string & ref); // add a new rule
    void iterateFor(int num); // create a generation, generated for num iterations

    std::string getGrammarName();
    int getLevel();
    std::string getResult();

};
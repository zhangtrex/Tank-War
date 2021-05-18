// Term--Fall 2020
// Chenyang Zhang

#include <string>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include "grammar.hpp"

using namespace std;

Grammar::Grammar(const string & name, const string & start):
	name(name), start(start)
{}

void Grammar::clear() {
    generations.clear();
}
 
int Grammar::find(char c) {
    for(int i=0; i< generations.size(); i++)
    {
        if( generations[i].first == c) return i;
    }
    return -1;
}
 
 
string Grammar::search(const char& c) {


    int id = find(c);
    
    if(id == -1) {
    	string res;
    	res.push_back(c);
    	return res;
    }
 
    int num = generations[id].second.size();
 
    int index = rand() % num;

 
    return generations[id].second[index];
}
 
 
void Grammar::addRule(const char& c,  const string& ref) {
    int id = find(c);
 
    if(id == -1)
    {
        pair<char, vector<string>> temp;
        temp.first = c;
        temp.second.push_back(ref);
        generations.push_back(temp);
        return;
    }
    generations[id].second.push_back(ref);
}
 
void Grammar::iterateFor(int num) {
    
    level = num;
 
    result = start;
    for(int i=0; i<num ;i++)
    {
        string tmp = "";
        for(int i=0; i<result.size();i++)
        {
            tmp+=search(result[i]);
        }
        result = tmp;
    }
    // cout << result << endl;
}

string Grammar::getGrammarName()
{
    return name;
}
 
int Grammar::getLevel()
{
    return level;
}
 
string Grammar::getResult()
{
    return result;
}


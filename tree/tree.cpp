// Term--Fall 2020
// Chenyang Zhang

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tree.hpp"

using namespace std;
using namespace glm;

LSystem::LSystem():
	grammar("new", "F")
{
	
}
 
void LSystem::clearAll()
{
    grammar.clear();
    trunks.clear();
}
 
void LSystem::initGrammar()
{   
	dx = dy = dz = 0.5f;
	length = 1;
	scale = 1;
	length_factor = 0.8;
	scale_factor = 0.8;
    // rotate x ^&
    // rotate y $%
    // rotate z */
    

	grammar.addRule('F',"F[*^F][*&F][/$F]");

	grammar.addRule('F',"F[*F][&F]");
	// grammar.addRule('F',"F[/F][$F[&F]]");
	// grammar.addRule('F',"F*[^F^F]%[*F]");
	// grammar.addRule('F', "F[%F][^F][/F]");
	grammar.addRule('F', "F[$F]");
	grammar.iterateFor(3);

}
 
void LSystem::generateTree(mat4 root_trans)
{ 
    trunks.clear();

    turtle.trans = root_trans;
    turtle.length = length;
    turtle.scale = scale;
    vector<Turtle> stacks;
    stacks.push_back(turtle);

    vec3 scale_vec = vec3(0.4f,3.0f,0.4f);
    vec3 length_vec = vec3(0.0f,1.6f,0.0f);

    Trunk first_trunk;

    for(int i=0; i<grammar.getResult().size();i++)
    {
        char c = grammar.getResult()[i];
 
        Trunk new_trunk;
 		vec3 new_scale;
        switch(c) {
	        case 'F':
	            
	            
	            
	            new_trunk.trans = turtle.trans;
	            new_trunk.trans = glm::scale(new_trunk.trans, scale_vec * turtle.scale);
	            new_trunk.trans = translate(new_trunk.trans, length_vec * 0.5f * turtle.length);
	            turtle.trans = translate(turtle.trans, length_vec * 2.0f * turtle.scale);
	 
	            trunks.push_back(new_trunk);
	            break;
	        case '$':
	            turtle.trans = rotate(turtle.trans, dy, vec3(0,1,0));
	            break;
	        case '%':
	            turtle.trans = rotate(turtle.trans, -dy, vec3(0,1,0));
	            break;
	        case '^': {
	        	turtle.trans = rotate(turtle.trans, dx, vec3(1,0,0));
	            break;
	        }
	        case '&':
	            turtle.trans = rotate(turtle.trans, -dx, vec3(1,0,0));
	            break;
	        case '*':
	            turtle.trans = rotate(turtle.trans, dz, vec3(0,0,1));
	            break;
	        case '/':
	            turtle.trans = rotate(turtle.trans, -dz, vec3(0,0,1));
	            break;
	        case '[':
	            stacks.push_back(turtle);
	            turtle.scale *= scale_factor;
	            turtle.length *= length_factor;
	            break;
	        case ']':
	            turtle = stacks.at(stacks.size()-1);
	            stacks.pop_back();
	            break;
	        default:
	            break;
        }
    }
}

vector<Trunk> LSystem::getTrunks() {
	return trunks;
}




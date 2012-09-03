#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>
#include "colour.h"

namespace regina{

colour::colour() {
	r=0;
	g=0;
	b=0;
}

colour::colour(float setR, float setG, float setB) {
	changeColour(setR,setG,setB);
}

float colour::getR() {return r;}
float colour::getB() {return b;}
float colour::getG() {return g;}

void colour::printForPS(std::ostream& out)
{
	out << r << " " << g << " " << b << '\n';
}

void colour::changeColour(float setR, float setG, float setB) {
	r = modOne(setR);
	g = modOne(setG);
	b = modOne(setB);
}

float colour::modOne(float num)
{
	if(num < 0)
		num = std::abs(num);
	if(num > 1)
		num = num-floor(num);
	return num;
}

}//namespace regina

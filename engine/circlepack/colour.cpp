#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>
#include "colour.h"

namespace regina{

CPcolour::CPcolour() {
	r=0;
	g=0;
	b=0;
}

CPcolour::CPcolour(float setR, float setG, float setB) {
	changeColour(setR,setG,setB);
}

float CPcolour::getR() {return r;}
float CPcolour::getB() {return b;}
float CPcolour::getG() {return g;}

void CPcolour::printForPS(std::ostream& out)
{
	out << r << " " << g << " " << b << '\n';
}

void CPcolour::changeColour(float setR, float setG, float setB) {
	r = modOne(setR);
	g = modOne(setG);
	b = modOne(setB);
}

float CPcolour::modOne(float num)
{
	if(num < 0)
		num = std::abs(num);
	if(num > 1)
		num = num-floor(num);
	return num;
}

}//namespace regina

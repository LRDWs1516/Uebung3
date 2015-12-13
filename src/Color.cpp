/*
 * Color.cpp
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#include "Color.h"

Color::Color(int a, int b, int c){
	value[0] = a;
    value[1] = b;
	value[2] = c;
}

int Color::absolute(){
	return (int)(value[0]+value[1]+value[2]);
}



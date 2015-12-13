/*
 * Color.h
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#ifndef COLOR_H_
#define COLOR_H_

class Color{
public:
	unsigned char value[3];
	Color(int, int, int);
	int absolute();
};

#endif /* COLOR_H_ */

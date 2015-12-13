/*
 * Points.h
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#ifndef POINTS_H_
#define POINTS_H_

#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

using namespace std;

class Point2D{
public:
	double x;
	double y;
	Point2D();
	Point2D(double, double);
	double getDistance(Point2D);
	void printVal();
	double getAngleTo(Point2D, double);
};

#endif /* POINTS_H_ */

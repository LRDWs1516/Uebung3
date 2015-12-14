/*
 * PointArray.h
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#ifndef POINTARRAY_H_
#define POINTARRAY_H_

#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include "Image.h"
#include "Points.h"

//Class used to flood edges and determine connected Points in an image
class PointArray{
public:
	void fill(Image &, int);
	void fill2(Image &, int);
	void getCentrals();
	vector<Point2D> getAllPoints();
	vector<Point2D> allPoints;
	vector<Point2D> startPoints;
	vector<Point2D> centralPoints;
	vector<vector<Point2D> > allObjects;
	vector<vector<bool> > overlay;
	Point2D getCenterof(int);
	int findClosest(Point2D, double);
	vector<Point2D> getCentralTriangle(Image &);
private:
	int vSize;
	vector<Point2D> checkSurrounding(Point2D &, Image &);
	vector<Point2D> followOutline(Point2D &, Image &);
	vector<Point2D> calcOverlay(Point2D &, Image &, int);
	Point2D getCenter(vector<Point2D>);
	bool isOutline(Point2D &, Image &);
};

#endif /* POINTARRAY_H_ */

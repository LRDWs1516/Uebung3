/*
 * Image.h
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include "Color.h"
#include "Points.h"

using namespace std;

class Image{
public:
	Image();
	vector<vector<Color> > Matrix;
	vector<vector<Color> > temporaryMat;
	vector<Point2D> fitArray;
	Point2D centerPoint;
	uint32_t w,h,start,filesize;
	char * overhead;
	bool readImageFromFile(const char *, int);
	bool readImageFromFile2(const char *, int);
	bool writeImageToFile(const char *);
	void thresholdImage(uint8_t thres);
	Point2D calcCenterOfMass();
	void getPointArray();
	void circleFit(vector<Point2D>);
	void detectEdges(int, int);
	void drawRectangle(Point2D, int, Color, int);
	void drawCross(Point2D, int, Color, int);
	void addImage(Image);
	void getCameraParams();
	void drawArrayToImage(vector<Point2D> &, Color);
	void drawBinarysToImage(vector<vector<bool> > &, Color);
	void copyTo(Image &, int);
	int getW(ifstream & , int);
	int getH(ifstream & , int);
	int getStart(ifstream & );
	int getFilesize(ifstream &);
private:
};

#endif /* IMAGE_H_ */

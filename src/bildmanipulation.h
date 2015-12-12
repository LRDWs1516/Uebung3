#ifndef _BILDMANIPULATION_HEADER_
#define _BILDMANIPULATION_HEADER_

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

int getW(ifstream & , int);
int getH(ifstream & , int);
int getStart(ifstream & );
int getFilesize(ifstream &);
void sleepcp(int);

class Point2D{
public:
	double x;
	double y;
	Point2D();
	Point2D(double, double);
};

class Color{
public:
	unsigned char value[3];
	Color(int, int, int);
	int absolute();
};

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
private:
};

class PointArray{
public:
	void fill(Image &, int);
	void fill2(Image &, int);
	bool getCentrals(int);
	vector<Point2D> getAllPoints();
	vector<Point2D> allPoints;
	vector<Point2D> startPoints;
	vector<Point2D> centralPoints;
	vector<vector<Point2D> > allObjects;
	vector<vector<bool> > overlay;
private:
	int vSize;
	vector<Point2D> checkSurrounding(Point2D &, Image &);
	vector<Point2D> followOutline(Point2D &, Image &);
	vector<Point2D> calcOverlay(Point2D &, Image &, int);
	Point2D getCenter(vector<Point2D>);
	bool isOutline(Point2D &, Image &);
};

double getf(double, double, double);
double getfov(double, double, Image);

#endif

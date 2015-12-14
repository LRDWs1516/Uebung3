#ifndef _VECTORCATALOG_HEADER_
#define _VECTORCATALOG_HEADER_

#include <vector>
#include <iostream>
#include <math.h>
#include "catalog.h"
#include "PointArray.h"
#include "vectorCatalog.h"

struct Triangle{
	Point2D A;
	Point2D B;
	Point2D C;
	double alphas[3];
	Triangle(vector<Point2D>);
	void setCorners(vector<Point2D>);
	void calculateAlphas(double);
	};

struct RID3{
	int rID[3] = {-1,-1,-1};
	RID3(int[3]);
	RID3(int, int, int);
};

struct Vector3D{
	double c[3] = {1,1,1};
	Vector3D();
	Vector3D(double[3]);
	Vector3D(double, double, double);
	double abs();
	double scalarMult(Vector3D);
	double angleTo(Vector3D);
};

class StarEntry{
public:
	StarEntry(int, double, double, double, double, double, double);
	StarEntry();
	int getID();
	double getE(int);
	double distanceTo(StarEntry);
private:
	int id;
	double x;
	double y;
	double z;
	double mag;
	double rectascension;
	double declination;
};

class TriangleEntry{
public:
	TriangleEntry(RID3, Vector3D);
	double getalpha(int);
	int getID(int);
private:
	int rID[3];
	double alpha[3];
};

class StarCatalog{
public:
	void translateCatalog(Catalog c);
	vector<StarEntry> getMainCatalog();
	int size();
	RID3 findClosest2(int);
	Vector3D triangleGetAlpha(RID3);
private:
	vector<StarEntry> mainCatalog;
};

class TriangleCatalog{
public:
	void createCatalog(StarCatalog &, double);
	bool containsTriangle(Triangle, double, Vector3D *, RID3 *);
private:
	bool compareTriangle(Triangle, TriangleEntry, double, Vector3D *);
	vector<TriangleEntry> mainCatalog;
};

#endif


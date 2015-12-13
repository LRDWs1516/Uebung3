#ifndef _VECTORCATALOG_HEADER_
#define _VECTORCATALOG_HEADER_

#include <vector>
#include <iostream>
#include "catalog.h"

class StarEntry{
public:
	StarEntry(int, double, double, double, double, double, double);
	int getID();
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
	TriangleEntry(int, int, int, double[3], double[3]);
private:
	int id1;
	int id2;
	int id3;
	double alpha[3];
	double beta[3];
};

class StarCatalog{
public:
	void translateCatalog(Catalog c);
private:
	vector<StarEntry> mainCatalog;
};

class TriangleCatalog{
public:
	void createCatalog(StarCatalog c);
private:
	vector<TriangleEntry> mainCatalog;
};

#endif


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

class StarCatalog{
public:
	void translateCatalog(Catalog c);
private:
	vector<StarEntry> mainCatalog;
};

class TriangleCatalog{
public:
	
private:
	
};

#endif


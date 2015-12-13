#ifndef _CATALOG_HEADER_
#define _CATALOG_HEADER_

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

int getNumberOfLines(FILE);
int getNumberofLines(ifstream &);

class StarCatalogEntry{
public:
		StarCatalogEntry *before;
		StarCatalogEntry  *next;
		int id;
		double x;
		double y;
		double z;
		double mag;
		double rectascension;
		double declination;

		StarCatalogEntry();
		StarCatalogEntry(StarCatalogEntry*, int, double, double, double, double, double, double);
		void setNext(StarCatalogEntry*);
		StarCatalogEntry* getNext();
};

class TriangleCatalogEntry{
public:
	TriangleCatalogEntry * before;
	TriangleCatalogEntry * next;
	StarCatalogEntry * stars[3];		//speichert Zeiger auf die drei Sterne im Katalog, die das Dreieck bilden
	int id1;
	int id2;
	int id3;
	double beta;
	double alpha1;
	double alpha2;

	TriangleCatalogEntry(int , int , int , double , double , double );
	TriangleCatalogEntry();

	void setStars(StarCatalogEntry* stars[3]);
	StarCatalogEntry **getStars(void);

	void setBefore(TriangleCatalogEntry*);
	void setNext(TriangleCatalogEntry*);
	TriangleCatalogEntry* getBefore();
	TriangleCatalogEntry* getNext();
};

class Catalog{
public:

	TriangleCatalogEntry* getHeadTrilog();
	int cSize;
	StarCatalogEntry *head_starlog = new StarCatalogEntry();
	StarCatalogEntry *current;
	TriangleCatalogEntry *head_trilog=new TriangleCatalogEntry();
	void makeCatalog(const char * fname);
	void setTriangleCatalog();
	TriangleCatalogEntry searchTriangle(StarCatalogEntry* );
	StarCatalogEntry& operator++(int);
	StarCatalogEntry& operator--(int);
	int size();
	void reset();
};

#endif

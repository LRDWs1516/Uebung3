/*
 * Dreiecksberechnung.h
 *
 *  Created on: 09.12.2015
 *      Author: david
 */

#ifndef CATALOGMANIPULATION_H_
#define CATALOGMANIPULATION_H_

#include "catalog.h"

bool comparator(double, double);
bool approx(double, double);
class Finder{
public:
	Finder(TriangleCatalogEntry *);
	TriangleCatalogEntry* getMatch(double,double,double);
private:
	TriangleCatalogEntry *head_trilog;
	TriangleCatalogEntry match;
};

#endif /* DREIECKSBERECHNUNG_H_ */

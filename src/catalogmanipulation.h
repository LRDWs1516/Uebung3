/*
 * Dreiecksberechnung.h
 *
 *  Created on: 09.12.2015
 *      Author: david
 */

#ifndef CATALOGMANIPULATION_H_
#define CATALOGMANIPULATION_H_
#include "bildmanipulation.h"
#include "catalog.h"

class Finder{
public:
	Finder();
	TriangleCatalogEntry getMatch();
private:
	TriangleCatalogEntry *head_trilog=new TriangleCatalogEntry();
	TriangleCatalogEntry match;
};

#endif /* DREIECKSBERECHNUNG_H_ */

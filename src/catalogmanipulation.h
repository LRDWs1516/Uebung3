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
	StarCatalogEntry getResult();
private:

	StarCatalogEntry result;
};

#endif /* DREIECKSBERECHNUNG_H_ */

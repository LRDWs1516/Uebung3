/*
 * Dreiecksberechnung.cpp
 *
 *  Created on: 09.12.2015
 *      Author: david
 */
#include "catalogmanipulation.h"

bool TriangleCatalogEntry:: comparator (double i,double j) { 
	return (i<j); 
}

TriangleCatalogEntry Finder::Finder(TriangleCatalogEntry *head_trilog){

	this->head_trilog=head_trilog;
}



 TriangleCatalogEntry Finder::getMatch(double deg1,double deg2, double deg3){

	vector<double> deg_vector=(deg1, deg2, deg3);
	deg_vector[0]=deg1;	
	deg_vector[1]=deg2;
	deg_vector[2]=deg3;

	TriangleCatalogEntry tri=head_trilog->next;
	
	for(tri!=NULL){
		//Initialisiert die Iteratoren
		vector<double> tri_vector=(3);
		tri_vector[0]=tri->alpha1;
		tri_vector[0]=tri->alpha2;
		tri_vector[0]=tri->beta;
	


		//Sortiert Vektoren
		sort(tri_vector.begin(), tri_vector.end(), comparator);
		sort(deg_vector.begin(), deg_vector.end(), comparator);
	
		if(deg_vector==tri_vector){
			return tri;
		}
		tri=tri->next;
	}
	return NULL;

}








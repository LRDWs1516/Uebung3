/*
 * Dreiecksberechnung.cpp
 *
 *  Created on: 09.12.2015
 *      Author: david
 */
#include "catalogmanipulation.h"
#include <vector>
#include <iostream>
#include <algorithm>


bool comparator (double i,double j) {
	return (i<j); 
}

Finder::Finder(TriangleCatalogEntry *head_trilog){

	this->head_trilog=head_trilog;
}



 TriangleCatalogEntry* Finder::getMatch(double deg1,double deg2, double deg3){

	vector<double> deg_vector(3);
	deg_vector[0]=fabs(deg1);	
	deg_vector[1]=fabs(deg2);
	deg_vector[2]=fabs(deg3);

	TriangleCatalogEntry * tri=this->head_trilog->next;
	
	while(tri!=NULL){
		//Initialisiert die Iteratoren
		vector<double> tri_vector(3);
		tri_vector[0]=fabs(tri->alpha1);
		tri_vector[1]=fabs(tri->alpha2);
		tri_vector[2]=fabs(tri->beta);
		
	
		//Sortiert Vektoren
		sort(tri_vector.begin(), tri_vector.end(), comparator);
		sort(deg_vector.begin(), deg_vector.end(), comparator);

		if(approx(deg_vector[0], tri_vector[0]) && approx(deg_vector[1], tri_vector[1]) && approx(deg_vector[2], tri_vector[2])){			

			//~ cout<<"Match found!"<<endl;
			//~ cout<<"Match angles: alpa1 "<<tri->alpha1<<", alpah2 "<<tri->alpha2 <<", beta "<<tri->beta<<endl;
			//~ cout<<"Match ids: id1 "<<tri->id1<<", id2 "<<tri->id2<<", id3 "<<tri->id3<<endl;
			//cout<<" d1 "<<d1 <<" d2 "<<d2 << " d3 "<<d3<<endl;
			return tri;
		}
		tri=tri->next;
	}
	TriangleCatalogEntry * empty;
	cout<<"no match found"<<endl;
	return empty;

}
bool approx(double b, double a){

	double c= a+0.001;
	double d= a-0.001;
	if(c>b && d<b){
		return true;
	}
	return false;
}







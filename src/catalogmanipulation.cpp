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
	deg_vector[0]=deg1;	
	deg_vector[1]=deg2;
	deg_vector[2]=deg3;

	TriangleCatalogEntry * tri=this->head_trilog->next;
	
	while(tri!=NULL){
		//Initialisiert die Iteratoren
		vector<double> tri_vector(3);
		tri_vector[0]=tri->alpha1;
		tri_vector[1]=tri->alpha2;
		tri_vector[2]=tri->beta;
		
	 	//cout<<" deg->alpa1 "<<deg_vector[0]<<" deg->alpah2 "<<deg_vector[1] <<" deg->beta "<<deg_vector[2]<<endl;
		//cout<<" tri->alpa1 "<<tri->alpha1<<" tri->alpah2 "<<tri->alpha2 <<" tri->beta "<<tri->beta<<endl;
		//Sortiert Vektoren
		sort(tri_vector.begin(), tri_vector.end(), comparator);
		sort(deg_vector.begin(), deg_vector.end(), comparator);
	
		if(approx(deg_vector[0], tri_vector[0])&& approx(deg_vector[1], tri_vector[1])&&approx(deg_vector[2], tri_vector[2])){				
			cout<<"match found"<<endl;
			return tri;
		}
		tri=tri->next;
	}
	TriangleCatalogEntry * empty;
	cout<<"no match found"<<endl;
	return empty;

}
bool approx(double b, double a){

	double c= a+0.1;
	double d=a-0.1;
	if(c>b && d<b){
		return true;
	}
	return false;
}







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

class StarCatalogEntry{
public:
		int id;
		double x;
		double y;
		double z;
		double mag;
		StarCatalogEntry(int, double, double, double, double);

};

StarCatalogEntry::StarCatalogEntry(int id, double x, double y, double z, double mag){

		this-> id=id;
		this -> x=x;
		this -> y=y;
		this -> z=z;
		this -> mag=mag;
}

class TriangelCatalogEntry{

	int id1;
	int id2;
	int id3;
	double beta;
	double alpha1;
	double alpha2;
	TriangelCatalogEntry(int , int , int , double , double , double );

};

TriangelCatalogEntry::TriangelCatalogEntry(int id1, int id2, int id3, double beta, double alpha1, double alpha2){
	this-> id1=id1;
	this -> id2=id2;
	this -> id3=id3;
	this -> beta=beta;
	this -> alpha1=alpha1;
	this -> alpha2=alpha2;
}

class Catalog{
public:
		StarCatalogEntry starlog[];
		TriangelCatalogEntry trilog[];
		void makeCatalog();
			

};
int getNumberOfLines(FILE *fp){

	char ch=0;
	int lines = 0;
	while(!feof(fp)){
	  ch = fgetc(fp);
	  if(ch == '\n'){
	    lines++;
	  }
	}
	return lines;

}


void Catalog::makeCatalog(){

	FILE *file;
	file= fopen("catalog.txt", "r");
	double size=getNumberOfLines(file);
	fclose(file);
	file= fopen("catalog.txt", "r");
	StarCatalogEntry[] starlog = new StarCatalogEntry[size-1];
	this->trilog= TriangelCatalogEntry[size-1];
	
	double azimut,elevation,mag;
	int index;
	double x,y,z;
	while(!feof(file)){
		
		
		fscanf(file, "%d %lf %lf %lf\n", &index, &azimut,&elevation,&mag );
		if (mag<6.5){		
			
			double f=0.25; //Brennweite in Millimeter...vielleicht lieber 10 pc ??
			//**Umrechnung in Kartesische Koordinaten**	
			x= f*sin(elevation)*cos(azimut);
			y=f*sin(elevation)*sin(azimut);
			z=f*cos(elevation);
			StarCatalogEntry e(index, x,y,z,mag);
			starlog[index]=e;
		}


	}		
}	



int main(){


	Catalog c;
	c.makeCatalog();

}
	



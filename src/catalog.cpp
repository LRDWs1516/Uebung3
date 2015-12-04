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
		StarCatalogEntry *before;
		StarCatalogEntry  *next;
		int id;
		double x;
		double y;
		double z;
		double mag;
		double azimut;
		double elevation;
		StarCatalogEntry(StarCatalogEntry*, int, double, double, double, double, double, double);
		void setNext(StarCatalogEntry*);
		StarCatalogEntry* getNext();
};

StarCatalogEntry::StarCatalogEntry(StarCatalogEntry *before, 
 int id, double x, double y, double z, double mag, double azimut, double elevation){
		this->before=before;
		this-> id=id;
		this -> x=x;
		this -> y=y;
		this -> z=z;
		this -> mag=mag;
		this -> azimut=azimut;
		this-> elevation=elevation;
}
void StarCatalogEntry::setNext(StarCatalogEntry *next){
		this->next=next;

}

StarCatalogEntry* StarCatalogEntry::getNext(){
		return this->next;

}


class TriangleCatalogEntry{
public:
	TriangleCatalogEntry *before;
	TriangleCatalogEntry *next;
	int id1;
	int id2;
	int id3;
	double beta;
	double alpha1;
	double alpha2;
	TriangleCatalogEntry(int , int , int , double , double , double );
	void setBefore(TriangleCatalogEntry*);
	void setNext(TriangleCatalogEntry*);
	TriangleCatalogEntry* getBefore();
	TriangleCatalogEntry* getNext();
};



//Eintrag für ein Dreieck, immer das kleinste
TriangleCatalogEntry::TriangleCatalogEntry(int id1, int id2, int id3, double beta, double alpha1, double alpha2){
	this-> id1=id1;
	this -> id2=id2;
	this -> id3=id3;
	this -> beta=beta; //Winkel im Dreieck, alpah1+alpha2 sei minimal, 180-alpha1-alpha2=beta
	this -> alpha1=alpha1;
	this -> alpha2=alpha2;
}

void TriangleCatalogEntry::setBefore(TriangleCatalogEntry* before){
	this->before=before;
}

TriangleCatalogEntry* TriangleCatalogEntry::getBefore(){
	return this->before;
}
void TriangleCatalogEntry::setNext(TriangleCatalogEntry* next){
	this->next=next;
}

TriangleCatalogEntry* TriangleCatalogEntry::getNext(){	
	return this->next;
}


class Catalog{
public:
	StarCatalogEntry *head_starlog;
	TriangleCatalogEntry *head_trilog;
	void makeCatalog();
	TriangleCatalogEntry searchTriangle(StarCatalogEntry* );	

};

TriangleCatalogEntry Catalog::searchTriangle(StarCatalogEntry* self){

	double alpha1;
	StarCatalogEntry *star1;
	double alpha2;
	StarCatalogEntry *star2;

	StarCatalogEntry *star= this -> head_starlog->next;	

	double thisalpha;
	while(star!= NULL){
		if (star->id!=self->id){
			thisalpha=(self->elevation)-(star->elevation);

			if(thisalpha< alpha1 || alpha1==NULL){
				star1=star;
				alpha1=thisalpha;
			}else if(thisalpha< alpha2 || alpha2==NULL){
				star2=star;
				alpha2=thisalpha;
			}
		
		}	
		star=star->next;
	}
	
	double beta= 180-(alpha1+alpha2);
	TriangleCatalogEntry t= TriangleCatalogEntry(self->id, star1->id, star2->id,beta, alpha1, alpha2);
	return t;
}


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
	//StarCatalogEntry *log[] = new StarCatalogEntry[size-1];
	//this->trilog= TriangelCatalogEntry[size-1]();
	
	double azimut,elevation,mag;
	int index; 
	StarCatalogEntry  *s_before=this->head_starlog;
	StarCatalogEntry  *next;	
	double x,y,z;
	while(!feof(file)){
		//**Sternenkatalog wird als Verkettete Liste gespeichert**
		
		fscanf(file, "%d %lf %lf %lf\n", &index, &azimut,&elevation,&mag );
		if (mag<6.5){		
			
			double f=0.25; //Brennweite in Millimeter...vielleicht lieber 10 pc ??, radius eig egal!
			//**Umrechnung in Kartesische Koordinaten**	
			x= f*sin(elevation)*cos(azimut);
			y=f*sin(elevation)*sin(azimut);
			z=f*cos(elevation);
			StarCatalogEntry e(s_before,index, x,y,z,mag, azimut, elevation);
			s_before->next=&e;			
			s_before=&e;	
		}
	}	

	StarCatalogEntry *star=head_starlog->next;
	TriangleCatalogEntry *t_before=this->head_trilog;
	while (star!=NULL){
		TriangleCatalogEntry t=searchTriangle(star);
		t.setBefore(t_before);
		t_before=&t;
	}
	
}	



int main(){


	Catalog c;
	c.makeCatalog();

}
	



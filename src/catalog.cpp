#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include "catalog.h"

using namespace std;

StarCatalogEntry::StarCatalogEntry(StarCatalogEntry *before, 
 int id, double x, double y, double z, double mag, double rectascension, double declination){
		this->before=before;
		this-> id=id;
		this -> x=x;
		this -> y=y;
		this -> z=z;
		this -> mag=mag;
		this -> rectascension=rectascension;
		this-> declination=declination;
		this->next=NULL;
}

StarCatalogEntry::StarCatalogEntry(){
	this->next=NULL;
	this->id=-88;
	this->before=NULL;
}

void StarCatalogEntry::setNext(StarCatalogEntry *next){
		this->next=next;
}

StarCatalogEntry* StarCatalogEntry::getNext(){
		return this->next;

}

TriangleCatalogEntry::TriangleCatalogEntry(){
	this->before=NULL;
	this->next=NULL;
	for(int i=0; i<3; i++){
		this->stars[i] = NULL;
	}
	this->id1=-88;
}

//Eintrag für ein Dreieck, immer das kleinste, Winkel in Rad
TriangleCatalogEntry::TriangleCatalogEntry(int id1, int id2, int id3, double beta, double alpha1, double alpha2){
	this -> id1=id1; 
	this -> id2=id2;
	this -> id3=id3;
	this -> beta=beta; //Winkel im Dreieck, alpah1+alpha2 sei minimal,PI-alpha1-alpha2=beta
	this -> alpha1=alpha1;
	this -> alpha2=alpha2;
	this -> before=NULL;
	this -> next=NULL;
	for(int i=0; i<3; i++){
		this->stars[i] = NULL;
	}
}


void TriangleCatalogEntry::setStars(StarCatalogEntry *stars[3]){
	this -> stars[0] = stars[0];
	this -> stars[1] = stars[1];
	this -> stars[2] = stars[2];
}

StarCatalogEntry ** TriangleCatalogEntry::getStars(void){
	return this->stars;
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

TriangleCatalogEntry Catalog::searchTriangle(StarCatalogEntry* self){

	double d1=5; //Kugel hat durchmesser=2 daher sollte es nichts größer als 5 geben
	StarCatalogEntry *star1;
	double d2=5;
	StarCatalogEntry *star2;

	StarCatalogEntry *star= this -> head_starlog->next;	

	double distance;

	while(star!= NULL){
		if (star->id!=self->id){
			double x= (self->x-star->x)*(self->x-star->x);
			double y=(self->y-star->y)*(self->y-star->y);	
			double z=(self->z-star->z)*(self->z-star->z);
			distance=sqrt(x+y+z);
			int nr=star->id;
			
			if(distance<d1){
				star1=star;
				d1=distance;
			}else if(star->id != star1->id && distance<d2){
				star2=star;
				d2=distance;
			}

				
		}
	
		star=star->next;
	}
	//cout<<"d1 "<<d1 << " d2 "<<d2 <<endl;
	double alpha1= acos(d1*d1/-4+1/2);
	double alpha2= acos(d2*d2/-4+1/2);
	double x= (star1->x-star2->x)*(star1->x-star2->x);
	double y=(star1->y-star2->y)*(star1->y-star2->y);	
	double z=(star1->z-star2->z)*(star1->z-star2->z);
	double d3=sqrt(x+y+z);	
	

	double beta= acos((d3*d3-d1*d1-d2*d2)/(-2*d2*d1));
	TriangleCatalogEntry t = TriangleCatalogEntry(self->id, star1->id, star2->id,beta, alpha1, alpha2);

	//speichert die drei Sterne im Dreieck in ein Array und fügt sie dem TriangleCatalogEntry hinzu
	StarCatalogEntry* stars[3];
	stars[0] = self;
	stars[1] = star1;
	stars[2] = star2;
	t.setStars(stars);


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

int getNumberofLines(ifstream & file){

    int lines = 0;
    string line;

    if(!file.is_open()) return -1; //catch closed streams

    while (getline(file, line)) lines++; //count lines
    return lines;
}

void Catalog::setTriangleCatalog(){
	StarCatalogEntry *star=this->head_starlog->next;
	//cout<< "Starlog head next: "<<this->head_starlog->next->id <<endl;
	TriangleCatalogEntry *t_b=this->head_trilog;
	
	while (star!=NULL ){
	
		TriangleCatalogEntry *t=new TriangleCatalogEntry();
		*t=searchTriangle(star);	
		
		t->before=t_b;	
		t_b->next= t;
		
		//t_before->setNext(&t);
		//cout << "trilog next " << t_before->next->id1 <<" "<<t_before->next->id2<<" "<<t_before->next->id3<<endl;
		t_b= t;
		star=star->next;
	}
	
}
TriangleCatalogEntry* Catalog::getHeadTrilog(){

	return this->head_trilog;
}

void Catalog::makeCatalog(const char * fname){
	this->cSize = 0;
	printf("Start Creating Catalogs\n");
	FILE *file;
	file= fopen(fname, "r");
	double size=getNumberOfLines(file);
	fclose(file);
	file= fopen(fname, "r");
	
	double rectascension,declination,mag;
	int index; 
	StarCatalogEntry *s_before = this->head_starlog;
	//printf(" vorgänger  (head) %d \n",s_before->id);
	StarCatalogEntry  *next;	
	double x,y,z;
	while(!feof(file)){
		//**Sternenkatalog wird als Verkettete Liste gespeichert**
		
		fscanf(file, "%d %lf %lf %lf\n", &index, &rectascension,&declination,&mag );
		if (mag<6.5){		
			double f=1; //Brennweite in Millimeter...vielleicht lieber 10 pc ??, radius eig egal!
			//**Umrechnung in Kartesische Koordinaten**	
			rectascension=rectascension*M_PI/180;
			declination=declination*M_PI/180;
			x= f*sin(declination )*cos(rectascension);
			y= f*sin(declination)*sin(rectascension);
			z= f*cos(declination);
			StarCatalogEntry *e= new StarCatalogEntry(s_before,index, x,y,z,mag, rectascension, declination);
			s_before->next= e;
			s_before=e;
			cSize++;
		}
	}

	this->head_starlog->before = NULL;
	this->current = this->head_starlog;
}

StarCatalogEntry& Catalog::operator++(int i){
	this->current = this->current->next;
	return *this->current;
}

StarCatalogEntry& Catalog::operator--(int i){
	this->current = this->current->before;
	return *this->current;
}

int Catalog::size() {
	return this->cSize;

}

void Catalog::reset(){
	this->current = this->head_starlog;
}

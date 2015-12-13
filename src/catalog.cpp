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

//Eintrag für ein Dreieck, immer das kleinste
TriangleCatalogEntry::TriangleCatalogEntry(int id1, int id2, int id3, double beta, double alpha1, double alpha2){
	this -> id1=id1;
	this -> id2=id2;
	this -> id3=id3;
	this -> beta=beta; //Winkel im Dreieck, alpah1+alpha2 sei minimal, 180-alpha1-alpha2=beta
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

	double alpha1=360;
	StarCatalogEntry *star1;
	double alpha2=360;
	StarCatalogEntry *star2;

	StarCatalogEntry *star= this -> head_starlog->next;	

	double thisalpha;
	while(star!= NULL){
		if (star->id!=self->id){
			//Hier bin ich mir nicht sicher ob die Winkel so richtig berechnet werden zwischen zwei beliebigen sternen zum hauptstern
			
			thisalpha=fabs((self->declination)-(star->declination));
			int nr=star->id;		
			if(thisalpha< fabs(alpha1)){
				star1=star;
				alpha1=thisalpha;
			}else if(thisalpha< fabs(alpha2) && star->id != star1->id){			
				star2=star;
				alpha2=thisalpha;
			}
		
		}	
		star=star->next;
	}

	double beta= fmod(180.0-fabs(alpha1+alpha2), 360.0);
	TriangleCatalogEntry t = TriangleCatalogEntry(self->id, star1->id, star2->id,beta, alpha1, alpha2);

	//speichert die drei Sterne im Dreieck in ein Array und fügt sie dem TriangleCatalogEntry hinzu
	StarCatalogEntry* stars[3];
	stars[0] = self;
	stars[1] = star1;
	stars[2] = star2;
	t.setStars(stars);

	//printf("self %d id2 %d id3 %d beta %f alpha1 %f alpha2 %f \n", t.id1, t.id2,t.id3,t.beta,t.alpha1,t.alpha2);
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
	cout<< "Starlog head next: "<<this->head_starlog->next->id <<endl;
	TriangleCatalogEntry *t_before=this->head_trilog;
	int i=0;
	while (star!=NULL && i<10){
		i++;
		TriangleCatalogEntry t=searchTriangle(star);
		t.setBefore(t_before);
		if (i==1){
			this->head_trilog->setNext(&t);
		}else{
			t_before->next=&t;
		}
		
		//t_before->setNext(&t);
		//cout << "trilog next " << t_before->next->id1 <<" "<<t_before->next->id2<<" "<<t_before->next->id3<<endl;
		t_before=&t;
		cout<<i<<" t before "<< t.before->id1<< " t id1 "<< t.id1<<endl;
		cout<<" head trilog next : "<< this->head_trilog->next->id1<<endl;
		star=star->next;
	}
	cout<<" Trilog head in setTriangleCatalog "<< this->head_trilog->next->id1 <<endl;
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
			double f=0.25; //Brennweite in Millimeter...vielleicht lieber 10 pc ??, radius eig egal!
			//**Umrechnung in Kartesische Koordinaten**	
			x= f*sin(declination)*cos(rectascension);
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

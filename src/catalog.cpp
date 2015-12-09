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
		
		StarCatalogEntry();
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
	TriangleCatalogEntry();

	void setBefore(TriangleCatalogEntry*);
	void setNext(TriangleCatalogEntry*);
	TriangleCatalogEntry* getBefore();
	TriangleCatalogEntry* getNext();
};
TriangleCatalogEntry::TriangleCatalogEntry(){
	this->before=NULL;
	this->next=NULL;
}


//Eintrag für ein Dreieck, immer das kleinste
TriangleCatalogEntry::TriangleCatalogEntry(int id1, int id2, int id3, double beta, double alpha1, double alpha2){
	this-> id1=id1;
	this -> id2=id2;
	this -> id3=id3;
	this -> beta=beta; //Winkel im Dreieck, alpah1+alpha2 sei minimal, 180-alpha1-alpha2=beta
	this -> alpha1=alpha1;
	this -> alpha2=alpha2;
	this->before=NULL;
	this->next=NULL;
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
	StarCatalogEntry *head_starlog = new StarCatalogEntry();
	TriangleCatalogEntry *head_trilog=new TriangleCatalogEntry();
	void makeCatalog(const char * fname);
	TriangleCatalogEntry searchTriangle(StarCatalogEntry* );	

};

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
			
			thisalpha=fabs((self->elevation)-(star->elevation));
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
	TriangleCatalogEntry t= TriangleCatalogEntry(self->id, star1->id, star2->id,beta, alpha1, alpha2);
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


void Catalog::makeCatalog(const char * fname){
	
	printf("Start Creating Catalogs\n");
	FILE *file;//ifstream fl;
	file= fopen(fname, "r");//fl.open("catalog.txt", ios::in | ios::binary);
	double size=getNumberOfLines(file); //double size = getNumberofLines(fl);
	fclose(file);//fl.seekg(0, fl.beg);
	file= fopen(fname, "r");
	
	double azimut,elevation,mag;
	int index; 
	StarCatalogEntry  *s_before=this->head_starlog;
	//printf(" vorgänger  (head) %d \n",s_before->id);
	StarCatalogEntry  *next;	
	double x,y,z;
	while(!feof(file)){
		//**Sternenkatalog wird als Verkettete Liste gespeichert**
		
		fscanf(file, "%d %lf %lf %lf\n", &index, &azimut,&elevation,&mag ); 	
		if (mag<6.5){		
			//printf(" Starcatalog index %d  \n", index);
			double f=0.25; //Brennweite in Millimeter...vielleicht lieber 10 pc ??, radius eig egal!
			//**Umrechnung in Kartesische Koordinaten**	
			x= f*sin(elevation)*cos(azimut);
			y=f*sin(elevation)*sin(azimut);
			z=f*cos(elevation);
			StarCatalogEntry *e= new StarCatalogEntry(s_before,index, x,y,z,mag, azimut, elevation);
			//printf(" vorgänger %d \n",e->before->id);
			s_before->next= e;
			//printf(" NACHfolger des vorgänger %d \n",e->before->next->id);	
			//printf("-------------------------------------------------------------\n");	
			s_before=e;	
		}
	}	

	StarCatalogEntry *star=this->head_starlog->next;
	TriangleCatalogEntry *t_before=this->head_trilog;
	while (star!=NULL){
		//printf("Trianglecatalog index %d \n", star->id);
		TriangleCatalogEntry t=searchTriangle(star);
		t.setBefore(t_before);
		t_before=&t;
		star=star->next;
	}
	
}	
	



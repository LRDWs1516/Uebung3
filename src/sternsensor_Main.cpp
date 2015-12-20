//Uebung 2 LRD Beispiel
//This Program has a problem: X is vertical, Y is horizontal

#include "catalog.h"
#include "PointArray.h"
#include "catalogmanipulation.h"
#include "rotationsmatrix.h"
#include "vectorCatalog.h"
#include <thread>

using namespace std;

#define FOCUS		25.0f	//in mm
#define PIXELSIZE	5.8f	//in um

void sleepcp(int milliseconds) // cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}

Point2D getfov(double f, double px, Image im){
	double a_v[2]; a_v[0] = 0; a_v[1] = 0;
 	a_v[0]=2.0*atan((im.w*px*0.000001)/(2.0*f/1000.0));
 	a_v[1]=2.0*atan((im.h*px*0.000001)/(2.0*f/1000.0));
	Point2D returnP(a_v[0], a_v[1]);
	return returnP;
}

//rad/Pixel
double getAvgAng(Point2D angs, Image im){
	double a = angs.x / im.w;
	double b = angs.y / im.h;
	return a;
}

double getAng(Point2D a, Point2D b, double app){
	return (sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y))*app);
}

//Gibt Winkel beta aus zeichnung zurück
//Berechnet Winkel bac
double getStarAng(Point2D a, Point2D b, Point2D c){
	Point2D av, bv;
	double 	IavI = 0, IbvI = 0;

	av = Point2D(b.x-a.x, b.y-a.y);
	bv = Point2D(c.x-a.x, c.y-a.y);
	IavI = sqrt((av.x*av.x + av.y*av.y));
	IbvI = sqrt((bv.x*bv.x + bv.y*bv.y));

	return cos((av.x*bv.x+av.y*bv.y)/(IavI*IbvI));
}

//Gibt Winkel beta aus zeichnung zurück
//Annahme: gesucher winkel bei b, ergibt gleiches Ergebnis wie getStarAng
double getStarAng2(Point2D a, Point2D b, Point2D c){
	double d1=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
	double d2=sqrt((c.x-b.x)*(c.x-b.x)+(c.y-b.y)*(c.y-b.y));
	double d3=sqrt((a.x-c.x)*(a.x-c.x)+(a.y-c.y)*(a.y-c.y));
	return  acos((d3*d3-d1*d1-d2*d2)/(-2*d2*d1));
}

//Gibt Winkel beta aus Zeichnung zurück
//Annahme: gesucher winkel bei a 
double getStarAng3(Point2D a, Point2D b, Point2D c){
	double d1=sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
	double d2=sqrt((c.x-a.x)*(c.x-a.x)+(c.y-a.y)*(c.y-a.y));
	double d3=sqrt((b.x-c.x)*(b.x-c.x)+(b.y-c.y)*(b.y-c.y));
	return  acos((d3*d3-d1*d1-d2*d2)/(-2*d2*d1));
}

void generation1(Catalog c, const char * fname2, double alpha2, double alpha1, double beta, double avg){

    clock_t start1 = clock() / (double)CLOCKS_PER_SEC * 1000;
	cout << "Size of Catalogs: " << c.size() << endl;
	clock_t end1 = clock() / (double)CLOCKS_PER_SEC * 1000;
	end1 = clock() / (double)CLOCKS_PER_SEC * 1000;
	Finder f(c.getHeadTrilog());
	cout << "Trianglelist created after " << end1-start1 << "ms" << endl;
	cout << "Listsearching" << endl;
	TriangleCatalogEntry * match = f.getMatch(alpha2, alpha1, beta);
	
	end1 = clock() / (double)CLOCKS_PER_SEC * 1000;
	
	cout << "Listsearch was conclusive after: " << end1-start1 << "ms" << endl;
	cout << "-Match accuracy: " << 0.001*180/M_PI << "°(deg) = " << 0.001*180/M_PI*3600 << "\"(arcsec)" <<  ", bzw. " << 0.001 << " rad -> " << 0.001/avg << " Pixel Fehler"  << endl;
	cout << "-Angles " << match->alpha1 << " " << match->alpha2 << " " << match->beta << endl;
	cout << "-Solution IDs: " << match->id1 << " " << match->id2 << " " << match->id3 << endl;
}

void generation2(StarCatalog cat, const char * fname2, Triangle central, double avg, clock_t mainStart){

	clock_t start = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	clock_t end = clock() / (double)CLOCKS_PER_SEC * 1000; 			//###
	cout << "Creating triangle vector" << endl;
	TriangleCatalog tcat;
	tcat.createCatalog(cat, avg);
	end = clock() / (double)CLOCKS_PER_SEC * 1000; 					//###
	cout << "\t#t#Trianglevector completed after " << end-start << "ms" << endl;
	cout << "__________________________________________________________________________" << endl;
	
	cout << "Vectorsearching" << endl;
	Vector3D solutionAngles(0,0,0);
	RID3 solutionIDs(0,0,0);
	double thres = 0;																								//
	int matchID = 0;																								//
	clock_t vstart = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	while(!tcat.containsTriangle(central, thres, &solutionAngles, &solutionIDs, &matchID) && thres < 0.01) thres += 0.000001;	//iteratives vergroessern des Suchthresholds
	clock_t vend = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	TriangleEntry tmatch = tcat.at(matchID);
	
	if(thres >= 0.01) cout << "--search inconclusive after " << vend-vstart << "ms" << endl;
	else{
		cout << "\t#t#Vectorsearch was conclusive after " << vend-vstart << "ms" << endl;
		cout << "-Match accuracy: " << thres*180/M_PI << "°(deg) = " << thres*180/M_PI*3600 << "\"(arcsec)" <<  " = " << thres << " rad = " << thres/avg << " pixel-units"  << endl;
		cout << "-Alphas: " << solutionAngles.c[0] << " " << solutionAngles.c[1] << " " << solutionAngles.c[2] << endl;
		cout << "-Beta: " << acos(-0.5*(tmatch.getD(0)*tmatch.getD(0) - tmatch.getD(1)*tmatch.getD(1) - tmatch.getD(2)*tmatch.getD(2))/(tmatch.getD(1)*tmatch.getD(2))) << endl;
		cout << "-Solution IDs: " << cat.getMainCatalog().at(solutionIDs.rID[0]).getID() << " " << cat.getMainCatalog().at(solutionIDs.rID[1]).getID() << " " << cat.getMainCatalog().at(solutionIDs.rID[2]).getID() << endl;
	cout << "__________________________________________________________________________" << endl;
	double mainstar[3];
	mainstar[0]=cat.getMainCatalog().at(solutionIDs.rID[0]).getE(0);
	mainstar[1]=cat.getMainCatalog().at(solutionIDs.rID[0]).getE(1);
	mainstar[2]=cat.getMainCatalog().at(solutionIDs.rID[0]).getE(2);
	//cout<<mainstar[0]<<" "<<mainstar[1]<<" "<<mainstar[2]<<endl;

	double nextstar[3];
	nextstar[0]=cat.getMainCatalog().at(solutionIDs.rID[1]).getE(0);
	nextstar[1]=cat.getMainCatalog().at(solutionIDs.rID[1]).getE(1);
	nextstar[2]=cat.getMainCatalog().at(solutionIDs.rID[1]).getE(2);
	//cout<<nextstar[0]<<" "<<nextstar[1]<<" "<<nextstar[2]<<endl;
	Rotation r;
	r.getRotationmatrix(mainstar,nextstar);
	r.printRotor(1);
	quaternion quat= r.getQuaternion(mainstar,nextstar);
	r.printQuat(quat,1);
	cout << "__________________________________________________________________________" << endl;
	}
}

void generateObjects(const char * fname, PointArray * fst, Image * first, vector<Point2D> * triplet, Triangle * central2, double * avg, double * alpha1, double * alpha2, double * beta){
	clock_t start = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	
	Color white(255,255,255);
	Color red(0,0,255);
	Color pink(255,0,200);
	Color black(0,0,0);
	Color green(0,255,0);

    Image second;
    cout << "Image: " << fname << endl;
    first->readImageFromFile(fname, 1);
    Point2D fov = getfov(FOCUS, PIXELSIZE, *first);
    //Berechnung Kameraparameter
    *avg = getAvgAng(fov, *first); 				//avg rad/pixel
    cout << "-°/Pixel: " << *avg*180/M_PI << endl; 

    second = *first;
    second.thresholdImage(1); 							//dunkle Sterne/Rauschen ausblenden (90 bis 140 scheint funktional)
    second.writeImageToFile("sterne1.bmp"); 			// Zwischenergebnis
    *first = second;

    fst->fill2(second,1); 								//Alle hellen Blobs finden
    fst->getCentrals(*first);							//Zentren der Blobs finden WICHTIG FÜR ERKENNUNG IN STERNENKATALOG
    second.drawBinarysToImage(fst->overlay, pink); 		//Overlay aufzeichnen
    second.drawArrayToImage(fst->startPoints, white); 	//Alle Startpunkte die in fst.fill2() gefunden wurden
    													//gr�n einzeichnen (je nur 1 Pixel pro Objekt)
    cout << "-found " << fst->allObjects.size() << " objects." << endl;
    
    clock_t stop = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
    cout << "\t#t#Imagemanipulation was conclusive after " << stop-start << "ms" << endl;
	cout << "__________________________________________________________________________" << endl;
	
	cout << "Searching for central Triangle in Image" << endl;
	start = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
    *triplet = fst->getCentralTriangle(*first);
    Triangle central(*triplet);
    cout << "-Found central triangle:" << endl;
    central.calculateAlphas(*avg);
    central.setDists();
    *central2 = central;
    
    *alpha1 = getAng((Point2D)triplet->at(0), (Point2D)triplet->at(1), *avg);
    *alpha2 = getAng((Point2D)triplet->at(0), (Point2D)triplet->at(2), *avg);

    *beta = getStarAng2(triplet->at(0), triplet->at(1), triplet->at(2));
	cout << "--BETA 1: " << *beta << endl;
    *beta = getStarAng3(triplet->at(0), triplet->at(1), triplet->at(2));
	cout << "--BETA 2: " << *beta << endl;
    
    cout << "--ALPHA 1: " << central.alphas[0] << endl;
    cout << "--ALPHA 2: " << central.alphas[1] << endl;
    cout << "--ALPHA 3: " << central.alphas[2] << endl; //Vectorsearch gibt 3 Alphawinkel zurück. Bitte lass das zum Prüfen drinne!
	//Markierungen einzeichnen
    first->drawCross((Point2D)triplet->at(0),0,green,0);
    first->drawCross((Point2D)triplet->at(1),0,white,0);
    first->drawCross((Point2D)triplet->at(2),0,red,0);
    first->writeImageToFile("sterne3.bmp");				//Kontrollergebnis speichern
    stop = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	cout << "\t#t#Imagesearch took " << stop-start << "ms" << endl << "__________________________________________________________________________" << endl;
}

void parallelMakeCatalog(const char * fname, Catalog * c, StarCatalog * sc){
	clock_t start = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	c->makeCatalog(fname);
	//c->setTriangleCatalog();
	sc->translateCatalog(*c);
	clock_t stop = clock() / (double)CLOCKS_PER_SEC * 1000; 		//###
	cout << "__________________________________________________________________________" << endl;
	cout << "\t#t#Catalogmaking was conclusive after " << stop-start << "ms" << endl;
	cout << "__________________________________________________________________________" << endl;
}

int main() {
	clock_t mainStart = clock() / (double)CLOCKS_PER_SEC * 1000;
	
	const char * fname1 = "Sterne.bmp";
    const char * fname2 = "catalog.txt";

    Image first, second;
    vector<Point2D> triplet;
    double avg, alpha1, alpha2, beta;
    PointArray fst;
    Triangle central;
    Catalog c;
    StarCatalog cat;
    
    thread t1(generateObjects, fname1, &fst, &first, &triplet, &central, &avg, &alpha1, &alpha2, &beta);
    thread t2(parallelMakeCatalog, fname2, &c, &cat);
    
    t1.join();
    t2.join();
    
	generation2(cat,fname2,central,avg,mainStart);
	
	//Multithreading code for both searchalgorithms
	//thread g1(generation1, c,fname2,alpha2,alpha1,beta,avg);
	//thread g2(generation2, cat,fname2,central,avg, mainStart);
	//g2.join();
	//g1.join();
	
	clock_t mainEnd = clock() / (double)CLOCKS_PER_SEC * 1000;
	cout << "\t#t#Overall time: " << mainEnd-mainStart << "ms" << endl;

    return 0;
}


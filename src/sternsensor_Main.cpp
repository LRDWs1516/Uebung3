//Uebung 2 LRD Beispiel
//This Program has a problem: X is vertical, Y is horizontal

#include "catalog.h"
#include "PointArray.h"
#include "catalogmanipulation.h"
#include "rotationsmatrix.h"
#include "vectorCatalog.h"

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

double getAvgAng(Point2D angs, Image im){
	double a = angs.x / im.w;
	double b = angs.y / im.h;
	return a;
}

double getAng(Point2D a, Point2D b, double app){
	return (sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y))*app);
}

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

int main() {

	Color white(255,255,255);
	Color red(0,0,255);
	Color pink(255,0,200);
	Color black(0,0,0);
	Color green(0,255,0);

    Image first, second;
    PointArray fst;
    first.readImageFromFile("Sterne.bmp", 1);

    second = first;
    second.thresholdImage(0); 							//dunkle Sterne/Rauschen ausblenden (90 bis 140 scheint funktional)
    second.writeImageToFile("sterne1.bmp"); 			// Zwischenergebnis
    first = second;

    fst.fill2(second,1); 								//Alle hellen Blobs finden
    fst.getCentrals();									//Zentren der Blobs finden WICHTIG FÜR ERKENNUNG IN STERNENKATALOG
    second.drawBinarysToImage(fst.overlay, pink); 		//Overlay aufzeichnen
    second.drawArrayToImage(fst.startPoints, white); 	//Alle Startpunkte die in fst.fill2() gefunden wurden
    													//gr�n einzeichnen (je nur 1 Pixel pro Objekt)
    second.writeImageToFile("sterne2.bmp"); 			//Zwischenergebnis speichern

    cout << "found " << fst.allObjects.size() << " objects." << endl;

    for(int i = 0; i<fst.allObjects.size(); i++){		//Alle gefundenen Blobs in unterschiedlichen Farben einzeichnen
		second.drawArrayToImage(fst.allObjects.at(i), Color(50+i*20,100+i*10,200 + i*5));
    }

	//Calculate IDs of most central Triangle of stars (central 0, triplet 0 and 1)
    second.writeImageToFile("sterne3.bmp");//Bild speichern

    vector<Point2D> triplet;
    triplet = fst.getCentralTriangle(first);

    Point2D fov = getfov(FOCUS, PIXELSIZE, first);
    double avg = getAvgAng(fov, first);
    double alpha1 = getAng((Point2D)triplet.at(0), (Point2D)triplet.at(1), avg);
    double alpha2 = getAng((Point2D)triplet.at(0), (Point2D)triplet.at(2), avg);
	//double beta= fmod(M_PI-fabs(alpha1+alpha2), M_PI);
	double beta = getStarAng((Point2D)triplet.at(0), (Point2D)triplet.at(1), (Point2D)triplet.at(2));

    cout << "beta " << beta*180/M_PI << endl;

    first.drawCross((Point2D)triplet.at(0),0,green,0);
    first.drawCross((Point2D)triplet.at(1),0,white,0);
    first.drawCross((Point2D)triplet.at(2),0,red,0);

    first.writeImageToFile("sterne4.bmp");

    cout << "Catalogmaking:" << endl;
	Catalog c;
	c.makeCatalog("catalog.txt");
	c.setTriangleCatalog();

	cout << "Done Catalogmaking" << endl;
	
	Finder f(c.getHeadTrilog());
	TriangleCatalogEntry * match = f.getMatch(alpha2, alpha1, beta);

	//cout << match->alpha1 << endl;
	//StarCatalog cat;
	//cat.translateCatalog(c);
/*
	//Simple Iteratorversuche
	while(c.current->next != NULL){
		printf("id: %d\n",c++.id);
	}

	while(c.current->before != NULL){
		printf("id: %d\n",c--.id);
	}
*/
	cout << "Done" << endl;

    return 0;



}


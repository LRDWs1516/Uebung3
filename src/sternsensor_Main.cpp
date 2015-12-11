//Uebung 2 LRD Beispiel
//This Program has a problem: X is vertical, Y is horizontal

#include "catalog.h"
#include "bildmanipulation.h"
#include "catalogmanipulation.h"
#include <exception>
#include <stdexcept>

int main() {

	Color white(255,255,255);
	Color red(0,0,255);
	Color pink(255,0,200);
	Color black(0,0,0);

    Image first, second;
    PointArray fst;
    first.readImageFromFile("Sterne.bmp", 1);

    second = first;
    second.thresholdImage(140); 						//dunkle Sterne/Rauschen ausblenden (90 bis 140 scheint funktional)
    second.writeImageToFile("sterne1.bmp"); 			// Zwischenergebnis
    first = second;

    fst.fill2(second,1); 								//Alle hellen Blobs finden
    fst.getCentrals(0);									//Zentren der Blobs finden WICHTIG FÜR ERKENNUNG IN STERNENKATALOG
    second.drawBinarysToImage(fst.overlay, pink); 		//Overlay aufzeichnen
    second.drawArrayToImage(fst.startPoints, white); 	//Alle Startpunkte die in fst.fill2() gefunden wurden
    													//gr�n einzeichnen (je nur 1 Pixel pro Objekt)
    second.writeImageToFile("sterne2.bmp"); 			//Zwischenergebnis speichern

    cout << "found " << fst.allObjects.size() << " objects." << endl;

    for(int i = 0; i<fst.allObjects.size(); i++){		//Alle gefundenen Blobs in unterschiedlichen Farben einzeichnen
		second.drawArrayToImage(fst.allObjects.at(i), Color(50+i*20,100+i*10,200 + i*5));
    }

    second.writeImageToFile("sterne3.bmp"); 			//Bild speichern

    cout << "Done!" << endl;

    cout << "Catalogmaking:" << endl;
	Catalog c;
	c.makeCatalog("catalog.txt");
	//c.printCatalog();
	printf("id: %d\n", c.current->id);
	printf("id: %d\n", c.head_starlog->id);

	//Simple Iteratorversuche
	for(c.reset(); c.current->next != NULL; c++){
		printf("id: %d\n", c.current->id);
	}
	printf("id: %d\n", c.current->id);

	for(c; c.current->before != NULL; c--){
		printf("id: %d\n", c.current->id);
	}
	printf("id: %d\n", c.current->id);

	cout << c.size() << endl;

	cout << "Done" << endl;

    return 0;



}


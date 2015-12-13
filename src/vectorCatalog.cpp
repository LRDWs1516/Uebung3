 
 #include "vectorCatalog.h"
 
 StarEntry::StarEntry(int id, double x, double y, double z, double mag, double rectascension, double declination){
	 this->id = id;
	 this->x = x;
	 this->y = y;
	 this->z = z;
	 this->mag = mag;
	 this->rectascension = rectascension;
	 this-> declination = declination;
 }
 
 int StarEntry::getID(){ return this->id;}
 
 TriangleEntry::TriangleEntry(int id1, int id2, int id3, double alpha[3], double beta[3]){
	 this->id1 = id1;
	 this->id2 = id2;
	 this->id3 = id3;
	 this->alpha[0] = alpha[0];
	 this->alpha[1] = alpha[1];
	 this->alpha[2] = alpha[2];
	 this->beta[0] = beta[0];
	 this->beta[1] = beta[1];
	 this->beta[2] = beta[2];
 }
 
 void StarCatalog::translateCatalog(Catalog c){
	 while(c.current->next != NULL){
		StarEntry newest(c.current->id, c.current->x, c.current->y,
						c.current->z, c.current->mag, c.current->rectascension, c.current->declination);
		mainCatalog.push_back(newest);
		cout << "new: " << c++.id << endl;
	}
	StarEntry newest(c.current->id, c.current->x, c.current->y,
					c.current->z, c.current->mag, c.current->rectascension, c.current->declination);
	mainCatalog.push_back(newest);
	c.reset();
 }

 
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

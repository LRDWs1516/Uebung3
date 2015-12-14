
 #include "vectorCatalog.h"
 
 Triangle::Triangle(vector<Point2D> corners){
	if(corners.size() != 3){
		cerr << "WRONG VECTOR SIZE" << endl;
		throw corners;
	}
	A = corners.at(0);
	B = corners.at(1);
	C = corners.at(2);
	alphas[0] = 0;
	alphas[1] = 0;
	alphas[2] = 0;
}
void Triangle::setCorners(vector<Point2D> corners){
	A = corners.at(0);
	B = corners.at(1);
	C = corners.at(2);
}

void Triangle::calculateAlphas(double avg){
	alphas[0] = this->A.getDistance(this->B);
	alphas[1] = this->A.getDistance(this->C);
	alphas[2] = this->B.getDistance(this->C);
	alphas[0] *= avg;
	alphas[1] *= avg;
	alphas[2] *= avg;
	
}
 
 RID3::RID3(int id[3]){
	this->rID[0] = id[0];
	this->rID[1] = id[1];
	this->rID[2] = id[2];
}

Vector3D::Vector3D(double c[3]){
	this->c[0] = c[0];
	this->c[1] = c[1];
	this->c[2] = c[2];
}

Vector3D::Vector3D(double c0, double c1, double c2){
	this->c[0] = c0;
	this->c[1] = c1;
	this->c[2] = c2;
}

Vector3D::Vector3D(){
	this->c[0] = 0;
	this->c[1] = 0;
	this->c[2] = 0;
}

double Vector3D::abs(){
	return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
}

double Vector3D::scalarMult(Vector3D second){
	return (this->c[0]*second.c[0]+this->c[1]*second.c[1]+this->c[2]*second.c[2]);
}

double Vector3D::angleTo(Vector3D second){
	double a = scalarMult(second);
	double b = abs() * second.abs();
	return acos(a/b);
}
 
 StarEntry::StarEntry(int id, double x, double y, double z, double mag, double rectascension, double declination){
	 this->id = id;
	 this->x = x;
	 this->y = y;
	 this->z = z;
	 this->mag = mag;
	 this->rectascension = rectascension;
	 this-> declination = declination;
 }
 StarEntry::StarEntry(){
	 this->id = 0;
	 this->x = 0;
	 this->y = 0;
	 this->z = 0;
	 this->mag = 0;
	 this->rectascension = 0;
	 this-> declination = 0;
 }
 
 int StarEntry::getID(){ return this->id;}
 
 double StarEntry::getE(int elem){
	 switch(elem){
		 case 0: return this->x;
		 case 1: return this->y;
		 case 2: return this->z;
		 case 3: return this->mag;
		 case 4: return this->rectascension;
		 case 5: return this->declination;
		 default:
			cout << "WRONG TYPE" << endl;
			throw -1;
	 }
 }
 
 double StarEntry::distanceTo(StarEntry second){
	 return sqrt((this->x-second.getE(0))*(this->x-second.getE(0))+(this->y-second.getE(1))*(this->y-second.getE(1))
				+(this->z-second.getE(2))*(this->z-second.getE(2)));
 }
 
 TriangleEntry::TriangleEntry(RID3 rID, Vector3D alpha){
	 this->alpha[0] = alpha.c[0];
	 this->alpha[1] = alpha.c[1];
	 this->alpha[2] = alpha.c[2];
	 this->rID[0] = rID.rID[0];
	 this->rID[1] = rID.rID[1];
	 this->rID[2] = rID.rID[2];
 }

double TriangleEntry::getalpha(int i){
	return this->alpha[i];
}

int TriangleEntry::getID(int i){
	return this->rID[i];
}
 
  vector<StarEntry> StarCatalog::getMainCatalog(){
	return this->mainCatalog;
 }
 
 void StarCatalog::translateCatalog(Catalog c){
	 c.reset();
	 while(c.current->id < 0)c++;
	 while(c.current->next != NULL){
		StarEntry newest(c.current->id, c.current->x, c.current->y,
						c.current->z, c.current->mag, c.current->rectascension, c.current->declination);
		mainCatalog.push_back(newest);
		c++;
	}
	StarEntry newest(c.current->id, c.current->x, c.current->y,
					c.current->z, c.current->mag, c.current->rectascension, c.current->declination);
	mainCatalog.push_back(newest);
	c.reset();
 }
 
 RID3 StarCatalog::findClosest2(int id){
	 double d[2] = {1000,1000};
	 int rid[3] = {-1,-1,-1};
	 for(int i = 0; i < this->mainCatalog.size(); i++){
		 double dist = this->mainCatalog.at(i).distanceTo(this->mainCatalog.at(id));
		 if(dist < d[0] && id != i){
			 d[0] = dist;
			 rid[1] = i;
		 }
	 }
	 for(int i = 0; i < this->mainCatalog.size(); i++){
		 double dist = this->mainCatalog.at(i).distanceTo(this->mainCatalog.at(id));
		 if(dist > d[0] && dist < d[1] && id != i){
			 d[1] = dist;
			 rid[2] = i;
		 }
	 }
	 //cout << d[0] << " " << d[1] << " " << rid[0] << " " << rid[1] << endl;
	 RID3 re(rid);
	 return re;
 }
 
 Vector3D StarCatalog::triangleGetAlpha(RID3 t){
	 Vector3D r(0,0,0);
	 StarEntry s[3];
	 s[0] = this->mainCatalog.at(t.rID[0]);
	 s[1] = this->mainCatalog.at(t.rID[1]);
	 s[2] = this->mainCatalog.at(t.rID[2]);
	 Vector3D starVecs[3];
	 starVecs[0] = Vector3D(s[0].getE(0), s[0].getE(1), s[0].getE(2));
	 starVecs[1] = Vector3D(s[1].getE(0), s[1].getE(1), s[1].getE(2));
	 starVecs[2] = Vector3D(s[2].getE(0), s[2].getE(1), s[2].getE(2));
	 
	 r.c[0] = starVecs[0].angleTo(starVecs[1]);
	 r.c[1] = starVecs[1].angleTo(starVecs[2]);
	 r.c[2] = starVecs[2].angleTo(starVecs[0]);
	 
	 return r;
 }
 
 void TriangleCatalog::createCatalog(StarCatalog c, double pxAng){
	 vector<StarEntry> cat = c.getMainCatalog();
	 
	 for(int i = 0; i<cat.size(); i++){
		 RID3 curr = c.findClosest2(i);
		 curr.rID[0] = i;
		 Vector3D alphas = c.triangleGetAlpha(curr);
		 TriangleEntry additional(curr, alphas);
		 this->mainCatalog.push_back(additional);
		 //cout << additional.getalpha(0) << " " << additional.getalpha(1) << " " << additional.getalpha(2) << endl;
	}
 }
 
 bool TriangleCatalog::compareTriangle(Triangle t1, TriangleEntry t2, double thres){
	 vector<double> alphas1 = {t1.alphas[0], t1.alphas[1], t1.alphas[2]};
	 vector<double> alphas2 = {t2.getalpha(0), t2.getalpha(1), t2.getalpha(2)};
	 for(int j = 0; j<3; j++){
		 for(int i = 0; i<alphas2.size(); i++){
			 if(fabs(alphas1.at(j)) <= (fabs(alphas2.at(i))+thres) && alphas1.at(j) >= (fabs(alphas2.at(i))-thres)) alphas2.erase(alphas2.begin() + i);
		 }
	}
	if(alphas2.size() == 0){
		cout << "found Match" << endl;
		cout << t2.getalpha(0) << " " << t2.getalpha(1) << " " << t2.getalpha(2) << endl;
		return true;
	}
	return false;
 }
 
 bool TriangleCatalog::containsTriangle(Triangle t, double thres){
	 bool match = false;
	for(int i = 0; i<this->mainCatalog.size(); i++){
		if(compareTriangle(t, this->mainCatalog.at(i), thres)){
			cout << "MATCH IDs: " << this->mainCatalog.at(i).getID(0) << " " << this->mainCatalog.at(i).getID(1) << " " << this->mainCatalog.at(i).getID(2) << endl;
			match = true;
		}
	}
	//cout << "no match found" << endl;
	return match;
 }

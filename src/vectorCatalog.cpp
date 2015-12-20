
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

Triangle::Triangle(){
	
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

void Triangle::setDists(){
	dists[0] = B.getDistance(this->C);
	dists[1] = A.getDistance(this->B);
	dists[2] = A.getDistance(this->C);
}
 
 RID3::RID3(int id[3]){
	this->rID[0] = id[0];
	this->rID[1] = id[1];
	this->rID[2] = id[2];
}

RID3::RID3(int a, int b,int c){
	this->rID[0] = a;
	this->rID[1] = b;
	this->rID[2] = c;
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

double * StarEntry::getPositionVector(){
	
	double * vector = new double[3];
	vector[0]=this->x;
	vector[1]=this->y;
	vector[2]=this->z;
 	return vector;
}


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
 
  vector<StarEntry> * StarCatalog::getMainCatalogP(){
	  return &this->mainCatalog;
  }
 
 int StarCatalog::size(){
	 return this->mainCatalog.size();
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
		 if(dist <= d[1] && id != i){
			 d[1] = d[0];
			 d[0] = dist;
			 rid[2] = rid[1];
			 rid[1] = i;
		 }
	 }
	 RID3 re(rid);
	 return re;
 }
 
  //~ double StarCatalog::triangleGetBeta(RID3 t){
	 //~ Vector3D r(0,0,0);
	 //~ StarEntry s[3];
	 //~ s[0] = this->mainCatalog.at(t.rID[0]);
	 //~ s[1] = this->mainCatalog.at(t.rID[1]);
	 //~ s[2] = this->mainCatalog.at(t.rID[2]);
	 //~ Vector3D starVecs[3];
	 //~ starVecs[0] = Vector3D(s[0].getE(0), s[0].getE(1), s[0].getE(2));
	 //~ starVecs[1] = Vector3D(s[1].getE(0), s[1].getE(1), s[1].getE(2));
	 //~ starVecs[2] = Vector3D(s[2].getE(0), s[2].getE(1), s[2].getE(2));
	 //~ double beta = getAngleAt(starVecs[0], starVecs[1], starVecs[2]);
  //~ }
 
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
 
 void TriangleEntry::setDist(StarCatalog * c){
	 Vector3D r(0,0,0);
	 StarEntry s[3];
	 s[0] = c->getMainCatalogP()->at(this->rID[0]);
	 s[1] = c->getMainCatalogP()->at(this->rID[1]);
	 s[2] = c->getMainCatalogP()->at(this->rID[2]);
	 Vector3D starVecs[3];
	 starVecs[0] = Vector3D(s[0].getE(0), s[0].getE(1), s[0].getE(2));
	 starVecs[1] = Vector3D(s[1].getE(0), s[1].getE(1), s[1].getE(2));
	 starVecs[2] = Vector3D(s[2].getE(0), s[2].getE(1), s[2].getE(2));
	
	double dis[3];
	
	dis[1] = sqrt((starVecs[1].c[0]-starVecs[0].c[0])*(starVecs[1].c[0]-starVecs[0].c[0]) + (starVecs[1].c[1]-starVecs[0].c[1])*(starVecs[1].c[1]-starVecs[0].c[1]) + (starVecs[1].c[2]-starVecs[0].c[2])*(starVecs[1].c[2]-starVecs[0].c[2]));
	dis[2] = sqrt((starVecs[2].c[0]-starVecs[0].c[0])*(starVecs[2].c[0]-starVecs[0].c[0]) + (starVecs[2].c[1]-starVecs[0].c[1])*(starVecs[2].c[1]-starVecs[0].c[1]) + (starVecs[2].c[2]-starVecs[0].c[2])*(starVecs[2].c[2]-starVecs[0].c[2]));
	dis[0] = sqrt((starVecs[2].c[0]-starVecs[1].c[0])*(starVecs[2].c[0]-starVecs[1].c[0]) + (starVecs[2].c[1]-starVecs[1].c[1])*(starVecs[2].c[1]-starVecs[1].c[1]) + (starVecs[2].c[2]-starVecs[1].c[2])*(starVecs[2].c[2]-starVecs[1].c[2]));

	this->d[0] = dis[0];
	this->d[1] = dis[1];
	this->d[2] = dis[2];
}

 double TriangleEntry::getD(int i){
	 return this->d[i];
 }
 
 void TriangleCatalog::printSth(StarCatalog * c, double px, int a, int e, vector<TriangleEntry> * triangles){
	 for(int i = a; i<e; i++){
		 RID3 curr = c->findClosest2(i);
		 curr.rID[0] = i; //Hauptstern ist bei rID[0];
		 Vector3D alphas = c->triangleGetAlpha(curr);
		 TriangleEntry additional(curr, alphas);
		 additional.setDist(c);
		 triangles->push_back(additional);
	 }
 }
 
 void TriangleCatalog::createCatalog(StarCatalog & c, double pxAng){
	vector<TriangleEntry> triangles1;
	vector<TriangleEntry> triangles2;
	
	int start = 0;
	int half = c.size()/2;
	int end = c.size();
	
	thread tr1(&TriangleCatalog::printSth, this, &c, pxAng, start, half, &triangles1);
	thread tr2(&TriangleCatalog::printSth, this, &c, pxAng, half+1, end, &triangles2);
	
	tr1.join();
	tr2.join();
	
	this->mainCatalog.insert(this->mainCatalog.end(), triangles1.begin(), triangles1.end());
	this->mainCatalog.insert(this->mainCatalog.end(), triangles2.begin(), triangles2.end());

	//~ for(int i = 0; i<c.size(); i++){
		 //~ RID3 curr = c.findClosest2(i);
		 //~ curr.rID[0] = i; //Hauptstern ist bei rID[0];
		 //~ Vector3D alphas = c.triangleGetAlpha(curr);
		 //~ TriangleEntry additional(curr, alphas);
		 //~ additional.setDist(&c);
		 //~ this->mainCatalog.push_back(additional);
	//~ }
 }
 
 bool TriangleCatalog::compareTriangle(Triangle t1, TriangleEntry t2, double thres, Vector3D * solution){
	 vector<double> alphas1 = {t1.alphas[0], t1.alphas[1], t1.alphas[2]};
	 vector<double> alphas2 = {t2.getalpha(0), t2.getalpha(1), t2.getalpha(2)};
	 for(int j = 0; j<3; j++){
		 for(int i = 0; i<alphas2.size(); i++){
			 if(fabs(alphas1.at(j)) <= (fabs(alphas2.at(i))+thres) && alphas1.at(j) >= (fabs(alphas2.at(i))-thres)) alphas2.erase(alphas2.begin() + i);
		 }
	}
	
	double beta1 = acos(-0.5*(t1.dists[0]*t1.dists[0] - t1.dists[1]*t1.dists[1] - t1.dists[2]*t1.dists[2])/(t1.dists[1]*t1.dists[2]));
	double beta2 = acos(-0.5*(t2.getD(0)*t2.getD(0) - t2.getD(1)*t2.getD(1) - t2.getD(2)*t2.getD(2))/(t2.getD(1)*t2.getD(2)));
	
	if(alphas2.size() == 0 && (fabs(beta1) - fabs(beta2)) < 0.1){
		*solution = Vector3D(t2.getalpha(0),t2.getalpha(1),t2.getalpha(2));
		return true;
	}
	return false;
 }
 
 TriangleEntry TriangleCatalog::at(int i){
	 return this->mainCatalog.at(i);
 }
 
 bool TriangleCatalog::containsTriangle(Triangle t, double thres, Vector3D * solutionAngles, RID3 * solutionIDs, int * id){
	bool match = false;
	for(int i = 0; i<this->mainCatalog.size(); i++){
		if(compareTriangle(t, this->mainCatalog.at(i), thres, solutionAngles)){
			*solutionIDs = RID3(this->mainCatalog.at(i).getID(0), this->mainCatalog.at(i).getID(1), this->mainCatalog.at(i).getID(2));
			*id = i;
			match = true;
		}
	}
	return match;
 }

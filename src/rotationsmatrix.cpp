#include <math.h>
#include "rotationsmatrix.h"


void Rotation::getRotationmatrix(double v[3] , double w[3] ){
	double u[3] ={v[1]*w[2]-v[2]*w[1],v[2]*w[0]-v[0]*w[2],v[0]*w[1]-v[1]*w[0]};
	normalize(u);
	double deg=acos((v[0]*w[0]+v[1]*w[1]+v[2]*w[2])/(sqrt(v[0]+v[1]+v[2]) * sqrt(w[0]+w[1]+w[2])));
	
	double d=(1-cos(deg));
	
	vector<vector<double> > ro(3, vector<double>(3,0));
	ro.at(0).at(0)=(u[0]*u[0]*d+cos(deg));
	double r_10=u[1]*u[0]*d+u[2]*sin(deg);
	double r_20=u[2]*u[0]*d-u[1]*sin(deg);

	double r_01= u[0]*u[1]*d-u[2]*sin(deg);
	double r_11=u[1]*u[1]*d+cos(deg);
	double r_21=u[2]*u[1]*d-u[0]*sin(deg);

	double r_02=u[0]*u[2]*d+u[1]*sin(deg);
	double r_12=u[1]*u[2]*d-u[0]*sin(deg);
	double r_22=u[2]*u[2]*d+cos(deg);

	//double rot [3][3]={{r_00, r_01, r_02 },{r_10, r_11, r_12},{r_20, r_21, r_22}};
	this->rotor = ro;
}
quaternion Rotation::getQuaternion(double v[3], double w[3]){
	double u[3] ={v[1]*w[2]-v[2]*w[1],v[2]*w[0]-v[0]*w[2],v[0]*w[1]-v[1]*w[0]};
	double deg=acos((v[0]*w[0]+v[1]*w[1]+v[2]*w[2])/(sqrt(v[0]+v[1]+v[2]) * sqrt(w[0]+w[1]+w[2])));
	
	normalize(u);
	quaternion q;
	q.deg=cos(deg/2);
	q.x=sin(deg/2)*u[0];
	q.y=sin(deg/2)*u[1];
	q.z=sin(deg/2)*u[2];
	return q;
}
double* normalize(double u[3]){
	double u_betrag=sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
	u[0]=u[0]/u_betrag;
	u[1]=u[1]/u_betrag;
	u[2]=u[2]/u_betrag;
	return u;
}


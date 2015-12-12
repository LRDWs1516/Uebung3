#include <vector>
using namespace std;

double* normalize(double[3]);

class quaternion{
public:	
	double deg;
	double x;
	double y;
	double z;
};




class Rotation{

public:
	void getRotationmatrix(double* , double* );
	vector<vector<double> >  rotor;
	quaternion getQuaternion(double);
	void printRotor();
	void printQuat(quaternion q);
private:
	double* RotateX(double , double[3]);
	double* RotateY(double , double[3]);
	double* RotateZ(double , double [3]);
	double* rotate(double*, double[3]);
	
	quaternion getQuaternion(double[3] , double[3] );
};

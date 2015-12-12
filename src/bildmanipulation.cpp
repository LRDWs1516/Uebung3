//Uebung 2 LRD Beispiel
//This Program has a problem: X is vertical, Y is horizontal

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

//all take open ifstreams
//doesn't hurt atm
int getW(ifstream & , int);
int getH(ifstream & , int);
int getStart(ifstream & );
int getFilesize(ifstream &);

class Point2D{
public:
	double x;
	double y;
	Point2D();
	Point2D(double, double);
};

Point2D::Point2D(){
	this->x = 0;
	this->y = 0;
}

Point2D::Point2D(double x, double y){
	this->x = x;
	this->y = y;
}

class Color{
public:
	unsigned char value[3];
	Color(int, int, int);
	int absolute();
};

Color::Color(int a, int b, int c){
	value[0] = a;
    value[1] = b;
	value[2] = c;
}

int Color::absolute(){
	return (int)(value[0]+value[1]+value[2]);
}

//basically implements all functionality
class Image{
public:
	Image();
	vector<vector<Color> > Matrix;
	vector<vector<Color> > temporaryMat;
	vector<Point2D> fitArray;
	Point2D centerPoint;
	uint32_t w,h,start,filesize;
	char * overhead;
	bool readImageFromFile(const char *, int);
	bool readImageFromFile2(const char *, int);
	bool writeImageToFile(const char *);
	void thresholdImage(uint8_t thres);
	Point2D calcCenterOfMass();
	void getPointArray();
	void circleFit(vector<Point2D>);
	void detectEdges(int, int);
	void drawRectangle(Point2D, int, Color, int);
	void drawCross(Point2D, int, Color, int);
	void addImage(Image);
	void getCameraParams();
	void drawArrayToImage(vector<Point2D> &, Color);
	void drawBinarysToImage(vector<vector<bool> > &, Color);
	void copyTo(Image &, int);
private:
};

//Class used to flood edges and determine connected Points in an image
class PointArray{
public:
	void fill(Image &, int);
	void fill2(Image &, int);
	bool getCentrals(int);
	vector<Point2D> getAllPoints();
	vector<Point2D> allPoints;
	vector<Point2D> startPoints;
	vector<Point2D> centralPoints;
	vector<vector<Point2D> > allObjects;
	vector<vector<bool> > overlay;
private:
	int vSize;
	vector<Point2D> checkSurrounding(Point2D &, Image &);
	vector<Point2D> followOutline(Point2D &, Image &);
	vector<Point2D> calcOverlay(Point2D &, Image &, int);
	Point2D getCenter(vector<Point2D>);
	bool isOutline(Point2D &, Image &);
};

//standard Constructor
Image::Image(){
	w = 0; h = 0; filesize = 0; overhead = ""; start = 0;
}

void Image::copyTo(Image & destination, int type = 0){
	try{
		if(type != 0 && type != 1) throw type;
	destination.w = this->w;
	destination.h = this->h;
	if(type == 0){
		vector<vector<Color> > temp(this->h, vector<Color>(this->w, Color(0,0,0)));
		destination.Matrix = temp;
		temp.clear();}
	else{
		destination.Matrix = this->Matrix;
	}
    destination.start = this->start;
    destination.overhead = this->overhead;
    destination.temporaryMat = this->temporaryMat;
	}catch(int e){
		cout << "Error: " << e << " is not a copy type" << endl;
	}
}

//reads Windows Bitmap file, stores pixel values in a vector matrix, and overhead in string
//won't read anything but Bitmaps of Windows standard
bool Image::readImageFromFile(const char * fname, int type){
	try{
	if(type != 1 && type != 0) throw type;

	ifstream image_in;
	image_in.open(fname, ios::binary);

    w = getW(image_in, type);
    h = getH(image_in, type);
    start = getStart(image_in);
    filesize = getFilesize(image_in);
    overhead = new char[start-1];

    cout << "W: " << w << " H: " << h << " start: " << start << " Fsize: " << filesize << endl;
    //printf("%d,%d,%d,%d\n",w,h,start,filesize);
    uint32_t cnt = 0;
    image_in.seekg(0);
    while(cnt < start){
    	cnt++;
    	overhead[cnt] = image_in.get();
    }

	vector<vector<Color> > temp(h, vector<Color>(w, Color(0,0,0)));
	this->Matrix = temp;
	this->temporaryMat = temp;
	temp.clear();
	image_in.seekg(start);
	for(uint32_t x = 0; x<h; x++){
		for(uint32_t y = 0; y<w; y++){
			for(int px = 0; px < 3; px++){
				Matrix.at(x).at(y).value[px] = image_in.get();
			}
		}
	}
    image_in.close();

	}catch(int e){
    		cout << "Exception: wrong image type: " << e << endl;
    	}
	return true;
}

//writes the header and pixeldata to a .bmp file
//will reuse the header of the old image
//to change header properties "set" functions for the values in char * overhead are necessary
//this Method will overwrite other files, so be careful with the names of your .bmps
bool Image::writeImageToFile(const char * fname){
	ofstream image_out;
	image_out.open(fname, ios::binary | ios::trunc);
    uint32_t cnt = 0;
    while(cnt < start){
    	cnt++;
    	image_out.put(overhead[cnt]);
    }

	for(uint32_t x = 0; x<h; x++){
		for(uint32_t y = 0; y<w; y++){
			for(int px = 0; px < 3; px++){
				image_out.put(Matrix.at(x).at(y).value[px]);
			}
		}
	}
	return true;
}

//simple threshold Operation
//will just do the job
void Image::thresholdImage(uint8_t thres){
		for(uint32_t x = 0; x<h; x++){
			for(uint32_t y = 0; y<w; y++){
				uint32_t grayValue = 0;
				for(int px = 0; px < 3; px++){
					grayValue += Matrix.at(x).at(y).value[px];
				}
				if(grayValue <= thres){
					for(int px = 0; px < 3; px++){
						Matrix.at(x).at(y).value[px] = 0;
					}
				}
				else{
					for(int px = 0; px < 3; px++){
						Matrix.at(x).at(y).value[px] = 255;
					}
				}
			}
		}
}

/*
 * Uses (n x n) Laplacian Matrix operation on every Pixel of the image the matrix fits on
 * is slow but creates reliable results as long as the Matrix isn't all too big
 * changes working Matrix AND temporary Matrix
 * their content will be identical
 * side 0: Inside
 * side 1: Outside
 * oder: both
 */
void Image::detectEdges(int lapSize=3, int side=2){
	try{
		if(lapSize%2==0)throw lapSize;
		int lapRange = lapSize/2;
		int mult = lapSize * lapSize - 1;
		//Iterate through image
		for(int x = 0; x<h; x++){
			for(int y = 0; y<w; y++){
				//Iterate through rgb
				for(int px = 0; px < 3; px++){
					//Iterate through nxn Matrix to perform Laplace-edge-detection
					//To get results the matrix must have odd dimensions
						int edgeVal = 0;
						for(int mx = -lapRange; mx <= lapRange; mx++){
							for(int my = -lapRange; my <= lapRange; my++){
								if(x-mx >= 0 && x+mx < h && y-my >= 0 && y+my < w){
									if(my == 0 && mx == 0){
										edgeVal += (mult * (int)Matrix.at(x).at(y).value[px]);
									}
									else{
										edgeVal -= Matrix.at(x+mx).at(y+my).value[px];
									}
								}
								else{//black if out
									edgeVal += 0;
								}
							}
						}
						if(side == 1){
							if(edgeVal > 0) edgeVal = 0; //dark Side
						}
						if (side == 0){
							if(edgeVal < 0) edgeVal = 0; //bright Side
						}
						edgeVal = abs(edgeVal);
						edgeVal /= mult;
						temporaryMat.at(x).at(y).value[px] = edgeVal; //save into temporary Matrix
				}
			}
		}
		Matrix = temporaryMat; //copy temporary to working Matrix
	}catch (int e){
		cout << "Exception: Laplacian Operator must be of odd dimension. \"" << e << "\" is not odd.\n";
	}
}

//The name says it all
//The more you can trust the pixels and the image quality the better the result will be
//is faster but a LOT less reliable than the circleFit function
//depends heavily on your threshold function and values
Point2D Image::calcCenterOfMass(){
	vector<vector<Color> > temp(h, vector<Color>(w, Color(0,0,0)));
	this->temporaryMat = temp;
	temp.clear();

	Point2D centerOfMass(0,0);
	int numPoints = 0;
	for(uint32_t x = 0; x<h; x++){
		for(uint32_t y = 0; y<w; y++){
			if(Matrix.at(x).at(y).value[0] >= 230)
			{
				numPoints++;
				centerOfMass.x = (centerOfMass.x * (numPoints-1) + (double)x+.5)/(double)numPoints;
				centerOfMass.y = (centerOfMass.y * (numPoints-1) + (double)y+.5)/(double)numPoints;
			}
		}
	}

	return centerOfMass;
}

//Puts Outline into vector.
//Also adds an additional threshold (not that it's needed, but it's going to stay)
//Could be done by flooding the Edge. This is not implemented as very narrow edges may be interrupted for >=1 Pixels.
void Image::getPointArray(){
	vector<Point2D> Points;
	for(int x = 0; x<h; x++){
		for(int y = 0; y<w; y++){
			if(Points.size() == 0 || Matrix.at(x).at(y).value[0] >= 230)
			{
				Points.push_back(Point2D(x,y));
			}
		}
	}
	fitArray = Points;
}

//A lot of loops create a lot of readability?
void Image::circleFit(vector<Point2D> basePoints){
	double x_ = 0, y_ = 0;
	vector<Point2D> transPoints;
	//get CoM
	for(int i = 0; i < basePoints.size(); i++){
		x_ = (x_ * i + basePoints.at(i).x)/ (double)(i+1);
		y_ = (y_ * i + basePoints.at(i).y)/ (double)(i+1);
	}
	//Get shifted Points
	for(int i = 0; i < basePoints.size(); i++){
		transPoints.push_back(Point2D(basePoints.at(i).x - x_, basePoints.at(i).y - y_));
	}
	//Get the Suvuvuvuvuvs
	double Suu, Suv, Suuu, Suvv, Svv, Svvv, Svuu;
	Suu = Suv = Suuu = Suvv = Svv = Svvv = Svuu = 0;
	for(int i = 0; i < basePoints.size(); i++){
		Suu += transPoints.at(i).x+.5 * transPoints.at(i).x+.5;
		Svv += transPoints.at(i).y+.5 * transPoints.at(i).y+.5;
		Suv += transPoints.at(i).x+.5 * transPoints.at(i).y+.5;
		Suuu += transPoints.at(i).x+.5 * transPoints.at(i).x+.5 * transPoints.at(i).x+.5;
		Svvv += transPoints.at(i).y+.5 * transPoints.at(i).y+.5 * transPoints.at(i).y+.5;
		Suvv += transPoints.at(i).x+.5 * transPoints.at(i).y+.5 * transPoints.at(i).y+.5;
		Svuu += transPoints.at(i).y+.5 * transPoints.at(i).x+.5 * transPoints.at(i).x+.5;
	}
	//Calculate center of circle
	double vc = 0, uc = 0;
	vc = 0.5 * (Svvv+Svuu-Suuu-Suvv);
	vc /= (Svv -Suv);
	uc = 0.5 * (Suuu+Suvv);
	uc -= (vc * Suv);
	uc /= Suu;

	vc += y_;
	uc += x_;

	centerPoint = Point2D(uc, vc);
}

//Quickly draw a Rectangle into the image
//used to take forever
//now it's fixed
void Image::drawRectangle(Point2D position, int size, Color col, int type = 0){
	unsigned char * color = col.value;
	for(int x = position.x-size; x < position.x+size; x++){
		for(int y = position.y-size; y < position.y+size; y++){
				if(x>=0 && x<h && y>=0 && y<w){
					if(type == 0){
						Matrix.at(x).at(y).value[0] = color[0];
						Matrix.at(x).at(y).value[1] = color[1];
						Matrix.at(x).at(y).value[2] = color[2];
					}
					else{
						Matrix.at(x).at(y).value[0] = .5 * Matrix.at(x).at(y).value[0]+ .5 * color[0];
						Matrix.at(x).at(y).value[1] = .5 * Matrix.at(x).at(y).value[1]+ .5 * color[1];
						Matrix.at(x).at(y).value[2] = .5 * Matrix.at(x).at(y).value[2]+ .5 * color[2];
					}
				}
		}
	}
}

//Basically draws two long Rectangles
void Image::drawCross(Point2D position, int size, Color col, int type = 0){
	unsigned char * color = col.value;
	for(int x = 0; x<h; x++){
		for(int y = position.y-size; y<=position.y+size; y++){
				if(y>=0 && y<w){
					if(type == 0){
						Matrix.at(x).at(y).value[0] = color[0];
						Matrix.at(x).at(y).value[1] = color[1];
						Matrix.at(x).at(y).value[2] = color[2];
					}
					else{
						Matrix.at(x).at(y).value[0] = .5 * Matrix.at(x).at(y).value[0]+ .5 * color[0];
						Matrix.at(x).at(y).value[1] = .5 * Matrix.at(x).at(y).value[1]+ .5 * color[1];
						Matrix.at(x).at(y).value[2] = .5 * Matrix.at(x).at(y).value[2]+ .5 * color[2];
					}
				}
		}
	}

	for(int x = position.x-size; x<=position.x+size; x++){
		for(int y = 0; y<w; y++){
				if(x>=0 && x<h){
					if(type == 0){
						Matrix.at(x).at(y).value[0] = color[0];
						Matrix.at(x).at(y).value[1] = color[1];
						Matrix.at(x).at(y).value[2] = color[2];
					}
					else{
						Matrix.at(x).at(y).value[0] = .5 * Matrix.at(x).at(y).value[0]+ .5 * color[0];
						Matrix.at(x).at(y).value[1] = .5 * Matrix.at(x).at(y).value[1]+ .5 * color[1];
						Matrix.at(x).at(y).value[2] = .5 * Matrix.at(x).at(y).value[2]+ .5 * color[2];
					}
				}
		}
	}
}

//Adds image values together into Image object matrix
//Won't create overflows but also doesn't scale down to avoid saturation
void Image::addImage(Image second){
	for(uint32_t x = 0; x<h; x++){
		for(uint32_t y = 0; y<w; y++){
				double g,b,r;
				b = Matrix.at(x).at(y).value[0];
				b +=second.Matrix.at(x).at(y).value[0];
				if(b <= 255) Matrix.at(x).at(y).value[0] = b; else Matrix.at(x).at(y).value[0] = 255;

				g = Matrix.at(x).at(y).value[1];
				g +=second.Matrix.at(x).at(y).value[1];
				if(g <= 255) Matrix.at(x).at(y).value[1] = g; else Matrix.at(x).at(y).value[1] = 255;

				r = Matrix.at(x).at(y).value[2];
				r +=second.Matrix.at(x).at(y).value[2];
				if(r <= 255) Matrix.at(x).at(y).value[2] = r; else Matrix.at(x).at(y).value[2] = 255;
		}
	}
}

void Image::drawArrayToImage(vector<Point2D> & outline, Color fill){
	//Color fill(255,255,0);
	for(int i = 0; i<outline.size(); i++){
		Matrix.at((int)outline.at(i).x).at((int)outline.at(i).y) = fill;
	}
}

void Image::drawBinarysToImage(vector<vector<bool> > & binIm, Color col){
	//Color fill(255,255,0);
	for(uint32_t x = 0; x<h; x++){
		for(uint32_t y = 0; y<w; y++){
			if(binIm.at(x).at(y)) Matrix.at(x).at(y) = col;
		}
	}
}

vector<Point2D> PointArray::checkSurrounding(Point2D & current, Image & input){
	int w = input.w;
	int h = input.h;
    vector<Point2D> output;
	vector<Point2D> checkpoints;
	if(current.x-1 	> 0 	&& current.y 	> 0){	checkpoints.push_back(Point2D(current.x-1, current.y));}
	if(current.x-1 	> 0 	&& current.y+1 	< w-1){	checkpoints.push_back(Point2D(current.x-1, current.y+1));}
    if(current.x 	> 0 	&& current.y+1 	< w-1){	checkpoints.push_back(Point2D(current.x, current.y+1));}
    if(current.x+1 	< h-1 	&& current.y+1 	< w-1){	checkpoints.push_back(Point2D(current.x+1, current.y+1));}
    if(current.x+1 	< h-1 	&& current.y 	< w-1){	checkpoints.push_back(Point2D(current.x+1, current.y));}
    if(current.x+1 	< h-1 	&& current.y-1 	> 0){	checkpoints.push_back(Point2D(current.x+1, current.y-1));}
    if(current.x 	< h-1 	&& current.y-1 	> 0){	checkpoints.push_back(Point2D(current.x, current.y-1));}
    if(current.x-1 	> 0 	&& current.y-1 	> 0){	checkpoints.push_back(Point2D(current.x-1, current.y-1));}

		for(int i = 0; i<checkpoints.size(); i++){
			if(isOutline(checkpoints.at(i), input) && !overlay.at(checkpoints.at(i).x).at(checkpoints.at(i).y))
			{
				output.push_back(checkpoints.at(i));
				overlay.at(checkpoints.at(i).x).at(checkpoints.at(i).y) = true;
			}
		}
		checkpoints.clear();
	return output;
}

vector<Point2D> PointArray::followOutline(Point2D & current, Image & input){
	int w = input.w;
	int h = input.h;
    vector<Point2D> output;
    bool isBorder = false;
    for(int mx = -1; mx < 2; mx++){
    	for(int my = -1; my < 2; my++){
    		Point2D checkpoint(current.x+mx, current.y+my);
    		if(current.x+mx >= 0 && current.x+mx < h && current.y+my >= 0 && current.y+my < w){
				if(isOutline(checkpoint, input) && !overlay.at(checkpoint.x).at(checkpoint.y))
				{
					output.push_back(checkpoint);
				}
				else if(!isOutline(checkpoint, input)){
					isBorder = true;
				}
    		}
    		else isBorder = true;
    	}
    }
	if(isBorder){
		for(int i = 0; i<output.size(); i++) {
			overlay.at(output.at(i).x).at(output.at(i).y) = true;
		}
	}
	else output.clear();

	return output;
}

vector<Point2D> PointArray::calcOverlay(Point2D & startpoint, Image & input, int type){
	//cout << "called" << endl;
	vector<Point2D> currentObject;
	try{
		if(type != 1 && type != 2) throw type;
		allPoints.push_back(startpoint);
		startPoints.push_back(startpoint);
		currentObject.push_back(startpoint);
		overlay.at(startpoint.x).at(startpoint.y) = true;
		input.Matrix.at(startpoint.x).at(startpoint.y) = Color(0,255,0);
		vector<Point2D> stage;
		vector<Point2D> last_stage;
		last_stage.push_back(startpoint);
		while(!last_stage.empty()){
			for(int j = 0; j < last_stage.size(); j++){
				vector<Point2D> append;
				if(type == 1)append = checkSurrounding(last_stage.at(j), input);
				else if(type == 2) append = followOutline(last_stage.at(j), input);
				stage.insert(stage.end(), append.begin(), append.end());
			}
			last_stage.clear();
			if(!stage.empty()){
				last_stage.insert(last_stage.end(), stage.begin(), stage.end());
				currentObject.insert(currentObject.end(), stage.begin(), stage.end());
			}
			stage.clear();
		}
	}catch (int e){
		cout << "Error: " << e << " is not an Overlay Type" << endl;
	}
	allPoints.insert(allPoints.end(), currentObject.begin(), currentObject.end());
	return currentObject;
}

bool PointArray::isOutline(Point2D & p, Image & input){
	if(input.Matrix.at(p.x).at(p.y).value[0] != 0 || input.Matrix.at(p.x).at(p.y).value[1] != 0 || input.Matrix.at(p.x).at(p.y).value[2] != 0) return true;
	return false;
}

vector<Point2D> PointArray::getAllPoints(){
	return allPoints;
}

/*
 * takes type 1 and type 2
 * 1: full, 2: outline
*/
void PointArray::fill(Image & input, int type)
{
	vSize = 0;
	vector<vector<bool> > temp(input.h, vector<bool>(input.w, false));
	overlay = temp;
	temp.clear();
	bool done = false;
	for(int x = 0; x<input.h; x++){
		for(int y= 0; y<input.w; y++){
			if(input.Matrix.at(x).at(y).value[0] != 0 && !done && !overlay.at(x).at(y)){
				Point2D startPoint(x,y);
				allObjects.push_back(calcOverlay(startPoint, input, type));
				done = true;
			}
			if(done){
				break;
			}
		}
		if(done){
			break;
		}
	}
}

void PointArray::fill2(Image & input, int type)
{
	vSize = 0;
	vector<vector<bool> > temp(input.h, vector<bool>(input.w, false));
	overlay = temp;
	temp.clear();
	for(int x = 0; x<input.h; x++){
		for(int y= 0; y<input.w; y++){
			if(input.Matrix.at(x).at(y).value[0] != 0 && !overlay.at(x).at(y)){
				Point2D startPoint(x,y);
				allObjects.push_back(calcOverlay(startPoint, input, type));
			}
		}
	}
}

Point2D PointArray::getCenter(vector<Point2D> object){
	Point2D center;
	int obsize = object.size();
	for(int i = 0; i<obsize; i++){
		center.x += object.at(i).x/((double)(obsize));
		center.y += object.at(i).y/((double)(obsize));
	}
	return center;
}

bool PointArray::getCentrals(int type){
	try{
		if(type != 0) throw type;
		for(int i = 0; i<this->allObjects.size(); i++){
			this->centralPoints.push_back(getCenter(allObjects.at(i)));
		}
		return true;
	}catch(int e){
		cout << "wrong type: " << e << endl;
		terminate();
	}
	return false;
}

//Get's image width from Windows BITMAPINFOHEADER
int getW(ifstream & stream, int type){
	stream.seekg(17);
	if(type != 0)	stream.seekg(18);
    unsigned int w = stream.get();
    if(type == 0){
    	w = w << 8;
    	w = w | stream.get();
		w = w << 8;
		w = w | stream.get();
		w = w << 8;
		w = w | stream.get();
    }
    else{
    	w = w | (uint32_t)(stream.get() << 8);
    }
    return w;
}

//Get's image heigth from Windows BITMAPINFOHEADER
int getH(ifstream & stream, int type){
	stream.seekg(21);
	if(type != 0)	stream.seekg(22);
	unsigned int h = stream.get();
    if(type == 0){
    	h = h << 8;
    	h = h | stream.get();
		h = h << 8;
		h = h | stream.get();
		h = h << 8;
		h = h | stream.get();
    }
    else{
    	h = h | (uint32_t)(stream.get() << 8);
    }
    return h;
}

//Get's start of pixeldata from Windows BITMAPINFOHEADER
int getStart(ifstream & stream){
	stream.seekg(10);
    uint32_t start = stream.get();
    return start;
}

//calculates size of pixeldata (could be faulty)
int getFilesize(ifstream & stream){
	stream.seekg(0,stream.end);
    uint32_t filesize=stream.tellg();
    return filesize;
}

void sleepcp(int milliseconds) // cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}

double * getfov(double f, double px, Image im){
	double a_v[2]; a_v[0] = 0; a_v[1] = 0;
 	a_v[0]=2.0*atan((im.w*px*0.000001)/(2.0*f));
 	a_v[1]=2.0*atan((im.h*px*0.000001)/(2.0*f));
	return a_v;
}

/*
 * Image.cpp
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#include "Image.h"

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

    cout << "-W: " << w << " H: " << h << " start: " << start << " Fsize: " << filesize << endl;
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

//Get's image width from Windows BITMAPINFOHEADER
int Image::getW(ifstream & stream, int type){
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
int Image::getH(ifstream & stream, int type){
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
int Image::getStart(ifstream & stream){
	stream.seekg(10);
    uint32_t start = stream.get();
    return start;
}

//calculates size of pixeldata (could be faulty)
int Image::getFilesize(ifstream & stream){
	stream.seekg(0,stream.end);
    uint32_t filesize=stream.tellg();
    return filesize;
}




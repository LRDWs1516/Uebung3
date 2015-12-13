/*
 * PointArray.cpp
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#include "PointArray.h"
#include <vector>

using namespace std;

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

void PointArray::getCentrals(){
	for(int i = 0; i<this->allObjects.size(); i++){
		this->centralPoints.push_back(getCenter(allObjects.at(i)));
	}
}

Point2D PointArray::getCenterof(int i){
	return this->centralPoints.at(i);
}

int PointArray::findClosest(Point2D center, double thres){
	int resultID = 0;
	double minD1 = 900;
	double minD2 = 900;
	for(int i = 0; i<this->centralPoints.size(); i++){
		double d = center.getDistance(this->centralPoints.at(i));
		if(d < minD1 && d >= 1 && d > thres){
			minD1 = d;
			resultID = i;
		}
	}
	return resultID;
}

vector<Point2D> PointArray::getCentralTriangle(Image & im){
	vector<Point2D> retVec;

    int central = this->findClosest(Point2D(im.h/2, im.w/2),0);
    int dist1 = this->findClosest(this->getCenterof(central),0);
    Point2D a = this->centralPoints.at(central);
    Point2D b = this->centralPoints.at(dist1);
    double tr = sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    cout << "Dist: " << tr << endl;
    int dist2 = this->findClosest(this->getCenterof(central),tr);
    Point2D c = this->centralPoints.at(dist2);
    retVec.push_back(a);
    retVec.push_back(b);
    retVec.push_back(c);
	return retVec;
}



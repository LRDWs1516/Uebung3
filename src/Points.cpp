/*
 * Points.cpp
 *
 *  Created on: Dec 13, 2015
 *      Author: david
 */

#include "Points.h"

Point2D::Point2D(){
	this->x = 0;
	this->y = 0;
}

Point2D::Point2D(double x, double y){
	this->x = x;
	this->y = y;
}

double Point2D::getDistance(Point2D second){
	return sqrt((this->x-second.x)*(this->x-second.x) + (this->y-second.y)*(this->y-second.y));
}

void Point2D::printVal(){
	cout << "x:" << this->x << " y:" << this->y;
}

double Point2D::getAngleTo(Point2D second, double app){
	return (this->getDistance(second) * app);
}

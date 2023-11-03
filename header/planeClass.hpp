#ifndef planeclass
#define planeclass
#include <cmath>
#include <iostream>
#include <vector>
#include "otherClass.hpp"
#include "bulletClass.hpp"

using std::vector;

class plane:public globalBullets{
	public:
		// general variables
		double px;
		double py;
		double rotation;
		double curSpeed;
		double curTurn;
		double downForce;
		double acc;
		double rdiff;
		double altForCal;
		int counter;
		int ammo;
		int hitBoxX;
		int hitBoxY;
		int knockbackX;
		int knockbackY;
		int knockbackMul;
		vector<gun> allGuns;
		aeroframe body = aeroframe();
		engine eng = engine();

		void printPlane();
		// constructors
		plane(int x,int y,aeroframe aero,engine eng,vector<gun> gunz);
		plane();
		// functions
		void checkCollision();
		void normalR(); //normalises the angle of the plane to -pi to pi   
		void updatePlane(double r,vector<bool> keys); // main update function being called every game tick
};
#endif

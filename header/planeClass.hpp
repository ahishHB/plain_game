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
		double r;
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
		int state;
		//const char stateLable[3][15]  = {"idle","pursue","defensive"};
		vector<gun> allGuns;
		aeroframe body = aeroframe();
		engine eng = engine();

		void printPlane();
		// constructors
		plane(int x,int y,aeroframe aero,engine eng,vector<gun> gunz,double rot=0.0);
		plane();
		// functions
		void checkCollision();
		void normalR(); //normalises the angle of the plane to -pi to pi   
		void updatePlane(double rotationTo,vector<bool> keys); // main update function being called every game tick
													  // if r is greater than m_pi then consider it as a no manuever state
													  // hence dont change the heading
};
#endif

#include "planeClass.hpp"
#include "bulletClass.hpp"
#include "otherClass.hpp"
#include <alloca.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <math.h>
#include <ostream>
#include <vector>

// checks collisions between bullets and planes
void plane::checkCollision(){
	for(int i=0;i<allBullets.size();i=i+1){
		if((allBullets[i][0]>px-20&&allBullets[i][0]<px+20)&&(allBullets[i][1]>py-20&&allBullets[i][1]<py+20)){		//within the  hit box
			//body.aeroCurHp = body.aeroCurHp - allBullets[i][4];	//dmg 
			//px = px + (allBullets[i][4]*(allBullets[i][2]*0.001));//knockback
			//py = py + (allBullets[i][4]*(allBullets[i][3]*0.001));//knockback
			knockbackX  = knockbackX + (allBullets[i][2])*knockbackMul;		
			knockbackY = knockbackY + (allBullets[i][3])*knockbackMul;
			knockbackMul = knockbackMul+2.0*allBullets[i][4];	
			allBullets.erase(allBullets.begin()+i);	//despawns the bullet
			i=i-1;
		}
	}
}
void plane::printPlane(){
	std::cout<<"speed: "<<curSpeed<<"    alt: "<<altForCal<<"    engine: "<<eng.cal(altForCal,curSpeed)<<"    gravityEffect: "<<body.calGrav(altForCal)<<"   drag: "<<body.calDrag(altForCal,curSpeed,rdiff);
}
plane::plane(){}

plane::plane(int x,int y,aeroframe aero,engine eng,vector<gun> gunz){
	px = x;
	py = y;
	downForce = 0.0;
	//curSpeed = maxSpeed/2;
	curSpeed = 15;
	rotation = 0;
	curTurn = 0.08;  // 5 degrees per tick 
	body = aero;
	this->eng = eng;
	allGuns = gunz;
	rdiff = 0;
	altForCal = 0;
	counter = 0;
	for(int i=0;i<allGuns.size();i=i+1){
		ammo = allGuns[i].ammo +ammo;

	}
	hitBoxX = 35 + (eng.enginePow/5-2)*5;
	hitBoxY = hitBoxX/2;
	knockbackMul =1;
	knockbackX =0;
	knockbackY =0;
}

void plane::normalR(){
	if(rotation > M_PI){
		rotation = (rotation - M_PI) - M_PI;
	}
	else if(rotation <= - M_PI){
		rotation = (rotation + M_PI) + M_PI;
	}
}

void plane::updatePlane(double r,vector<bool> keys){
	if(counter>=120){
		counter =0;
	}
	counter = counter +1;
	checkCollision();
	// shooting handler
	
	if(keys[6]){
		for(int i=0;i<allGuns.size();i=i+1){
			if(counter%allGuns[i].rateOfFire==0&&allGuns[i].ammo||allGuns[i].firstShot){
				allBullets.push_back({px+(hitBoxX+5)*cos(rotation),py+(hitBoxX+5)*sin(rotation),cos(rotation)*(allGuns[i].velocity+curSpeed)+((rand()%2)-1),sin(rotation)*(allGuns[i].velocity+curSpeed)+((rand()%2)-1),static_cast<double>(allGuns[i].dmg)});
				ammo = ammo -1;
				allGuns[i].ammo = allGuns[i].ammo-1;
				allGuns[i].firstShot = false;
				//ammo -1;
			}
		}
		//globalBullets::allBullets;	
	}
	else{
		for(int i=0;i<allGuns.size();i=i+1){
			if(counter%allGuns[i].rateOfFire==0&&allGuns[i].ammo){
				allGuns[i].firstShot = true;
			}
		}
	}
	


	//		###  start plane position  ###
	if(px> 15424){
		rotation = M_PI;
		px = 15424;
	}
	else if(px < 965){
		rotation = 0;
		px = 965;
	}
	if(py>10383){
		//rotation = - rotation;
		py = 10383;
	}
	else if(py<545){
		rotation = - rotation; 
		py = 545;
	}
	rdiff = r - rotation;
	if(((rdiff < curTurn)&&(rdiff > -curTurn))||((rdiff > (M_PI*2 - curTurn))||(rdiff < -(M_PI*2 - curTurn)))){
		rotation = r;
	}
	else{	
		if(std::abs(rdiff)< M_PI){
			if(rdiff > 0){
				rotation = rotation + curTurn;
			}
			else {
				rotation = rotation - curTurn;
			}
		}
		else{
			if(rdiff < 0){
				rotation = rotation + curTurn;
			}	
			else{
				rotation = rotation - curTurn;
			}
		}
	}
	if(py==10383){
		if(rotation>0&&rotation<M_PI/2){
			rotation =0;
		}
		else if(rotation>M_PI/2&&rotation<M_PI){
			rotation = M_PI;
		}
		
		if(curSpeed<1.5){
			if(rotation>-M_PI/2&&rotation<0){
				rotation = 0;
			}	
			else if(rotation<-M_PI/2&&rotation>-M_PI){
				rotation = -M_PI;
			}
		}
	}
	normalR();	

	// START ENGINE CONRTOL //
	if(keys[0]&&!keys[1]&&(eng.enginePerc+0.1<1.0)&&counter%10==0){
		eng.enginePerc = eng.enginePerc+0.1;
		if(eng.enginePerc == 0.10001){
			eng.enginePerc = 0.1;
		}		
	}
	else if(!keys[0]&&keys[1]&&(eng.enginePerc-0.1>0)&&counter%10==0){
		eng.enginePerc = eng.enginePerc -0.1;
		if(eng.enginePerc<0.1){
			eng.enginePerc = 0.00001;
		}
	}
	// END OF ENGINE CONTROL
	altForCal = 10383 - py;
	//altForCal = (altForCal<500)?500:altForCal;
	if(altForCal<=100){	// not landing state	was 2.0
		//acc = 0.01*eng.enginePerc;
		acc = 2.0*eng.cal(altForCal,curSpeed) - (body.calDrag(altForCal,curSpeed,(std::abs(rdiff)>curTurn)?curTurn:std::abs(rdiff))) + (body.calGrav(altForCal)*(std::sin(rotation)))/1.0;
		acc = (acc>0.1)?0.1:acc;
		acc = (acc<-0.1)?-0.1:acc;
		acc = (acc!=0)?(acc<0.003?0.007*eng.enginePerc-0.005:acc):0;
		//std::cout<<py<<" "<<eng.enginePerc<<std::endl;
	}
	else{
		//acc = 2.1*eng.cal(altForCal,curSpeed) - 1.7*(body.calDrag(altForCal,curSpeed,(std::abs(rdiff)>curTurn)?curTurn:std::abs(rdiff))) + (body.calGrav(altForCal)*(body.aeroWeight/((eng.engineCurHp/eng.engineMaxHp)*eng.enginePerc))*(std::sin(rotation)))/4.0;

		acc = 1.0*eng.cal(altForCal,curSpeed) - 1.0*(body.calDrag(altForCal,curSpeed,(std::abs(rdiff)>curTurn)?curTurn:std::abs(rdiff))) + (body.calGrav(altForCal)*(std::abs(std::sin(rotation))*(std::sin(rotation))))/0.25;
		acc = (acc<-0.1)?-0.1:acc;
		acc = (acc>0.1)?0.1:acc;
	}

	curSpeed = curSpeed + acc/2.0;
	//
	//	DO THIS !! make sure the plane cant point down when on the ground // 
	//
	//std::cout<<"######\n\n";
	//SPEED PROTECTION

	if(curSpeed<=1.5&&downForce<5-1/60.0&&py<10380){
		downForce = downForce +1.0/60.0;
	}
	//else if (curSpeed<=3.0&&downForce<7-1/60.0&&py<10080) {
	//	downForce = downForce + 1.0/60.0;
	//}
	else if(curSpeed>3.0&&downForce>1.0/60.0){
		downForce = downForce -1/60.0;
		curSpeed = curSpeed+1/60.0;
	}

	curSpeed = (curSpeed <0.0)?0.0:curSpeed;
	curSpeed = (curSpeed >15.0)?15.0:curSpeed;
	curTurn = 2.0*body.calTurn(curSpeed);

	downForce = (downForce<1/120.0)?0.0:downForce;
	//std::cout<<curSpeed<<" turn: "<<curTurn<<std::endl;
	// found out downForce can vary from 0-5 to the y component 
	// based on the speed u can calculate 0-15 speed 0-5 y component 0-5 should have the most impact 5-10 midium and 10-15 least affect

	//std::cout<<"rotation:"<<rotation<<" "<<altForCal<<" acc:"<<acc<<" cur:"<<curSpeed<<std::endl;
	py = py + sin(rotation)*curSpeed + downForce ;//+3 is y axis
	px = px + cos(rotation)*curSpeed;

	px = px + knockbackX/2.0;
	knockbackX = knockbackX - knockbackX/2.0;
	py = py + knockbackY/2.0;
	knockbackY = knockbackY - knockbackY/2.0;
	//		###  end plane position  ###  
}

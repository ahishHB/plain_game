#ifndef bullets
#define bullets
#include "otherClass.hpp"
#include <cmath>
#include <cstddef>
#include <vector>
#include <array>

using std::vector;
using std::array;

class globalBullets{
	public:
		static vector<array<double,5>> allBullets;// [x,y,xSp,ySp,dmg]

		static void bulletsUpdate(){
			for(int i=0;i<allBullets.size();i=i+1){
				allBullets[i][0] = allBullets[i][0] + allBullets[i][2];		//x update
				allBullets[i][1] = allBullets[i][1] + allBullets[i][3];		//y update
				if(allBullets[i][2]>5||allBullets[i][2]<-5){		//the x speed wont go lower than this
					allBullets[i][2] =allBullets[i][2]-(allBullets[i][2]*0.001);	//for reducing x speed properly
				}
				/*
				if(std::abs(allBullets[i][2])>5){
					if(std::signbit(allBullets[i][2])){
						allBullets[i][2] = 
					}
					else{

					}
				}
				*/
				if(allBullets[i][3]<20){
					allBullets[i][3] = allBullets[i][3] + 0.03*allBullets[i][4];//based on the dmg the drop is faster

				}
				if((allBullets[i][0])>=15424||(allBullets[i][1]>=10383)||(allBullets[i][0]<=965)||(allBullets[i][1]<=545)){
					allBullets.erase(allBullets.begin()+i);
					i=i-1;
				}
			}
		}

};
#endif
#ifndef otherclass
#define otherclass
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <string>
#include <fstream>

using std::array;
using std::vector;
using std::ifstream;
using std::string;

double calculate(double x,vector<double> eq);
double calculateR(double x,vector<double> eq);

class aeroframe{
	public:
		double aeroMaxHp;
		double aeroCurHp;
		double aeroWeight; // calculate the weight and hp etc while in the main loop while player creates the plane , eg when he confirms values the the weight will be like
		double Cd;	  // plane weight = maxHp + gundamage/2 + etc etc ; 
		
		//gravity calculations that reduce speeed
		vector <double> gAlt;
		vector <double> gSpeed;
		vector <double> gWeight;
		
		//drag calculation eqs
		vector <double> dAlt;
		vector <double> dSpeed;
		vector <double> dRdiff;

		vector<double> ySpeed ={5.0851,-0.4134,0.0174,-0.0006};
		vector<double> tSpeed;	//turn rate for different speeds

		// constructs
		aeroframe(double aeroHp,double weight,double C,vector<double> gAltInit,vector<double> gWeightInit,vector<double> dAltInit,vector<double> dSpeedInit,vector<double> dRdiffInit,vector<double> tSpeediInit);
		aeroframe(aeroframe &c);
		aeroframe();
		
		// functions
		void printAll();
		double calGrav(double alt);
		double calDrag(double alt,double speed,double rdiff);
		double calTurn(double speed);
};
class engine{
	public:
		double enginePow;
		double enginePerc;
		
		vector <double> eAlt;
		vector <double> eSpeed;
		
		// constructs
		engine(double MaxPow,vector<double> eAltInit, vector<double> eSpeedInit);
		engine(engine &t);
		engine();
		
		// functions
		void printAll();
		double cal(double alt,double speed);

};

class gun{
	public:
		int ammo;
		int dmg;
		double velocity;	
		int rateOfFire;	// 1 means it shoots every other tick
		bool firstShot=true;
		gun(int ammo,int dmg,double velocity,int rateOfFire){
			this->ammo = ammo;
			this->dmg = dmg;
			this->velocity = velocity;
			this->rateOfFire = rateOfFire;
		}
		gun(){}
};

vector<double> converts(string s);
void fillVectorEngine(vector<vector<double>> &v1Alt,vector<vector<double>> &v2Sp);
void fillVectorDrag(vector<double> &v1Alt,vector<double> &v2Sp,vector<double> &v3Rdiff);
void fillVectorGravity(vector<double> &v1Alt,vector<double> &v2Wg);
void fillTurnSpeed(vector<vector<double>> &v1Turn);

#endif
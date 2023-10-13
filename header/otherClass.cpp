#include "otherClass.hpp"
#include <vector>
double calculate(double x,vector<double> eq){
	double result=0.0;
	for(int i=0;i<eq.size();i=i+1){
		result = result + (eq[i]*(pow(x,i)));	
	}
	return result/1000.0;
}
double calculateR(double x,vector<double> eq){
	double result=0.0;
	for(int i=0;i<eq.size();i=i+1){
		result = result + (eq[i]*(pow(x,i)));	
	}
	return result;
}
void print(vector<double> x){
	if(!x.size()){
		std::cout<<"empty\n";	
	}
	for(auto a:x){
		std::cout<<a<<",";	
	}
	std::cout<<"\n";
}
aeroframe::aeroframe(double aeroHp,double weight,double C,vector<double> gAltInit,vector<double> gWeightInit,vector<double> dAltInit,vector<double> dSpeedInit,vector<double> dRdiffInit,vector<double> tSpeediInit){
	aeroMaxHp = aeroHp;
	aeroCurHp = aeroHp;
	aeroWeight = weight;
	Cd = C;
	gWeight = gWeightInit;
	gAlt = gAltInit;
	dAlt = dAltInit;
	dSpeed = dSpeedInit;
	dRdiff = dRdiffInit;
	tSpeed = tSpeediInit;
}

aeroframe::aeroframe(aeroframe &c){
	aeroMaxHp = c.aeroMaxHp;
	aeroCurHp = c.aeroCurHp;
	aeroWeight = c.aeroWeight;
	Cd = c.Cd;
	gWeight = c.gWeight;
	gAlt = c.gAlt;
    dAlt = c.dAlt;
    dSpeed = c.dSpeed;
    dRdiff = c.dRdiff;
	tSpeed = c.tSpeed;
	ySpeed = c.ySpeed;
}

aeroframe::aeroframe(){
}

void aeroframe::printAll(){
	std::cout<<"aeroframe\n";
	std::cout<<"weight formula:\n";
	print(gWeight);
	std::cout<<"altitude formula:\n";
	print(gAlt);
	std::cout<<"drag\n";
	std::cout<<"alt formula:\n";
	print(dAlt);
	std::cout<<"speed formula:\n";
	print(dSpeed);
	std::cout<<"rdiff formula:\n";
	print(dRdiff);
		
}

double aeroframe::calGrav(double alt){
	double re = calculate(alt/1000.0,gAlt) + calculate(aeroWeight,gWeight);
	return re;
}

double aeroframe::calDrag(double alt,double speed,double rdiff){
	double re = calculate(alt/1000.0,dAlt) + 2.0*calculate(speed,dSpeed) + 4.0*calculate(rdiff*100.0,dRdiff);
	//std::cout<<calculate(rdiff*100.0,dRdiff)<<std::endl;
	//std::cout<<calculate(speed,dSpeed)<<std::endl;
	re = re*Cd;
	return re;
}

double aeroframe::calTurn(double speed){
	double re = calculate(speed,tSpeed);

	return re;
}

engine::engine(double MaxPow,vector<double> eAltInit, vector<double> eSpeedInit){
	enginePow = MaxPow;
	enginePerc = 1;
	eAlt = eAltInit;
	eSpeed = eSpeedInit;
}

engine::engine(engine &t){
	enginePow = t.enginePow;
	enginePerc = t.enginePerc;
	eAlt = t.eAlt;
        eSpeed = t.eSpeed;
	enginePerc = 0.00001;
}

engine::engine(){
}

void engine::printAll(){
	std::cout<<"engine out\n";
	std::cout<<"alt formula:\n";
	std::cout<<eAlt.size()<<std::endl;
	print(eAlt);
	std::cout<<"speed formula\n";
	print(eSpeed);
}
double engine::cal(double alt,double speed){
	double re;
	if(speed > 5){
		re = calculate(alt/1000.0,eAlt) + calculate(speed,eSpeed);
	}
	else{
		re = 0.001;
	} 
	re = re*enginePerc;
	//std::cout<<re<<std::endl;
	return re;
}

vector<double> converts(string s){	//converts :12.33,-23.3,0.1 ..... to a vector without the colon
	vector <double> v1;
	double f=0;
	int c=0;
	int sign=1;
	for(int i =1;i<s.length();i=i+1){
		if(s[i]==','){
			v1.push_back(sign*f);
			c=0;f=0.0,sign=1;
			continue;
		}
		else if(s[i]=='.'){
			c = 10;
			continue;
		}
		else if(s[i]=='-'){
			sign = -1;
			continue;
		}
		if(c==0){
			f = f*10+ (s[i]-'0');	
		}
		else{
			f= f+((double)(s[i]-'0'))/((double)c);
			c=c*10;
		}
	}
	v1.push_back(f*sign);
	return v1;
}

void fillVectorEngine(vector<vector<double>> &v1Alt,vector<vector<double>> &v2Sp){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("./res/gameValues/engine.txt");
	while(getline(f1,s1)){
		if(s1[0]==':'){
			if(index == 0){
				v1Alt.push_back(converts(s1));
			}

			if(index == 1){
				v2Sp.push_back(converts(s1));
			}
		}
		if(s1 == "//"){
			index = index+1;
		}
	}
}


void fillVectorDrag(vector<double> &v1Alt,vector<double> &v2Sp,vector<double> &v3Rdiff){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("./res/gameValues/drag.txt");
	while(getline(f1,s1)){
		
		if(s1[0]==':'){
			if(index == 0){
				v1Alt=converts(s1);
			}
			if(index == 1){
				v2Sp=converts(s1);
			}
			if(index == 2){
				v3Rdiff=converts(s1);
			}
		}	
		if(s1 == "//"){
			index = index+1;
		}
	}
}

void fillVectorGravity(vector<double> &v1Alt,vector<double> &v2Wg){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("./res/gameValues/gravity.txt");
	while(getline(f1,s1)){
		
		if(s1[0]==':'){
			if(index == 0){
				v1Alt=converts(s1);
			}
			if(index == 1){
				v2Wg=converts(s1);
			}
		}	
		if(s1 == "//"){
			index = index+1;
		}
	}
}

void fillTurnSpeed(vector<vector<double>> &v1Turn){
	string s1;
	ifstream f1;
	f1.open("./res/gameValues/turn.txt");
	while(getline(f1,s1)){
		if(s1[0]==':'){
			v1Turn.push_back(converts(s1));
		}	
	}
}

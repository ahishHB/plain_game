#include<fstream>
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<cmath>
using std::pow;
using std::reverse;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

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

void fillVectorEngine(vector<vector<double>> &v1,vector<vector<double>> &v2){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("engine.txt");
	while(getline(f1,s1)){
		if((s1[0]==':')){
			if(index == 0){
				v1.push_back(converts(s1));
			}

			if(index == 1){
				v2.push_back(converts(s1));
			}
		}
		if(s1 == "//"){
			index = index+1;
		}
	}
}


void fillVectorDrag(vector<double> &v1,vector<double> &v2,vector<double> &v3){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("drag.txt");
	while(getline(f1,s1)){
		
		if((s1[0]==':')){
			if(index == 0){
				v1=converts(s1);
			}
			if(index == 1){
				v2=converts(s1);
			}
			if(index == 2){
				v3=converts(s1);
			}
		}	
		if(s1 == "//"){
			index = index+1;
		}
	}
}

void fillVectorGravity(vector<double> &v1,vector<double> &v2){
	string s1;
	int index=0;
	ifstream f1;
	f1.open("gravity.txt");
	while(getline(f1,s1)){
		
		if((s1[0]==':')){
			if(index == 0){
				v1=converts(s1);
			}
			if(index == 1){
				v2=converts(s1);
			}
		}	
		if(s1 == "//"){
			index = index+1;
		}
	}
}

void print(vector<double> v){
	for(int i=0;i<v.size();i=i+1){
		if(i!=0){
			cout<<",";	
		}
		cout<<v[i]<<" ";	
	}	
	cout<<endl;
}
double calculate(double x,vector<double> eq){
	double result=0;
	for(int i=0;i<eq.size();i=i+1){
		result = result + (eq[i]*(pow(x,i)));	
		//cout<<"x pow: "<<x<<" i= "<<i<<" :: "<<"eq: "<<eq[i]<<" :: "<<(eq[i]*(pow(x,i)))<<"\n";
	}
	return result/1000.0;
}
int main(){
	cout.precision(2);
	vector<vector<double>> eAtl,eSp;
	vector<double> dAtl,dSp,dRdiff,gAtl,gWg;
	fillVectorEngine(eAtl,eSp);
	fillVectorDrag(dAtl,dSp,dRdiff);
	fillVectorGravity(gAtl,gWg);
	cout<<"engine altitude variation\n";
	for(auto e : eAtl){
		//print(e);	
	}
	cout<<"engine speed variation\n";
	for(auto e : eSp){
		//print(e);	
	}
	double x;
	bool run = true;
	cout<<"\n\n\n\n";
	vector <double> newEq = {1000.846156,-737.371812,227.135251,-37.957936,3.734058,-0.217014,0.006916,-0.000093};
	//print(newEq);
	cout<<"\n\n\n";
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"engine altitude\n";
	for(int i=1;i<11;i=i+1){
		if(i==1){
			for(int j=0;j<eAtl.size();j=j+1){
				cout<<calculate(0.5,eAtl[j])<<"    ";
			}
		}	
		else if(i==10){
			for(int j=0;j<eAtl.size();j=j+1){
				cout<<calculate(10.5,eAtl[j])<<"   ";
			}
		}
		else{
			for(int j=0;j<eAtl.size();j=j+1){
				cout<<calculate(i,eAtl[j])<<"    ";
			}
		}
		cout<<endl;
	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"engine speed\n";
	for(int i =5;i<16;i=i+1){
		for(auto a : eSp){
			cout<<calculate(i,a)<<"    ";	
		}	
		cout<<endl;

	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"drag alt\n";
	for(int i = 1;i<11;i=i+1){
		if(i==1){
			cout<<calculate(0.5,dAtl)<<"    ";
		}	
		else if(i==10){
			cout<<calculate(10.5,dAtl)<<"   ";
		}
		else{
			cout<<calculate(i,dAtl)<<"    ";
		}

		cout<<endl;
	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"drag speed\n";
	for(int i=5;i<16;i=i+1){
		cout<<calculate(i,dSp)<<"    ";	
		cout<<endl;
	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"drag rDiff\n";
	for(int i=1;i<9;i=i+1){
		cout<<calculate(i,dRdiff)<<"    ";	
		cout<<endl;
	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"gravity Atl\n";

	for(int i = 1;i<11;i=i+1){
		if(i==1){
			cout<<calculate(0.5,gAtl)<<"    ";
		}	
		else if(i==10){
			cout<<calculate(10.5,gAtl)<<"   ";
		}
		else{
			cout<<calculate(i,gAtl)<<"    ";
		}

		cout<<endl;
	}
	cout<<"enter 1 to continue\n";
	cin>>run;
	cout<<"gravity Wg\n";
	for(int i=25;i<85;i=i+10){
		cout<<calculate(i,gWg);
		cout<<endl;	
	}
	/*
	reAtl = eAtl;
	reSp = eSp;
	rdAtl=dAtl;
	rdSp=dSp;
	rdRdiff=dRdiff;
	rgAtl=gAtl;
	rgWg=gWg;
	cout<<"\nbefore reverse\n";
	for(int i=0;i<eAtl.size();i=i+1){
		print(eAtl[i]);	
	}	
	
	cout<<"\nafter reverse\n";
	for(int i=0;i<reAtl.size();i=i+1){
		reverse(reAtl[i].begin(),reAtl[i].end());
		print(reAtl[i]);	
	}	
			
	cout<<"\nbefore reverse\n";
	for(int i=0;i<eSp.size();i=i+1){
		print(eSp[i]);	
	}	
	
	cout<<"\nafter reverse\n";
	for(int i=0;i<reSp.size();i=i+1){
		reverse(reSp[i].begin(),reSp[i].end());
		print(reSp[i]);	
	}	
	cout<<"\nbefore reversing\n";
	print(dAtl);
	reverse(rdAtl.begin(),rdAtl.end());
	cout<<"\nafter reversing\n";
	print(rdAtl);

	cout<<"\nbefore reversing\n";
	print(dSp);
	reverse(rdSp.begin(),rdSp.end());
	cout<<"\nafter reversing\n";
	print(rdSp);

	cout<<"\nbefore reversing\n";
	print(dRdiff);
	reverse(rdRdiff.begin(),rdRdiff.end());
	cout<<"\nafter reversing\n";
	print(rdRdiff);

	cout<<"\nbefore reversing\n";
	print(gAtl);
	reverse(rgAtl.begin(),rgAtl.end());
	cout<<"\nafter reversing\n";
	print(rgAtl);

	cout<<"\nbefore reversing\n";
	print(gWg);
	reverse(rgWg.begin(),rgWg.end());
	cout<<"\nafter reversing\n";
	print(rgWg);
	
	
	ofstream of;
        of.open("drag1.txt");
	of<<"same thing as engine ,refer that\n"<<"DRAG\n"<<"\naltitude variation\n\n:";

	for(int i=0;i<rdAtl.size();i=i+1){
		if(i>0){
			of<<",";	
		}
		of<<rdAtl[i];
	}
	of<<"\n//\nspeed variation\n\n:";
	for(int i=0;i<rdSp.size();i=i+1){
		if(i>0){
			of<<",";	
		}
		of<<rdSp[i];
	}
	of<<"\n//\nrdiff variation\n\n:";
	for(int i=0;i<rdRdiff.size();i=i+1){
		if(i>0){
			of<<",";	
		}
		of<<rdRdiff[i];
	}

	of.close();
	
	of.open("gravity1.txt");
	of<<"just gravity stuff 0-0.0175\n\naltitude 0-0.0015\n:";
	for(int i=0;i<rgAtl.size();i=i+1){
		if(i>0){
			of<<",";	
		}	
		of<<rgAtl[i];
	}	
	of<<"\n//\nweight 0-0.008 maybe do a 2x\n:";
	for(int i=0;i<rgWg.size();i=i+1){
		if(i>0){
			of<<",";	
		}	
		of<<rgWg[i];
	}
	of.close();	
	of.open("engine1.txt");
	of<<"engine deets\n\naltitude variation\n\n";
	for(auto a : reAtl){
		of<<":";
		for(int i=0;i<a.size();i=i+1){
			if(i>0){
				of<<",";	
			}	
			of<<a[i];
		}
		of<<endl;
	}
	of<<"\n//\nspeed variation\n\n:";

	for(auto a : reSp){
		of<<":";
		for(int i=0;i<a.size();i=i+1){
			if(i>0){
				of<<",";	
			}	
			of<<a[i];
		}
		of<<endl;
	}
		
	*/
	return 0;
}

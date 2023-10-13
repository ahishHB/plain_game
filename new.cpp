#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <cstring>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "header/bulletClass.hpp"
#include "header/planeClass.hpp"
#include "header/otherClass.hpp"
#include <ostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <array>
#include <malloc.h>
#include <cstdlib>

using std::vector;// no need to use std everytime
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::array;


struct mallinfo2 before,after;
int totsz;

vector<array<double,5>> globalBullets::allBullets;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window *gWindow = NULL;
SDL_Texture *rPointer = NULL;
SDL_Texture *rSky = NULL;
SDL_Texture *rEscape = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *Font = NULL; 

// start of sdl video handle //
// till line 90 
bool myInit(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		cout<<"sdl could not initialize "<<SDL_GetError()<<endl;
		return false;
	}
	gWindow = SDL_CreateWindow("test_my_guy",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if(gWindow==NULL){
		cout<<"window couldnt be init "<<SDL_GetError()<<endl;
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL){
		cout<<"renderer couldnt be createdd "<<SDL_GetError()<<endl;
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
	if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){
		cout<<"SDL_images could not be initialized \n";
		return false;
	}
	TTF_Init();
	Font = TTF_OpenFont("./fonts/antonioLight.ttf",40);

	return true;
}
SDL_Texture* loadThis(string path){
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL){
		cout<<"couldnt load image "<<path.c_str()<<IMG_GetError()<<endl;

	}
	else{
		newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
		if(newTexture == NULL){
			cout<<"could not make texture "<<SDL_GetError()<<endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

bool loadTexture(){
	rSky = loadThis("res/sky.png");
	rPointer = loadThis("res/pointer.png");
	if(rSky == NULL){
		cout<<"could not load sky.png\n";
		return false;
	}
	if(rPointer== NULL){
		cout<<"could not load pointer.png\n";
		return false;
	}
	return true;

}

//usage:: DisplayText(urstring,10,SDL_Rect(x,y,w,h),SDL_Colour(255,255,255));
void DisplayText(string text,SDL_Rect displayRect,SDL_Colour color){
	char *charFromString = new char[text.length()+1];
	std::strcpy(charFromString, text.c_str());

	if(Font == NULL){
		cout<<"init error "<<TTF_GetError()<<endl;
	}
	SDL_Surface *textSurface = TTF_RenderText_Solid(Font,charFromString, color);
	SDL_Texture *message = SDL_CreateTextureFromSurface(gRenderer, textSurface);	
	SDL_RenderCopy(gRenderer, message, NULL, &displayRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(message);
	delete [] charFromString;
	return;
}
void DisplayNum(double num,SDL_Rect displayRect,SDL_Colour color,int length = 6){
	string text = std::to_string(num);
	char *charFromString = new char[text.length()+1];
	//cout<<text<<endl;
	std::strcpy(charFromString, text.c_str());
	charFromString[length] = '\0';
	if(Font == NULL){
		cout<<"init error "<<TTF_GetError()<<endl;
	}
	SDL_Surface *textSurface = TTF_RenderText_Solid(Font,charFromString, color);
	SDL_Texture *message = SDL_CreateTextureFromSurface(gRenderer, textSurface);	
	SDL_RenderCopy(gRenderer, message, NULL, &displayRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(message);
	charFromString[length] = '0';
	delete [] charFromString;
	return;
}


void close(){
	SDL_DestroyTexture(rPointer);
	SDL_DestroyTexture(rSky);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	rPointer = NULL;
	rSky = NULL;
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}
// end of video handle 

// start translate input
double getAngle(int x,int y){
	int diffx = x - 960;
	int diffy = y - 540;
	double result = atan2(((double)diffy),(double) diffx);
	return result;
}
// end of input translate

//  ####  input handle  ####  //

void getInput(vector<bool> &keys,SDL_Point &mousePos){
	int count =0;
	//vector <bool> keys = {false,false,false,false,false,false};//{w,s,a,d,quit,esc}
	SDL_Event e;
	while(SDL_PollEvent(&e)!=0){
		//cout<<count++<<endl;
		if(e.type == SDL_QUIT){
			keys[4] = true;
		}
		else if(e.type == SDL_KEYDOWN){
			switch(e.key.keysym.sym){
				case SDLK_w:
					keys[0] = true;
					break;
				case SDLK_s:
					keys[1] = true;
					break;
				case SDLK_a:
					keys[2] = true;
					break;
				case SDLK_d:
					keys[3] = true;
					break;
				case SDLK_ESCAPE:
					if(keys[5]){
						keys[5] = false;
						break;
					}
					keys[5] = true;
					break;
				default:
					break;
			}
		}
		else if(e.type == SDL_MOUSEBUTTONDOWN){
			if(e.button.button == SDL_BUTTON_LEFT){
				keys[6] = true;
			}
			else if(e.button.button == SDL_BUTTON_RIGHT){
				keys[7] = true;
			}
		}
		else if(e.type == SDL_KEYUP){
			switch(e.key.keysym.sym){
				case SDLK_w:
					keys[0] = false;
					break;
				case SDLK_s:
					keys[1] = false;
					break;
				case SDLK_a:
					keys[2] = false;
					break;
				case SDLK_d:
					keys[3] = false;
					break;
				default:
					break;
			}
		}
		else if(e.type == SDL_MOUSEBUTTONUP){
			if(e.button.button == SDL_BUTTON_LEFT){
				keys[6] = false;
			}
			else if(e.button.button == SDL_BUTTON_RIGHT){
				keys[8] = false;
			}
		}
		else if(e.type == SDL_MOUSEMOTION){
			SDL_GetMouseState(&(mousePos.x),&(mousePos.y));
		}
	}
}



//### end of getting input ###//
void drawRect(SDL_Renderer *gRenderer1,int x,int y,int w,int h,int r=255,int g=255,int b=255,bool fillBool=true){
	
	SDL_Rect pos = {x,y,w,h};
	SDL_SetRenderDrawColor(gRenderer1,r,g,b,255);
	SDL_RenderDrawRect(gRenderer1,&pos);
	if(fillBool){
		SDL_RenderFillRect(gRenderer1,&pos);
	}

}

void displayAllBulletes(int px,int py){
	for(int i =0;i<globalBullets::allBullets.size();i=i+1){
		if((std::abs(globalBullets::allBullets[i][0]-px)<960)&&(std::abs(globalBullets::allBullets[i][1]-py)<540)){
			drawRect(gRenderer,globalBullets::allBullets[i][0]-px+960,globalBullets::allBullets[i][1]-py+540,(6+2*globalBullets::allBullets[i][4])/2,(6+2*globalBullets::allBullets[i][4])/2,255,50,0);
		}
	}
}
void displayPlane(const plane &p1,int playerX,int playerY){
	SDL_Point temp ={p1.hitBoxX/2,p1.hitBoxY/2};		// center of the plane to rotate the image
	//SDL_Point temp ={0,0};		// center of the plane to rotate the image
	SDL_Rect dstRect= {(int)((SCREEN_WIDTH/2.0)+(p1.px-playerX)-p1.hitBoxX/2.0),(int)((SCREEN_HEIGHT/2.0)+(p1.py-playerY)-p1.hitBoxY/2.0),p1.hitBoxX,p1.hitBoxY}; //where the player image is placed
	//SDL_Rect dstRect= {(int)((SCREEN_WIDTH/2)+(p1.px-playerX)),(int)((SCREEN_HEIGHT/2)+(p1.py-playerY)),p1.hitBoxX,p1.hitBoxY}; //where the player image is placed
	SDL_RenderCopyEx(gRenderer,rPointer,NULL,&dstRect,p1.rotation*(180/M_PI),&temp,(SDL_RendererFlip)SDL_FLIP_NONE); // copy player plane to renderer
}

void DisplayTri(const SDL_FPoint p1,const SDL_FPoint p2,const SDL_FPoint p3,const SDL_Color c){
	SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
	vector<SDL_Vertex> verts={{p1,c,{1,1}},
							  {p2,c,{1,1}},
							  {p3,c,{1,1}}};
	SDL_RenderGeometry(gRenderer, nullptr, verts.data(), verts.size(), nullptr,0);
}

int main(){
	before = mallinfo2();	
	//### texture loading ###//
	if(!myInit()){
		cout<<"could not initialize\n";
		return 0;
	}
	if(!loadTexture()){
		cout<<"could not load textures\n";
		return 0;
	}
	
	//### fps handling ###//
	int fps = 120;
	int minTimeForFrame = 1000/fps;
	Uint32 gameRunTime;
	int frameTime;

	int state =4;		//game state
	int counter =0; 	//for flashing images
	
	unsigned int seed;
	vector <bool> keys;		//all input keys
	vector<vector<double>> engAlt;	// all possible engineAlt configs
	vector<vector<double>> engSp;	// same as above (0-6)
	vector<double> dragAlt;			//
	vector<double> dragSp;			//
	vector<double> dragRdiff;		//
	vector<double> gravAlt;			//
	vector<double> gravWg;			//
	vector<vector<double>> turnSpeed;//
	SDL_Rect srcRect= {0,0,SCREEN_WIDTH-25,SCREEN_HEIGHT-12}; 	//picture of the entire sky and offsets
	SDL_Point mousePos;	
	engine e1;
	aeroframe a1;
	vector<gun> g1;
	plane player;
	SDL_Rect rectForText;
	SDL_Color colorForText;
	fillVectorDrag(dragAlt, dragSp, dragRdiff);
	fillVectorEngine(engAlt, engSp);
	fillVectorGravity(gravAlt, gravWg);
	fillTurnSpeed(turnSpeed);

	bool clickUp;
	//gun selections
	int gun1VelSel,gun1dmgSel,gun1AmmoSel,gun1RateSel,gun1Wg,gun1Bool;
	int gun2VelSel,gun2dmgSel,gun2AmmoSel,gun2RateSel,gun2Wg,gun2Bool;
	int gun3VelSel,gun3dmgSel,gun3AmmoSel,gun3RateSel,gun3Wg,gun3Bool;

	//engine
	int engPowSel;
	int engAltSel,engSpSel;

	//aeroframe;
	int aeroHpSel,aeroWgSel;
	int turnSel;

	while(state){	
	switch (state){   
		case 4:		// main menu
			
			state = 1;
			keys = {false,false,false,false,false,false,false,false};//{w,s,a,d,quit,esc,leftclick,rightclick}
			mousePos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
			gun1VelSel=10;	//10-30
			gun1dmgSel=1;  	//1-3
			gun1AmmoSel=150;//150-450 step150
			gun1RateSel=gun1dmgSel*5 + gun1VelSel*0.8333;
			gun1Wg = ((gun1dmgSel*10+gun1VelSel)/2 + gun1AmmoSel/15)/4;

			gun2Bool = 0;
			gun2VelSel=10;	//10-30
			gun2dmgSel=1;  	//1-3
			gun2AmmoSel=150;//150-450 step150
			gun2RateSel=gun2dmgSel*5 + gun2VelSel*0.8333;
			gun2Wg = ((gun2dmgSel*10+gun2VelSel)/2 + gun2AmmoSel/15)/4;

			gun3Bool =0;
			gun3VelSel=10;	//10-30
			gun3dmgSel=1;  	//1-3
			gun3AmmoSel=150;//150-450 step150
			gun3RateSel=gun3dmgSel*5 + gun3VelSel*0.8333;
			gun3Wg = ((gun3dmgSel*10+gun3VelSel)/2 + gun3AmmoSel/15)/4;

			//engine
			engPowSel=10;//10-30
			engAltSel=0; //0-6
			engSpSel=0;  //0-6

			//aeroframe;
			aeroHpSel=5;// 5 - 15
			aeroWgSel=aeroHpSel+(gun1Wg+gun2Wg+gun3Wg)/3;
			turnSel=0; // 0-4

			while(!keys[4]){
				gameRunTime = SDL_GetTicks();

				//  ####  input handle  ####  //
				getInput(keys,mousePos);

				if(!keys[6]){
					clickUp = true;
				}	

				SDL_RenderClear(gRenderer);
				drawRect(gRenderer, 0, 0, 1920, 1080,0,0,0);

				//for the buttons
				if(mousePos.x<100&&mousePos.y<50){
					if(keys[6]){
						keys[4] = true;
						state = 0;
					}
					drawRect(gRenderer,SCREEN_WIDTH-100,SCREEN_HEIGHT-50,100,50,100,205,0);
					drawRect(gRenderer,0,0,100,50,255,12,15);
				}
				else if (mousePos.x>SCREEN_WIDTH-100&&mousePos.y>SCREEN_HEIGHT-50){
					if(keys[6]){
						keys[4] = true;
						state = 1;
					}
					drawRect(gRenderer,SCREEN_WIDTH-100,SCREEN_HEIGHT-50,100,50,255,12,15);
					drawRect(gRenderer,0,0,100,50,100,205,0);
				}
				else{
					drawRect(gRenderer,0,0,100,50,100,205,0);
					drawRect(gRenderer,SCREEN_WIDTH-100,SCREEN_HEIGHT-50,100,50,100,205,0);
				}
				DisplayText("QUIT",{10,5,80,40},{255,255,255});
				DisplayText("START",{SCREEN_WIDTH-90,SCREEN_HEIGHT-45,80,40},{255,255,255});

				drawRect(gRenderer,0,0,2,1080,50,100,50);
				drawRect(gRenderer,0,0,1920,0,50,100,50);
				drawRect(gRenderer,1918,0,2,1080,50,100,50);
				drawRect(gRenderer,0,1078,1920,2,50,100,50);
				drawRect(gRenderer,979,0,2,1080,50,100,50);
				drawRect(gRenderer,0,360,980,2,50,100,50);
				drawRect(gRenderer,0,720,980,2,50,100,50);
				drawRect(gRenderer,980,720,980,2,50,100,50);
				drawRect(gRenderer,980,480,980,2,50,100,50);
				drawRect(gRenderer,980,240,980,2,50,100,50);
				DisplayTri({200,80}, {220,60}, {220,100}, {50,150,50});
				DisplayTri({200,180}, {220,160}, {220,200}, {50,150,50});
				DisplayTri({960,80}, {940,60}, {940,100}, {50,150,50});
				DisplayTri({960,180}, {940,160}, {940,200}, {50,150,50});


				drawRect(gRenderer,1240,15,205*gun1dmgSel,30,50,150,50);
				drawRect(gRenderer,1240,15+240,205*gun2dmgSel,30,50,150,50);
				drawRect(gRenderer,1240,15+480,205*gun3dmgSel,30,50,150,50);
				drawRect(gRenderer, 1240,15+50,20.5*gun1VelSel,30,50,150,50);
				drawRect(gRenderer, 1240,15+240+50,20.5*gun2VelSel,30,50,150,50);
				drawRect(gRenderer, 1240,15+480+50,20.5*gun3VelSel,30,50,150,50);
				drawRect(gRenderer,1240,15+100,205*(gun1AmmoSel/150),30,50,150,50);
				drawRect(gRenderer,1240,15+100+240,205*(gun2AmmoSel/150),30,50,150,50);
				drawRect(gRenderer,1240,15+100+480,205*(gun3AmmoSel/150),30,50,150,50);
				drawRect(gRenderer,1240,15+150,7995/gun1RateSel,30,50,150,50);
				drawRect(gRenderer,1240,15+150+240,7995/gun2RateSel,30,50,150,50);
				drawRect(gRenderer,1240,15+150+480,7995/gun3RateSel,30,50,150,50);
				drawRect(gRenderer,240,60,22.666*engPowSel,40,50,150,50);
				drawRect(gRenderer,240,160,45.333*aeroHpSel,40,50,150,50);
				drawRect(gRenderer,240,260,22.666*aeroWgSel,40,50,150,50);

				for(int i=0;i<720;i=i+240){
					DisplayText("dmg        :",{1100,20+0+i,100,20},{50,175,50});
					DisplayText("velocity   :",{1100,20+50+i,100,20},{50,175,50});
					DisplayText("ammo       :",{1100,20+100+i,100,20},{50,175,50});
					DisplayText("rateOfFire :",{1100,20+150+i,100,20},{50,175,50});
					
					DisplayTri({1205,(float)30+0+i}, {1220,(float)15+0+i},{1220,(float)45+0+i}, {50,175,50});	
					DisplayTri({1205,(float)30+50+i}, {1220,(float)15+50+i},{1220,(float)45+50+i}, {50,175,50});	
					DisplayTri({1205,(float)30+100+i}, {1220,(float)15+100+i},{1220,(float)45+100+i}, {50,175,50});	
					DisplayTri({1890,(float)30+0+i}, {1875,(float)15+0+i},{1875,(float)45+0+i}, {50,175,50});	
					DisplayTri({1890,(float)30+50+i}, {1875,(float)15+50+i},{1875,(float)45+50+i}, {50,175,50});	
					DisplayTri({1890,(float)30+100+i}, {1875,(float)15+100+i},{1875,(float)45+100+i}, {50,175,50});	
					
					// triple layered boundaries
					drawRect(gRenderer, 1240, 15+0+i, 615, 30,150,50,50,false);
					drawRect(gRenderer, 1240, 15+50+i, 615, 30,150,50,50,false);
					drawRect(gRenderer, 1240, 15+100+i, 615, 30,150,50,50,false);
					drawRect(gRenderer, 1240, 15+150+i, 615, 30,150,50,50,false);
					drawRect(gRenderer, 1241, 16+0+i, 613, 28,150,50,50,false);
					drawRect(gRenderer, 1241, 16+50+i, 613, 28,150,50,50,false);
					drawRect(gRenderer, 1241, 16+100+i, 613, 28,150,50,50,false);
					drawRect(gRenderer, 1241, 16+150+i, 613, 28,150,50,50,false);
					drawRect(gRenderer, 1242, 17+0+i, 611, 26,150,50,50,false);
					drawRect(gRenderer, 1242, 17+50+i, 611, 26,150,50,50,false);
					drawRect(gRenderer, 1242, 17+100+i, 611, 26,150,50,50,false);
					drawRect(gRenderer, 1242, 17+150+i, 611, 26,150,50,50,false);
					// end of triple layered

					drawRect(gRenderer,1240+205,15+0+i,3,30,150,50,50);
					drawRect(gRenderer,1240+410,15+0+i,3,30,150,50,50);
					drawRect(gRenderer,1240+205,15+50+i,3,30,150,50,50);
					drawRect(gRenderer,1240+410,15+50+i,3,30,150,50,50);
					drawRect(gRenderer,1240+205,15+100+i,3,30,150,50,50);
					drawRect(gRenderer,1240+410,15+100+i,3,30,150,50,50);
				}

				// triple layered meters

				drawRect(gRenderer, 240+225, 60, 3, 40,150,50,50);
				drawRect(gRenderer, 240+450, 60, 3, 40,150,50,50);
				drawRect(gRenderer, 240, 60, 680, 40,150,50,50,false);
				drawRect(gRenderer, 241, 61, 678, 38,150,50,50,false);
				drawRect(gRenderer, 242, 62, 676, 36,150,50,50,false);


				drawRect(gRenderer, 240+225, 160, 3, 40,150,50,50);
				drawRect(gRenderer, 240+450, 160, 3, 40,150,50,50);
				drawRect(gRenderer, 240, 160, 680, 40,150,50,50,false);
				drawRect(gRenderer, 241, 161, 678, 38,150,50,50,false);
				drawRect(gRenderer, 242, 162, 676, 36,150,50,50,false);

				drawRect(gRenderer, 240, 260, 680, 40,150,50,50,false);
				drawRect(gRenderer, 241, 261, 678, 38,150,50,50,false);
				drawRect(gRenderer, 242, 262, 676, 36,150,50,50,false);
				// 
				DisplayText("Gun 1",{1000,20,80,40},{50,175,50});
				DisplayText("Gun 2",{1000,260,80,40},{50,175,50});
				DisplayText("Gun 3",{1000,500,80,40},{50,175,50});
				DisplayText("Speed",{1400,1020,60,20},{50,175,50});
				DisplayText("Speed",{450,1030,60,20},{50,175,50});
				DisplayText("Altitude",{450,680,75,20},{50,175,50});
				DisplayText("TurnRate",{990,900,75,20},{50,175,50});
				DisplayText("EnginePow",{10,900,80,20},{50,175,50});
				DisplayText("EnginePow",{10,540,80,20},{50,175,50});
				DisplayText("EnginePow   :",{20,60,170,40},{50,175,50});
				DisplayText("TotalHP     :",{20,160,170,40},{50,175,50});
				DisplayText("TotalWeight :",{20,260,170,40},{50,175,50});
				drawRect(gRenderer, 1020, 95, 25, 25,150,50,50);
				if(gun2Bool){
					drawRect(gRenderer, 1020, 240+95, 25, 25,150,50,50);
				}
				else{
					drawRect(gRenderer, 1020, 240+95, 25, 25,50,150,50);
				}
				if(gun3Bool){
					drawRect(gRenderer, 1020, 480+95, 25, 25,150,50,50);
				}
				else{
					drawRect(gRenderer, 1020, 480+95, 25, 25,50,150,50);
				}

				if(keys[6]&&clickUp){
					//engine Pow
					if((mousePos.x>=200)&&(mousePos.x<=220)&&(mousePos.y>=60)&&(mousePos.y<=100)){
						//eng power --
						DisplayTri({200,80}, {220,60}, {220,100}, {150,50,50});
						if(engPowSel>10){
							engPowSel = engPowSel - 10;
						}
					}
					else if((mousePos.x>=200)&&(mousePos.x<=220)&&(mousePos.y>=160)&&(mousePos.y<=200)){
						//aero Hp --
						DisplayTri({200,180}, {220,160}, {220,200}, {150,50,50});
						if(aeroHpSel>5){
							aeroHpSel = aeroHpSel -5;
						}
					}
					else if((mousePos.x>=940)&&(mousePos.x<=960)&&(mousePos.y>=60)&&(mousePos.y<=100)){
						//eng power ++
						DisplayTri({960,80}, {940,60}, {940,100}, {150,50,50});
						if(engPowSel<30){
							engPowSel = engPowSel + 10;
						}
					}
					else if((mousePos.x>=940)&&(mousePos.x<=960)&&(mousePos.y>=160)&&(mousePos.y<=200)){
						//aero Hp ++
						DisplayTri({960,180}, {940,160}, {940,200}, {150,50,50});
						if(aeroHpSel<15){
							aeroHpSel = aeroHpSel +5;
						}
					}
					



					// ### GUN 1 ### //

					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=15)&&(mousePos.y<=45)){
						// gun 1 dmg --
						DisplayTri({1205,30}, {1220,15},{1220,45}, {175,50,50});	
						if(gun1dmgSel>1){
							gun1dmgSel = gun1dmgSel-1;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=65)&&(mousePos.y<=95)){
						// gun 1 velocity --
						DisplayTri({1205,80}, {1220,65},{1220,95}, {175,50,50});	
						if(gun1VelSel>10){
							gun1VelSel = gun1VelSel -10;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=115)&&(mousePos.y<=145)){
						// gun 1 ammo -- 
						DisplayTri({1205,130}, {1220,115},{1220,145}, {175,50,50});	
						if(gun1AmmoSel>150){
							gun1AmmoSel = gun1AmmoSel - 150;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=15)&&(mousePos.y<=45)){
						// gun 1 dmg ++
						DisplayTri({1890,30}, {1875,15},{1875,45}, {175,50,50});	
						if(gun1dmgSel<3){
							gun1dmgSel = gun1dmgSel +1;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=65)&&(mousePos.y<=95)){
						// gun 1 velocity ++
						DisplayTri({1890,80}, {1875,65},{1875,95}, {175,50,50});	
						if(gun1VelSel<30){
							gun1VelSel = gun1VelSel + 10;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=115)&&(mousePos.y<=145)){
						// gun 1 ammo ++ 
						DisplayTri({1890,130}, {1875,115},{1875,145}, {175,50,50});	
						if(gun1AmmoSel<450){
							gun1AmmoSel = gun1AmmoSel + 150;
						}
					}	

					// ### GUN 1 ### //


					// ### GUN 2 ### //
					else if((mousePos.x>=1020)&&(mousePos.x<=1045)&&(mousePos.y>=240+95)&&(mousePos.y<=240+95+25)){
						gun2Bool = (gun2Bool)?false:true;
					}
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=15+240)&&(mousePos.y<=45+240)){
						// gun 2 dmg --
						DisplayTri({1205,30+240}, {1220,15+240},{1220,45+240}, {175,50,50});	
						if(gun2dmgSel>1){
							gun2dmgSel = gun2dmgSel-1;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=65+240)&&(mousePos.y<=95+240)){
						// gun 2 velocity --
						DisplayTri({1205,80+240}, {1220,65+240},{1220,95+240}, {175,50,50});	
						if(gun2VelSel>10){
							gun2VelSel = gun2VelSel -10;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=115+240)&&(mousePos.y<=145+240)){
						// gun 2 ammo -- 
						DisplayTri({1205,130+240}, {1220,115+240},{1220,145+240}, {175,50,50});	

						if(gun2AmmoSel>150){
							gun2AmmoSel = gun2AmmoSel - 150;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=15+240)&&(mousePos.y<=45+240)){
						// gun 2 dmg ++
						DisplayTri({1890,30+240}, {1875,15+240},{1875,45+240}, {175,50,50});	
						if(gun2dmgSel<3){
							gun2dmgSel = gun2dmgSel +1;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=65+240)&&(mousePos.y<=95+240)){
						// gun 2 velocity ++
						DisplayTri({1890,80+240}, {1875,65+240},{1875,95+240}, {175,50,50});	
						if(gun2VelSel<30){
							gun2VelSel = gun2VelSel + 10;
						}

					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=115+240)&&(mousePos.y<=145+240)){
						// gun 2 ammo ++ 
						DisplayTri({1890,130+240}, {1875,115+240},{1875,145+240}, {175,50,50});	
						if(gun2AmmoSel<450){
							gun2AmmoSel = gun2AmmoSel + 150;
						}
					}	

					// ### GUN 2 ### //


					// ### GUN 3 ### //

					else if((mousePos.x>=1020)&&(mousePos.x<=1045)&&(mousePos.y>=480+95)&&(mousePos.y<=480+95+25)){
						gun3Bool = (gun3Bool)?false:true;
					}
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=15+480)&&(mousePos.y<=45+480)){
						// gun 3 dmg --
						DisplayTri({1205,30+480}, {1220,15+480},{1220,45+480}, {175,50,50});	
						if(gun3dmgSel>1){
							gun3dmgSel = gun3dmgSel-1;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=65+480)&&(mousePos.y<=95+480)){
						// gun 3 velocity --
						DisplayTri({1205,80+480}, {1220,65+480},{1220,95+480}, {175,50,50});	
						if(gun3VelSel>10){
							gun3VelSel = gun3VelSel -10;
						}
					}	
					else if((mousePos.x>=1205)&&(mousePos.x<=1220)&&(mousePos.y>=115+480)&&(mousePos.y<=145+480)){
						// gun 3 ammo -- 
						DisplayTri({1205,130+480}, {1220,115+480},{1220,145+480}, {175,50,50});	
						if(gun3AmmoSel>150){
							gun3AmmoSel = gun3AmmoSel - 150;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=15+480)&&(mousePos.y<=45+480)){
						// gun 3 dmg ++
						DisplayTri({1890,30+480}, {1875,15+480},{1875,45+480}, {175,50,50});	

						if(gun3dmgSel<3){
							gun3dmgSel = gun3dmgSel +1;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=65+480)&&(mousePos.y<=95+480)){
						// gun 3 velocity ++
						DisplayTri({1890,80+480}, {1875,65+480},{1875,95+480}, {175,50,50});	
						if(gun3VelSel<30){
							gun3VelSel = gun3VelSel + 10;
						}
					}	
					else if((mousePos.x>=1875)&&(mousePos.x<=1890)&&(mousePos.y>=115+480)&&(mousePos.y<=145+480)){
						// gun 3 ammo ++ 
						DisplayTri({1890,130+480}, {1875,115+480},{1875,145+480}, {175,50,50});	
						if(gun3AmmoSel<450){
							gun3AmmoSel = gun3AmmoSel + 150;
						}
					}	

					// ### GUN 3 ### //

					// ### graph selection ### //

					// turn rate 
					else if((mousePos.x>=1020)&&(mousePos.x<=1040)&&(mousePos.y>=820)&&(mousePos.y<=860)){
						turnSel = (turnSel)?turnSel-1:turnSel;
					}
					else if((mousePos.x>=1800)&&(mousePos.x<=1820)&&(mousePos.y>=820)&&(mousePos.y<=860)){
						turnSel = (turnSel<4)?turnSel+1:turnSel;
					}

					// eng Sp
					else if ((mousePos.x>=70)&&(mousePos.x<=90)&&(mousePos.y>=820)&&(mousePos.y<=860)){
						engSpSel = (engSpSel)?engSpSel-1:engSpSel;
					}
					else if ((mousePos.x>=850)&&(mousePos.x<=870)&&(mousePos.y>=820)&&(mousePos.y<=860)){
						engSpSel = (engSpSel<6)?engSpSel+1:engSpSel;
					}
					// eng Alt
					else if ((mousePos.x>=70)&&(mousePos.x<=90)&&(mousePos.y>=460)&&(mousePos.y<=500)){
						engAltSel = (engAltSel)?engAltSel-1:engAltSel;
					}
					else if ((mousePos.x>=850)&&(mousePos.x<=870)&&(mousePos.y>=460)&&(mousePos.y<=500)){
						engAltSel = (engAltSel<6)?engAltSel+1:engAltSel;
					}



					// ### 				   ### //
				}
				// ### graphs ### //

				// TURN speed x and y lim
				// x : 1080 to 1780 (700)
				// y : 1000 to 800  (200) y=(0-8)*25  x=(0-15)*45
				drawRect(gRenderer, 1070, 760, 700, 240,150,50,50,false);
				DisplayTri({1020,840}, {1040,820}, {1040,860}, {50,150,50});
				DisplayTri({1820,840}, {1800,820}, {1800,860}, {50,150,50});
				for(float i=0;i<=15;i=i+1/45.0){
					float y = 25*calculateR(i, turnSpeed[turnSel]);
					drawRect(gRenderer,1080+i*45,1000-y,1,1,50,150,50);
				}

				//ENG SP lims
				// x : (130-810)(680) (5-15)  x*45.33
				// y : (750-980)(230) (0-5.5) y*41.82
				drawRect(gRenderer, 120, 760, 700,240,150,50,50,false);
				DisplayTri({70,840}, {90,820}, {90,860}, {50,150,50});
				DisplayTri({870,840}, {850,820}, {850,860}, {50,150,50});
				for(float i=5;i<=15;i=i+1/68.0){
					float y = 41.82*calculateR(i,engSp[engSpSel]);
					drawRect(gRenderer,130+(i-5)*68,1010-y,1,1,50,175,50);
				}

				drawRect(gRenderer, 120, 400, 700,240,150,50,50,false);
				DisplayTri({70,480}, {90,460}, {90,500}, {50,150,50});
				DisplayTri({870,480}, {850,460}, {850,500}, {50,150,50});
				for(float i=0.5;i<=10.5;i=i+1/68.0){
					float y = 25*calculateR(i,engAlt[engAltSel]);
					drawRect(gRenderer,130+(i-0.5)*68.0,650-y,1,1,50,175,50);
				}
				// ### 		  ### //
				gun1RateSel = gun1dmgSel*5 + gun1VelSel*0.8333;
				gun2RateSel = gun2dmgSel*5 + gun2VelSel*0.8333;
				gun3RateSel = gun3dmgSel*5 + gun3VelSel*0.8333;

				gun1Wg = ((gun1dmgSel*10+gun1VelSel)/2 + gun1AmmoSel/15)/4;
				gun2Wg = (gun2Bool)?((gun2dmgSel*10+gun2VelSel)/2 + gun2AmmoSel/15)/4:0;
				gun3Wg = (gun3Bool)?((gun3dmgSel*10+gun3VelSel)/2 + gun3AmmoSel/15)/4:0;
				aeroWgSel=aeroHpSel+(gun1Wg+gun2Wg+gun3Wg)/3;
				//cout<<aeroWgSel<<endl;
				//}

				//end of buttons

				// options of customization
				if(keys[6]){
					clickUp = false;
				}
			


				SDL_RenderPresent(gRenderer);

				frameTime = SDL_GetTicks()-gameRunTime;
				if(frameTime<minTimeForFrame){
					SDL_Delay(minTimeForFrame-frameTime);
				}
			}

			seed = 12;
			srand(seed);
			e1 = engine(engPowSel,engAlt[0],engSp[engSpSel]);
			a1 = aeroframe(aeroHpSel,aeroWgSel,1,gravAlt,gravWg,dragAlt,dragSp,dragRdiff,turnSpeed[turnSel]);	
			g1.clear();
			g1.push_back(gun(gun1AmmoSel,gun1dmgSel,gun1VelSel,gun1RateSel));
			if(gun2Bool){
				g1.push_back(gun(gun2AmmoSel,gun2dmgSel,gun2VelSel,gun2RateSel));
			}
			if(gun3Bool){
				g1.push_back(gun(gun3AmmoSel,gun3dmgSel,gun3VelSel,gun3RateSel));
			}
			player = plane(SCREEN_WIDTH/2 +200,2200,a1,e1,g1);	
			/*
			here is where u set player and enemy plane char 
			here is where u have to read from a file and select a specific value
			 */
			break;
		case 1:    	// restarting match with preset parameters
			player.rotation = 0;
			player.eng.enginePerc = 0.00001;
			player.curSpeed = 15;
			player.px = 1000;
			player.py = 10383;
			counter =0;
			//clear bullet and add ammo to all 
			//### event  ###//
			//keys = {false,false,false,false,false,false};//{w,s,a,d,quit,esc}
			//mousePos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};

			//### texture placement ###//
			//float angle = 60.0f;
			/*	
			
			
			*///### player plane definition ###//
			//e1.printAll();
			//player.eng.printAll();
			while(1){
				if(counter>=240){
					break;
				}
				counter=counter+1;
				gameRunTime = SDL_GetTicks();

				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer,rSky,&srcRect,NULL);
				string num = "3";
				num[0] = num[0] - counter/80;
				DisplayText(num,{940,460,80,160},{50,175,50});

				SDL_RenderPresent(gRenderer);
				frameTime = SDL_GetTicks()-gameRunTime;
				if(frameTime<minTimeForFrame){
					SDL_Delay(minTimeForFrame-frameTime);
				}
			}
			state = 2;
			break;
		case 2:		// resume without reseting 
			keys = {false,false,false,false,false,false,false,false};//{w,s,a,d,quit,esc,leftclick,rightclick}
			mousePos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
			counter =0;
			while(!keys[4]){
				gameRunTime = SDL_GetTicks();

				//  ####  input handle  ####  //
				getInput(keys,mousePos);
				/*
				while(!keys[0]){
					getInput(keys,mousePos);
				}
				*/
				//  ####  end input handle  ####  //
				
				
				if(keys[5]){
					state = 3;
					keys[4] = true;	
				}

				//### update planes ###//
				globalBullets::bulletsUpdate();
				player.updatePlane(getAngle(mousePos.x,mousePos.y),keys);


				
				//### update renderer ###//
				SDL_RenderClear(gRenderer);
				srcRect.x = player.px - SCREEN_WIDTH/2  ;
				srcRect.y = player.py - SCREEN_HEIGHT/2 ;
				SDL_RenderCopy(gRenderer,rSky,&srcRect,NULL); //copy skybox to renderer
				displayPlane(player, player.px, player.py);
				displayAllBulletes(player.px,player.py);	
				//drawRect(gRenderer,,, 2, 2,255,0,0);

				//drawRect(gRenderer,, int y, int w, int h)
				
				
				drawRect(gRenderer, 958, 538, 2, 2,255,0,0);
				//	HUD updates	
				rectForText = {10,1000,50,20};
				colorForText = {0,255,0};
				DisplayText("IAS ",rectForText,colorForText);
				rectForText ={70,1000,55,20};
				DisplayNum(player.curSpeed*33.33,rectForText, colorForText,3);

				rectForText = {10,970,50,20};
				DisplayText("Alt",rectForText,colorForText);
				rectForText = {70,970,110,20};
				DisplayNum(10383-player.py, rectForText, colorForText,6);

				rectForText = {10,940,50,20};
				DisplayText("Eng",rectForText,colorForText);
				rectForText ={70,940,55,20};
				DisplayNum(player.eng.enginePerc*100, rectForText, colorForText,3);

				rectForText = {10,910,60,20};
				DisplayText("Ammo",rectForText,colorForText);
				rectForText = {80,910,75,20};
				DisplayNum(player.ammo, rectForText, colorForText,4);

				rectForText ={850,200,150,25};
				if(counter<100&&counter>0&&((player.curSpeed<1.5&&player.py<10100)||(player.curSpeed>1.5&&player.py>10100))){
					DisplayText("!!CAUTION!!", rectForText, colorForText);
				}
				SDL_RenderPresent(gRenderer);
				//	end of HUD updates

				// #### this is fps handler #### //
				counter = (counter>=fps*2)?0:counter +1;
				frameTime = SDL_GetTicks()-gameRunTime;
				if(frameTime<minTimeForFrame){
					SDL_Delay(minTimeForFrame-frameTime);
				}

				// #### 		    #### // 
			}
			//state = 0;// main menu
			break;
		case 3:		// escape menu
			int hoverMouse =0;
			keys = {false,false,false,false,false,false,false,false};//{w,s,a,d,quit,esc,leftclick,rightclick}
			//SDL_Surface *rectangle;
			//rectangle = SDL_CreateRGBSurface(0,200,200,32,0,0,0,0);
			SDL_Rect pos = {(SCREEN_WIDTH/2)-240,(SCREEN_HEIGHT/2)-250,480,500};
			//SDL_SetRenderDrawColor(gRenderer,255,255,255,10);
			//SDL_RenderDrawRect(gRenderer,&pos);
			//SDL_RenderFillRect(gRenderer,&pos);
			mousePos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
			int topLeftMenuX =(SCREEN_WIDTH/2)-140;
			int topLeftMenuY = (SCREEN_HEIGHT/2)-250;
			while(!keys[4]){
				gameRunTime = SDL_GetTicks();
				//  ####  input handle  ####  //
				getInput(keys,mousePos);
				
				//if quit keys[4] = true;state = 0;
				//if start keys[4] = true;state = 1;
				//if(keys[6]){
				//	state = 0;
				//	keys[4] = true;
				//}
				
				
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer,rSky,&srcRect,NULL);
				//SDL_SetRenderDrawColor(gRenderer,51,255,153,255);
				//SDL_RenderDrawRect(gRenderer,&pos);
				//SDL_RenderFillRect(gRenderer,&pos);
				if(mousePos.x>topLeftMenuX+15 && mousePos.x<topLeftMenuX+265){
					if(mousePos.y>topLeftMenuY+75 && mousePos.y<topLeftMenuY+125){
						hoverMouse = 1;
					}
					else if(mousePos.y>topLeftMenuY+175 && mousePos.y<topLeftMenuY+225){
						hoverMouse = 2;
					}
					else if(mousePos.y>topLeftMenuY+275 && mousePos.y<topLeftMenuY+325){
						hoverMouse = 3;
					}
					else if(mousePos.y>topLeftMenuY+375 && mousePos.y<topLeftMenuY+425){
						hoverMouse =4;
					}
					else{
						hoverMouse =0;
					}
				
				
				}
				else{
					hoverMouse =0;
				}
				if(keys[5]){
					state = 2;
					keys[4] = true;
				
				}
				else if(keys[6] && hoverMouse){
					keys[4] = true;
					switch (hoverMouse){
						case 1:
							state = 2;
							break;
						case 2:
							state = 1;
							break;
						case 3:
							state = 4;
							break;
						case 4:
							state =0;
							break;
					
					}
				}
				drawRect(gRenderer,topLeftMenuX,topLeftMenuY,280,500,51,255,153);
				switch (hoverMouse){
					case 0:
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+75,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+175,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+275,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+375,250,50,83,12,255);
						break;
					case 1:
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+75,250,50,225,12,62);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+175,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+275,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+375,250,50,83,12,255);
						break;
					case 2:
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+75,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+175,250,50,225,12,62);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+275,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+375,250,50,83,12,255);
						break;
					case 3:
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+75,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+175,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+275,250,50,225,12,62);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+375,250,50,83,12,255);
						break;
					case 4:
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+75,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+175,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+275,250,50,83,12,255);
						drawRect(gRenderer,topLeftMenuX+15,topLeftMenuY+375,250,50,225,12,62);
						break;
				}
				
				rectForText = {topLeftMenuX+65,topLeftMenuY+80,150,40};
				colorForText = {0,255,0};
				DisplayText("Resume",rectForText,colorForText);
					
				rectForText = {topLeftMenuX+55,topLeftMenuY+180,160,40};
				colorForText = {0,255,0};
				DisplayText("Restart",rectForText,colorForText);

				rectForText = {topLeftMenuX+90,topLeftMenuY+280,100,40};
				colorForText = {0,255,0};
				DisplayText("Menu",rectForText,colorForText);

				rectForText = {topLeftMenuX+90,topLeftMenuY+380,90,40};
				colorForText = {0,255,0};
				DisplayText("Exit",rectForText,colorForText);

				SDL_RenderPresent(gRenderer);
				frameTime = SDL_GetTicks()-gameRunTime;
				if(frameTime<minTimeForFrame){
					SDL_Delay(minTimeForFrame-frameTime);
				}

				// #### 		    #### // 
			}
			break;
		}
	}

	close();

	return 0;
}
#include "aiController.hpp"
#include "planeClass.hpp"
#include <math.h>
#include <vector>
//#include <iostream>
#include <cmath>


    void targetPlane(plane *sPlane, plane *dPlane){
        //temp for now to check if bullets work
        // add defensive dodge and reversal (only when the player is behind obviously), offensive tracking (easy)
        // keys = {w,s,a,d,quit,esc,leftclick,rightclick};

        vector<bool> keys = {false,false,false,false,false,false,false,false};
        double r=5.0;
        int diffx;
        int diffy;       
        switch (sPlane->state){
            case 0:
                keys[0] = true;
                if(sPlane->px>=14000){
                    r = + M_PI - 0.0001;  
                }
                if(sPlane->px<=2480){
                    r = -0.0;
                }
                sPlane->state = 1;
                
                break;
            case 1: // chase
                // aim 20 pixels behind the plane
                diffx = (dPlane->px+20*(cos(dPlane->rotation))) - sPlane->px;
                diffy = (dPlane->py+20*(sin(dPlane->rotation))) - sPlane->py;
                r = atan2(((double)diffy),(double) diffx);
                //(dPlane->px+20*(cos(dPlane->rotation)));
                //(dPlane->py+20*(sin(dPlane->rotation)));
            
                if(sPlane->curSpeed<5){
                    sPlane->state = 2;
                } 
                break;
            case 2: // recovery
                
                
                break;
        }
        sPlane->updatePlane(r, keys);    
    }

    /*
    state 1 = chase px py as long as u have speed and altitude
        if(no speed && no plane separation && no altitude)->go to state 2
        if(no speed && no plane separation && altitude) ->go to state 3
        //defensive recovery states

        if(no speed && plane separation && no altiutude) ->go to state 4
        if(no speed && plane separation && altitude) -> got to state 5
        // these are recovery states

        if(speed && no plane separation && no altitude) -> goto state 6
        if(speed && no plane separation && altitude) ->go to state 7
        if(speed && plane sepration && no altitude) ->go to state 8
        if(speed && plane separation && altitude) -> go to state 9
    
    */
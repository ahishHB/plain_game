#include "aiController.hpp"
#include "planeClass.hpp"
#include <cmath>

void targetPlane(plane *sPlane, plane *dPlane){
    //temp for now to check if bullets work
    // keys = {w,s,a,d,quit,esc,leftclick,rightclick};
    vector<bool> keys = {false,false,false,false,false,false,false,false};
    double r=0.1;
    if(sPlane->px>15400){
        r = - M_PI;
    }
    if(sPlane->px<980){
        r = -0.0;
    }
    sPlane->updatePlane(r, keys);    
}
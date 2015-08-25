#ifndef ISOMETRICPLANE_H
#define ISOMETRICPLANE_H

#include "ofMain.h"
#include "eyeMesh.h"
#include "ofxGLWarper.h"

class isoMetricPlane
{
public:
    isoMetricPlane();
    void setup();
    void update();
    void draw();
    void makeOrtho();
    void makeIsoToScreen();
    void makeCorner();
    ofPoint getWorldToScreenPos(const ofVec4f &_srcpos);
    ofPoint getWorldToScreenPos(const ofPoint &_srcpos);
private:
    ofxGLWarper warp;
    eyeMesh mesh;
    ofMatrix4x4 matprojected;
    ofMatrix4x4 matproj;
    ofMatrix4x4 matprojortho;
    ofMatrix4x4 matmv;
    ofMatrix4x4 matworldtoiso;
    ofVec4f mousepos;
    float isoGridWidth;
    float scaleProportion;

    ofVec4f corner[4];
};

#endif // ISOMETRICPLANE_H

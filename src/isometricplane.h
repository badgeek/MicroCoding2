#ifndef ISOMETRICPLANE_H
#define ISOMETRICPLANE_H

#include "ofMain.h"
#include "eyeMesh.h"
#include "ofxGLWarper.h"

class isoMetricPlane
{
public:
    isoMetricPlane();
    ~isoMetricPlane();
    void setup();
    void update();
    void draw();

    void updateOrtho();
    void updateIsoToScreen();
    void updateCorner();
    void updateScreenToSourceCoord();

    ofPoint getIsoToScreenCoord(const ofVec4f &_srcpos);
    ofPoint getIsoToScreenCoord(const ofPoint &_srcpos);
    ofPoint getScreenToSourceCoord(const ofPoint &_srcpos);

    void enableMouseEvents();
    void disableMouseEvents();
    void _mouseMoved(ofMouseEventArgs &e);
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);


private:
    ofxGLWarper warp;
    eyeMesh mesh;
    ofMatrix4x4 matprojected;
    ofMatrix4x4 matproj;
    ofMatrix4x4 matprojortho;
    ofMatrix4x4 matmv;
    ofMatrix4x4 matworldtoiso;

    ofVec4f mousepos;
    ofVec4f mousepos_src;

    float isoGridWidth;
    float scaleProportion;

    ofVec4f corner[4];
};

#endif // ISOMETRICPLANE_H

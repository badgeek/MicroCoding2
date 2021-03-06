#ifndef ISOMETRICPLANE_H
#define ISOMETRICPLANE_H

#include "ofMain.h"
#include "eyeMesh.h"
#include "ofxGLWarper.h"
#include "ofxSyphon.h"

#include "microinspector.h"
#include "microinspectormanager.h"

class isoMetricPlane
{
public:

    isoMetricPlane();
    ~isoMetricPlane();

    void setup(ofVideoGrabber * _video, ofBaseApp *_app);
    void update();
    void draw();

    void updateOrtho();
    void updateIsoToScreen();
    void updateCorner();
    void updateScreenToSourceCoord();

    ofPoint getIsoToWindowCoord(const ofVec4f &_srcpos);
    ofPoint getIsoToWindowCoord(const ofPoint &_srcpos);
    ofPoint getScreenToSourceCoord(const ofPoint &_srcpos);

    void enableMouseEvents();
    void disableMouseEvents();

    void enableKeyboardEvents();
    void disableKeyboardEvents();


    void _mouseMoved(ofMouseEventArgs &e);
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    void _keyPressed(ofKeyEventArgs &e);
    void _keyReleased(ofKeyEventArgs &e);

    void audioIn(float * input, int bufferSize, int nChannels);


    void drawDepthMap();
    void drawIsoMetric();
    void drawDeptMapCamera();

    float zDepth;

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

    ofRectangle isoPlaneRect;

    float _isoPlaneWidth;
    float _isoPlaneHeight;
    float _isoPlaneCenterX;
    float _isoPlaneCenterY;


    float isoGridWidth;
    float scaleProportion;

    ofVec4f corner[4];

    microInspectorManager microManager;

    ofEasyCam camera;

    //webcam
    ofVideoGrabber * webcam;
    ofImage imgSourceVideo;
    ofImage imgCroppedVideo;

    //effects
    ofShader shader;
    ofFbo buffFbo;

    int scene;

    ofxSyphonClient mClient;



};

#endif // ISOMETRICPLANE_H

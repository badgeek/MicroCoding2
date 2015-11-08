#pragma once

#include "ofMain.h"
//#include "ofxOsc.h"
#include "isometricplane.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

#include "ofxtiming.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void audioIn(float * input, int bufferSize, int nChannels);

        ofxPanel gui;

private:
        ofVideoGrabber vidGrabber;
        ofVideoGrabber * wew;
        ofTexture ps3;
        isoMetricPlane isoplane;


        ofSoundStream soundStream;

        int 	bufferCounter;
        int 	drawCounter;

        float smoothedVol;
        float scaledVol;

        vector <float> left;
        vector <float> right;
        vector <float> volHistory;

};

#pragma once

#include "ofMain.h"
//#include "ofxOsc.h"
#include "isometricplane.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

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

        ofxPanel gui;

private:
        ofVideoGrabber vidGrabber;
        ofVideoGrabber * wew;
        ofTexture ps3;
        isoMetricPlane isoplane;
    ofxSyphonClient mClient;
};

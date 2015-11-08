#include "ofApp.h"

void ofApp::setup()
{
    ofSetFrameRate(30);
    ofSetVerticalSync(false);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }

    // vidGrabber.setGrabber(ofPtr<ofxPS3EyeGrabber>(new ofxPS3EyeGrabber()));
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(640, 480);
    vidGrabber.setDesiredFrameRate(30);


    isoplane.setup( &vidGrabber, this);

    ofHideCursor();
    

    int bufferSize = 256;
    soundStream.listDevices();


    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);

    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;

    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update()
{

    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    isoplane.zDepth = scaledVol * 400;

//    ofLog() << scaledVol * 200;

    //lets record the volume into an array
    volHistory.push_back( scaledVol );

    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

   vidGrabber.update();
    isoplane.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    isoplane.draw();

    // camera.begin();

}
//

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    isoplane.updateOrtho();
    isoplane.updateIsoToScreen();
    isoplane.updateCorner();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}


void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    float curVol = 0.0;

    // samples are "interleaved"
    int numCounted = 0;

    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;

        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }

    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;

    // this is how we get the root of rms :)
    curVol = sqrt( curVol );

    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;

    bufferCounter++;

}

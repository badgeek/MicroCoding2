#include "ofApp.h"

void ofApp::setup()
{
    ofSetFrameRate(30);
    ofSetVerticalSync(false);
//    ofEnableBlendMode(OF_BLENDMODE_ADD);


    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++)
        {
            cout << devices[i].id << ": " << devices[i].deviceName;
            if( devices[i].bAvailable )
                {
                    cout << endl;
                }
            else
                {
                    cout << " - unavailable " << endl;
                }
        }
    // vidGrabber.setGrabber(ofPtr<ofxPS3EyeGrabber>(new ofxPS3EyeGrabber()));
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(640, 480);
    vidGrabber.setDesiredFrameRate(30);
    shader.load("shaders/fuxShader");


//    glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
//    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);


    isoplane.setup();



}

//--------------------------------------------------------------
void ofApp::update()
{
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
    isoplane.makeOrtho();
    isoplane.makeIsoToScreen();
    isoplane.makeCorner();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}

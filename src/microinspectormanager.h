#ifndef MICROINSPECTORMANAGER_H
#define MICROINSPECTORMANAGER_H

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxtiming.h"

#include "microinspector.h"

class microInspectorManager
{
public:
    microInspectorManager();
    ~microInspectorManager();

    void enableMouseEvents();
    void disableMouseEvents();

    void _mouseMoved(ofMouseEventArgs &e);
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);

    void setup(ofVec4f * _mousesrc);

    void draw();
    void drawLabel();
    void update();

    vector<microInspector> inspectors;

    ofVec4f *mouse_src;

    ofxTiming tmrOsc;
    ofxOscSender sender;
    void updateOsc();
};

#endif // MICROINSPECTORMANAGER_H

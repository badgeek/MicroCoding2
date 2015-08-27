#ifndef MICROINSPECTORMANAGER_H
#define MICROINSPECTORMANAGER_H

#include "ofMain.h"
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
    vector<microInspector> inspectors;

    ofVec4f *mouse_src;
};

#endif // MICROINSPECTORMANAGER_H

#include "microinspectormanager.h"

microInspectorManager::microInspectorManager()
{
    enableMouseEvents();
    tmrOsc.setInterval(0.1);
}

microInspectorManager::~microInspectorManager()
{
    disableMouseEvents();
}

void microInspectorManager::setup(ofVec4f * _mousesrc)
{
    mouse_src = _mousesrc;
}
//--------------------------------------------------------------

void microInspectorManager::draw()
{
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it).draw();
    }
}

void microInspectorManager::drawLabel()
{
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it).drawLabel();
    }
}

//--------------------------------------------------------------

void microInspectorManager::updateOsc()
{
    if (tmrOsc.tick())
    {
        microInspector * _inspector;
        for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
            _inspector = &(*it);

            ofLog() << "tick";

        }
    }
}

void microInspectorManager::update()
{
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it).update();
    }

//    updateOsc();


}

//--------------------------------------------------------------
void microInspectorManager::_mouseMoved(ofMouseEventArgs &e) {
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it)._mouseMoved(mouse_src->x, mouse_src->y, e.button);
    }
}


//--------------------------------------------------------------
void microInspectorManager::_mousePressed(ofMouseEventArgs &e) {

    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it)._mousePressed(mouse_src->x, mouse_src->y, e.button);
    }

}

//--------------------------------------------------------------
void microInspectorManager::_mouseDragged(ofMouseEventArgs &e) {
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it)._mouseDragged(mouse_src->x, mouse_src->y, e.button);
    }
}

//--------------------------------------------------------------
void microInspectorManager::_mouseReleased(ofMouseEventArgs &e) {
    for(vector<microInspector>::iterator it = inspectors.begin(); it != inspectors.end(); it++){
        (*it)._mouseReleased(mouse_src->x, mouse_src->y, e.button);
    }
}

//--------------------------------------------------------------
void microInspectorManager::enableMouseEvents() {
    ofAddListener(ofEvents().mousePressed, this, &microInspectorManager::_mousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &microInspectorManager::_mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &microInspectorManager::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &microInspectorManager::_mouseReleased);
}

//--------------------------------------------------------------
void microInspectorManager::disableMouseEvents() {
    ofRemoveListener(ofEvents().mousePressed, this, &microInspectorManager::_mousePressed);
    ofRemoveListener(ofEvents().mouseMoved, this, &microInspectorManager::_mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &microInspectorManager::_mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &microInspectorManager::_mouseReleased);
}

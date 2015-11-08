#ifndef MICROINSPECTOR_H
#define MICROINSPECTOR_H

#include "ofMain.h"

class microInspector
{
public:
    microInspector();

    void draw();
    void drawLabel();

    void setPos(float _x, float _y);
    void setup(ofImage * _video, ofRectangle * _isoRect);
    void update();
    void grabFrame();
    void diffFrame();

    float getScreenPosX();
    float getScreenPosY();
    bool hitTest(float tx, float ty) const;
    bool isMouseOver() const;
    bool isMousePressed(int mouseButton) const;

    void _mouseMoved(float x, float y, int button);
    void _mousePressed(float x, float y, int button);
    void _mouseDragged(float x, float y, int button);
    void _mouseReleased(float x, float y, int button);

    virtual void onRollOver(float x, float y) ;                   // called when mouse enters object x, y, width, height
    virtual void onRollOut()          ;                        // called when mouse leaves object x, y, width, height
    virtual void onMouseMove(float x, float y)  ;                  // called when mouse moves while over object x, y, width, height
    virtual void onDragOver(float x, float y, int button) ;        // called when mouse moves while over object and button is down
    virtual void onDragOutside(float x, float y, int button) ;    // called when mouse moves while outside the object after being clicked on it
    virtual void onPress(float x, float y, int button)  ;        // called when mouse presses while over object
    virtual void onPressOutside(float x, float y, int button) ;   // called when mouse presses while outside object
    virtual void onRelease(float x, float y, int button)  ;        // called when mouse releases while over object
    virtual void onReleaseOutside(float x, float y, int button) ;  // called when mouse releases outside of object after being pressed on object

    float differenceVideo;
    float differenceVideo2;

    float posx; //source coord [oF normal coordinate system]
    float posy; //source coord [oF normal coordinate system]

    ofRectangle *rectIsoPlane;
    ofRectangle rectIsoCoord;
    ofRectangle rectSourceCoord;

private:


    unsigned char savedVideo[1440000];

    float width;
    float height;

    float prevposx;
    float prevposy;

    bool dragging;

    ofVideoGrabber * webcam;
    ofImage *sourceVideo;
    ofImage croppedVideo;

    float _isoPlaneCenterX;
    float _isoPlaneCenterY;

    //msainteract
    bool enabled;
    bool            _isMouseOver;       // is mouse over the rect
    map<int, bool>    _isMousePressed;       // is mouse down over the rect (for any given mouse button)
    unsigned long   _stateChangeTimestampMillis;
};

#endif // MICROINSPECTOR_H

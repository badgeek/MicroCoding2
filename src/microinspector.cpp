#include "microinspector.h"

microInspector::microInspector()
{
    width = 100;
    height = 100;
    _isMouseOver	= false;

    //setPos(0,0);
    _stateChangeTimestampMillis = 0;

    enabled = true;
    dragging = false;

    croppedVideo.allocate(width,height, OF_IMAGE_COLOR_ALPHA);

}
//--------------------------------------------------------------

void microInspector::draw()
{

    ofPushMatrix();
        ofPushStyle();
            if (differenceVideo > 2.0)
            {
                ofSetColor(255,0,0);
            }else{
                ofSetColor(0,255,0);
            }
            ofTranslate(0,0,10);
            ofRect(rectScreenCoord);
            //ofCircle(rectScreenCoord.getTopLeft().x, rectScreenCoord.getTopLeft().y, 10);
        ofPopStyle();
   ofPopMatrix();
  // ofDrawBitmapString("microInspector", rect.x, rect.y,10);
    ofRect(rectScreenCoord);

    ofPushMatrix();
        ofPushMatrix();
            ofTranslate(rectScreenCoord.getCenter().x, rectScreenCoord.getCenter().y,40);
            //croppedVideo.draw(0,0);
//            sourceVideo->draw(0,0);
    //   // ofCircle(0,0,10);
            ofPushMatrix();
                ofRotateX(-90);
                ofPushMatrix();
                ofRotateY(ofGetElapsedTimef()*40);
                ofDrawBox(40);
                ofPopMatrix();
        //        ofDrawCone(0, 0, 10,50);
            ofPopMatrix();
        ofPopMatrix();
    ofPopMatrix();

}
//--------------------------------------------------------------

void microInspector::setPos(float _x, float _y)
{
   posx = _x;
   posy = _y;

   _isoPlaneCenterX = rectIsoPlane->getCenter().x;
   _isoPlaneCenterY = rectIsoPlane->getCenter().y;

   float _tx = _x - _isoPlaneCenterX;
   float _ty = (_y  - _isoPlaneCenterY) * -1.0f;

   rectScreenCoord.setFromCenter(_tx, _ty, width, height);
   rectSourceCoord.setFromCenter(_x,_y,    width, height);
   //rect.setFromCenter(posx, posy, width, height);
   ofLog() << rectSourceCoord.getCenter().x;
   //ofLog() << " x: " << _isoPlaneCenterX << " y: " << _isoPlaneCenterY;
}
//--------------------------------------------------------------

void microInspector::setup(ofImage * _video, ofRectangle * _isoRect)
{
    sourceVideo = _video;
    rectIsoPlane = _isoRect;
    _isoPlaneCenterX = _isoRect->getCenter().x;
    _isoPlaneCenterY = _isoRect->getCenter().y;
}
//--------------------------------------------------------------

void microInspector::update()
{

    float posxnorm = ((posx/rectIsoPlane->getWidth()) * sourceVideo->getWidth()) - width/2;
    float posynorm = ((posy/rectIsoPlane->getHeight()) * sourceVideo->getHeight()) - height/2;

    float _posxclamp = ofClamp(posxnorm, 0, sourceVideo->getWidth() - width);
    float _posyclamp = ofClamp(posynorm, 0, sourceVideo->getHeight() - height);

//    ofLog() << "posxnorm " << posxnorm << " posynorm" << posynorm;

//    ofLog() << "_posxclamp " << _posxclamp << " _posyclamp" << _posyclamp;
    //    ofLog() << "posx " << posx << " posy" << posy;

//    if (posx < width-posx && posy < height-posy)
//    {
//        ofLog() << "Crop";
        croppedVideo.cropFrom(*sourceVideo,_posxclamp,_posyclamp,width,height);
        croppedVideo.setImageType(OF_IMAGE_GRAYSCALE);



//    }

        diffFrame();

        ofLog() << differenceVideo;
}

float microInspector::getScreenPosX()
{
    return posx - _isoPlaneCenterX;
}

float microInspector::getScreenPosY()
{
    return  (posx - _isoPlaneCenterY) * -1.0f;
}


void microInspector::grabFrame()
{

    int crop_width = width;
    int crop_height = height;

    int channel_crop = croppedVideo.getPixelsRef().getBytesPerPixel();
    unsigned char *pix = croppedVideo.getPixels();

    for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
    {
        savedVideo[x] = pix[x];
    }

}

void microInspector::diffFrame()
{
    float crop_width = width;
    float crop_height = height;

    float channel_crop = croppedVideo.getPixelsRef().getBytesPerPixel();

    unsigned char *pix = croppedVideo.getPixels();

    float diff;
    float diff2;

    for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
    {
        diff = diff + (abs(savedVideo[x] - pix[x]));
        diff2 = diff2 + ((savedVideo[x] - pix[x]));
    }

    differenceVideo = diff/(crop_width * crop_height * channel_crop);
    differenceVideo2 = diff2/(crop_width * crop_height * channel_crop);
}

//--------------------------------------------------------------

bool microInspector::hitTest(float tx, float ty) const {
    return ((tx > posx) && (tx < posx + width) && (ty > posy) && (ty < posy + height));
}

bool microInspector::isMouseOver() const {
    return _isMouseOver;
}

bool microInspector::isMousePressed(int mouseButton) const {
    if(_isMousePressed.find(mouseButton) == _isMousePressed.end()) return false;
    return _isMousePressed.at(mouseButton);
}

void microInspector::_mouseMoved(float x, float y, int button)
{
    if(rectSourceCoord.inside(x,y) )
    {
        if(!_isMouseOver) {						// if wasn't over previous frame
            _isMouseOver = true;						// update flag
            onRollOver(x, y);						// call onRollOver
        }
        onMouseMove(x, y);
    }else{
        onRollOut();							// call onRollOut
        _isMouseOver = false;
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------

void microInspector::_mousePressed(float x, float y, int button)
{
    if(rectSourceCoord.inside(x,y) )
    {
        if(!isMousePressed(button)) {						 // if wasn't down previous frame
            _isMousePressed[button] = true;						// update flag
            onPress(x, y, button);					// call onPress
        }
    }else{
        _isMousePressed[button] = false;	// update flag
          onPressOutside(x, y, button);
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------

void microInspector::_mouseDragged(float x, float y, int button)
{
    if(rectSourceCoord.inside(x,y) )
    {
        if(!_isMouseOver) {						// if wasn't over previous frame
            //				onPress(x, y);							// call onPress - maybe not
            _isMouseOver = true;						// update flag
            onRollOver(x, y);						// call onRollOver
        }
        onDragOver(x, y, button);
    }else{
        if(_isMouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
            onRollOut();							// call onRollOut
            _isMouseOver = false;						// update flag
        }
        if(isMousePressed(button)) {
            onDragOutside(x, y, button);
        }
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------

void microInspector::_mouseReleased(float x, float y, int button)
{
    if(rectSourceCoord.inside(x,y) )
    {
        onRelease(x, y, button);
    }else{
        if(isMousePressed(button)) onReleaseOutside(x, y, button);
    }

    _isMousePressed[button] = false;
    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();
    //mouseReleased(x, y, button);
}

//--------------------------------------------------------------

//--------------------------------------------------------------
// msa interactive object
//--------------------------------------------------------------

void microInspector::onRollOver(float x, float y) {

}

void microInspector::onRollOut() {

}

void microInspector::onMouseMove(float x, float y){

}

void microInspector::onDragOver(float x, float y, int button) {
    dragging = true;
    setPos( x - prevposx,  y - prevposy);
}

void microInspector::onDragOutside(float x, float y, int button) {
    if (dragging)
    {
        setPos( x - prevposx,  y - prevposy);
    }
}

void microInspector::onPress(float x, float y, int button) {
    ofLog() << "_mousePressed inside";
    prevposx = x - posx;
    prevposy = y - posy;
}

void microInspector::onPressOutside(float x, float y, int button)
{
    // called when mouse presses while outside object

}

void microInspector::onRelease(float x, float y, int button)
{
    ofLog() << "onRelease inside";

    if (dragging)
    {
        grabFrame();
        ofLog() << "Dropped";
    }

    dragging = false;

    // called when mouse releases while over object
}

void microInspector::onReleaseOutside(float x, float y, int button)
{  // called when mouse releases outside of object after being pressed on object

}

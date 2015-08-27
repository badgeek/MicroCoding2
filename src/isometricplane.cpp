#include "isometricplane.h"

//notes
// coord

// SCREEN = window/interaction getmouse
// SOURCE = source rectangle before transformed to isometric
// ISOCOORD = coord on isometric  -> getIsoToScreenCoord

// ISOCOORD to SCREEN -> getIsoToScreenCoord
// SCREEN to SOURCE -> getScreenToSourceCoord, mousepos_src
// SOURCE to ISOCOORD -> float _posxx = mousepos_src.x - 500; float _posyy = (mousepos_src.y - 500) * -1.0f;

isoMetricPlane::isoMetricPlane()
{

    scene = 0;
}
//--------------------------------------------------------------

isoMetricPlane::~isoMetricPlane() {
    disableMouseEvents();
    disableKeyboardEvents();
}
//--------------------------------------------------------------

void isoMetricPlane::setup(ofVideoGrabber *_video)
{
    mesh.setMeshGrid(150,150);
    mesh.setMeshSize(0.0066,0.0066);
    mesh.updateMeshTexCoord();
    mesh.updateMeshVerCoord();
    mesh.updateDisplayList();

    isoPlaneRect.set(0,0,1000,750);

    _isoPlaneWidth = isoPlaneRect.getWidth();
    _isoPlaneHeight = isoPlaneRect.getHeight();
    _isoPlaneCenterX = isoPlaneRect.getCenter().x;
    _isoPlaneCenterY = isoPlaneRect.getCenter().y;


    isoGridWidth = 500;

    corner[0].set(-_isoPlaneCenterX, -_isoPlaneCenterY,0,1);
    corner[1].set(-_isoPlaneCenterX, _isoPlaneCenterY,0,1);
    corner[2].set(_isoPlaneCenterX, _isoPlaneCenterY,0,1);
    corner[3].set(_isoPlaneCenterX, -_isoPlaneCenterY,0,1);

    warp.setup(_isoPlaneWidth,_isoPlaneHeight); //isometric area - fromScreenToWarpCoord

//    warp.toogleActive();
//    warp.deactivate();

    updateOrtho();
    updateIsoToScreen();
    updateCorner();

    enableMouseEvents();
    enableKeyboardEvents();

    webcam = _video;
    shader.load("shaders/fuxShader");

    imgCroppedVideo.allocate(100,100, OF_IMAGE_COLOR_ALPHA);

    microManager.setup(&mousepos_src);

//    buffFbo.allocate(1024, 768, GL_RGBA);

//    buffFbo.begin();
//    ofClear(0);
//    buffFbo.end();

}
//--------------------------------------------------------------

void isoMetricPlane::update()
{
    //mousepos.set(    (float)  ofGetMouseX() - (float) ofGetWidth()/2.0f ,(float)  ofGetMouseY() -(float)  ofGetHeight()/2.0f, 0, 1);
    updateScreenToSourceCoord();

    if (webcam->isFrameNew())
    {
        imgSourceVideo.setFromPixels(webcam->getPixelsRef());
        imgCroppedVideo.cropFrom(imgSourceVideo,100,100,100,100);
      //  imgCroppedVideo.setImageType(OF_IMAGE_GRAYSCALE);
    }

    microManager.update();

}
//--------------------------------------------------------------

void isoMetricPlane::drawDepthMap()
{
    shader.begin();
    shader.setUniform1f("eyeMultiply", 200.0f);
    shader.setUniformTexture("eyeTexDepth", webcam->getTextureReference(),0);
    ofPushMatrix(); //visual
        ofScale(1,1,1);
      //  ofRotateZ(180);

                        glPushMatrix();
                            glTranslatef(_isoPlaneCenterX,_isoPlaneCenterY,0);
                            glPushMatrix();
                                glScalef(_isoPlaneWidth,_isoPlaneHeight,-1);
                                glPushAttrib(GL_POLYGON_BIT);
                                glFrontFace(GL_CW);
                                glPointSize(2.5f);
                                mesh.drawDisplayList(GL_POINTS);
                                glPopAttrib();
                            glPopMatrix();
                            glPushMatrix();
                                glScalef(_isoPlaneWidth,_isoPlaneHeight,-1);
                                glPushAttrib(GL_POLYGON_BIT);
                                    glFrontFace(GL_CW);
                                    glPolygonMode(GL_FRONT, GL_LINE);
                                    glPolygonMode(GL_BACK, GL_LINE);
                                    mesh.drawDisplayList(GL_LINES);
                                glPopAttrib();
                            glPopMatrix();
                        glPopMatrix();

    ofPopMatrix(); //visual
    shader.end();
}

void isoMetricPlane::drawIsoMetric()
{
    glGetFloatv(GL_PROJECTION_MATRIX, matproj.getPtr());

    glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glLoadMatrixf(matprojortho.getPtr());
    glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); //1

            glLoadIdentity();
            glMultMatrixf(matmv.getPtr());
            glScalef(1,-1,1);
            ofRotateZ(-90);

            glPushMatrix(); //2

            //screen coord to isometric coord on screen
            float _posxx = mousepos_src.x - _isoPlaneCenterX;
            float _posyy = (mousepos_src.y - _isoPlaneCenterY) * -1.0f;



            ofLine(-2000, _posyy, 2000, _posyy);
            ofLine(_posxx, -2000, _posxx, 2000);
            ofCircle(_posxx,_posyy, 20);

                ofPushMatrix();
                    ofNoFill();
                    ofCircle(0,0,ofGetWidth());
                ofPopMatrix();

//                ofPushMatrix();
//                    glScalef(1,1,-1);
//                    ofTranslate(_posxx,_posyy);
//                    ofDrawAxis(100);
//                ofPopMatrix();

//                ofPushStyle();
//                    ofSetColor(0,255,0);
//                    ofCircle(corner[0], 5);
//                    ofCircle(corner[1], 10);
//                    ofCircle(corner[2], 15);
//                    ofCircle(corner[3], 20);
//                ofPopStyle();

//                ofDrawGrid(isoGridWidth,5.0f,false,false,false,true);

                ofPushStyle();
                ofColor c;
                c.set(50);
                ofSetColor(c);
                ofPushMatrix();
                ofRotate(90, 0, 1, 0);
                ofDrawGridPlane(isoGridWidth, 5.0f, false);
                ofPopMatrix();
                ofPopStyle();

                glPushMatrix();
                    glTranslatef(1*mousepos.x, 1*mousepos.y,0);
                    ofPushStyle();
                    ofSetColor(255,0,0);
                    ofCircle(0,0,5);
                    ofPopStyle();
                glPopMatrix();

                //gpufux
                drawDepthMap();

                ofPushMatrix();
                    ofScale(1,1,-1);
                    microManager.draw();
                    //imgSourceVideo.draw(-500,-375, 1000, 750);
                ofPopMatrix();


            glPopMatrix(); //2
        glPopMatrix(); //1


        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matproj.getPtr()); //restore original projection
        glMatrixMode(GL_MODELVIEW);


//            ofPoint test1 = getIsoToScreenCoord(corner[0]);
//            ofPoint test2 = getIsoToScreenCoord(corner[1]);
//            ofPoint test3 = getIsoToScreenCoord(corner[2]);
//            ofPoint test4 = getIsoToScreenCoord(corner[3]);

//            ofCircle(test1.x, test1.y, 10);
//            ofCircle(test2.x, test2.y, 20);
//            ofCircle(test3.x, test3.y, 30);
//            ofCircle(test4.x, test4.y, 40);


//            imgSourceVideo.draw(0,0);
//            imgCroppedVideo.draw(640,0);

         //   ofRect(ofGetWidth()/2  - 500 , ofGetHeight()/2 - 500, 1000,1000);

//            float _posx = mousepos_src.x - _isoPlaneCenterX;
//            float _posy = (mousepos_src.y - _isoPlaneCenterY) * -1;

          //  ofCircle(mousepos_src, 10);

            //ofCircle((float)ofGetWidth()/2.0f - 500.0f + mousepos_src.x, ((float)ofGetHeight()-1000)/2 +  mousepos_src.y ,30);

//            ofDrawBitmapString(ofToString(_posx) + " " + ofToString((_posy*-1)), ofGetMouseX(), ofGetMouseY(), 1);


//            ofDrawBitmapString(ofToString(_posx) + " " + ofToString(_posy), ofGetMouseX(), ofGetMouseY(), 1);
            ofDrawBitmapString(ofToString(mousepos_src.x) + " " + ofToString(mousepos_src.y), ofGetMouseX(), ofGetMouseY(), 1);
}

void isoMetricPlane::drawDeptMapCamera()
{


    camera.begin();
    drawDepthMap();
    camera.end();


//    buffFbo.begin();
//    ofClear(0);
//    drawIsoMetric();
//    buffFbo.end();

//    buffFbo.draw(0,0);

    ofPushStyle();
    ofNoFill();
    ofCircle(ofGetMouseX(), ofGetMouseY(), 10);
    ofPopStyle();

}

void isoMetricPlane::draw()
{
    switch (scene) {
    case 0:
        drawIsoMetric();
        break;
    case 1:
        drawDeptMapCamera();
        break;
    }

    if(scene == 1)
    {
        camera.enableMouseInput();
    }else{
        camera.disableMouseInput();
    }
            //ofCircle(mousepos_src.x, mousepos_src.y, 10);

            //   _inspector.setPos(_posxx, _posyy);
}
//--------------------------------------------------------------

void isoMetricPlane::updateOrtho()
{
    matprojortho.makeOrthoMatrix(-ofGetWidth()/2, ofGetWidth()/2,  ofGetHeight()/2, -ofGetHeight()/2 ,-1000, 1000);
}
//--------------------------------------------------------------

void isoMetricPlane::updateIsoToScreen()
{
    //multiplication sequencec is different with opengl / reversed
    matmv.makeIdentityMatrix();
    matmv.rotate(0.0f, 0.0f, 0.0f, 1.0f);
    matmv.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    matmv.rotate(-45.0f, 0.0f, 1.0f, 0.0f);
    matmv.rotate(35.264f, 1.0f, 0.0f, 0.0f);
}
//--------------------------------------------------------------

void isoMetricPlane::updateCorner()
{
    warp.setCorner(warp.BOTTOM_LEFT, getIsoToScreenCoord(corner[0]) );
    warp.setCorner(warp.TOP_LEFT, getIsoToScreenCoord(corner[1]) );
    warp.setCorner(warp.TOP_RIGHT, getIsoToScreenCoord(corner[2]) );
    warp.setCorner(warp.BOTTOM_RIGHT, getIsoToScreenCoord(corner[3]) );
}
//--------------------------------------------------------------

ofPoint isoMetricPlane::getIsoToScreenCoord(const ofVec4f &_srcpos)
{
    matprojected = matmv * matprojortho ;
    ofVec4f result 	 = _srcpos * matprojected ;
    ofPoint coord;
    coord.set( ((float)ofGetWidth()/2.0f) + result.x * ofGetWidth()/2.0f  , (ofGetHeight()/2.0f) + result.y * ofGetHeight()/2.0f, result.z);
    return coord;
}
//--------------------------------------------------------------


ofPoint isoMetricPlane::getIsoToScreenCoord(const ofPoint &_srcpos)
{
    ofVec4f pos;
    pos.set(_srcpos.x, _srcpos.y, _srcpos.z, 1);
    return getIsoToScreenCoord(pos);
}

//--------------------------------------------------------------

void isoMetricPlane::updateScreenToSourceCoord()
{
    //screenmouse coord to source coord
    mousepos_src = warp.fromScreenToWarpCoord(ofGetMouseX(), ofGetMouseY(), 0);
}

//--------------------------------------------------------------

ofPoint isoMetricPlane::getScreenToSourceCoord(const ofPoint &_srcpos)
{
    //screen coord to source coord
    ofVec4f res;
    res = warp.fromScreenToWarpCoord(_srcpos.x, _srcpos.y, 0);
    return res;
}


//--------------------------------------------------------------
void isoMetricPlane::enableMouseEvents() {
    ofAddListener(ofEvents().mousePressed, this, &isoMetricPlane::_mousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &isoMetricPlane::_mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &isoMetricPlane::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &isoMetricPlane::_mouseReleased);
}

//--------------------------------------------------------------
void isoMetricPlane::disableMouseEvents() {
    ofRemoveListener(ofEvents().mousePressed, this, &isoMetricPlane::_mousePressed);
    ofRemoveListener(ofEvents().mouseMoved, this, &isoMetricPlane::_mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &isoMetricPlane::_mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &isoMetricPlane::_mouseReleased);
}

void isoMetricPlane::enableKeyboardEvents()
{
    ofAddListener(ofEvents().keyPressed, this, &isoMetricPlane::_keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &isoMetricPlane::_keyReleased);
}

void isoMetricPlane::disableKeyboardEvents()
{
    ofRemoveListener(ofEvents().keyPressed, this, &isoMetricPlane::_keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &isoMetricPlane::_keyReleased);
}


//--------------------------------------------------------------
void isoMetricPlane::_mouseMoved(ofMouseEventArgs &e) {

}


//--------------------------------------------------------------
void isoMetricPlane::_mousePressed(ofMouseEventArgs &e) {

}

//--------------------------------------------------------------
void isoMetricPlane::_mouseDragged(ofMouseEventArgs &e) {

}

//--------------------------------------------------------------
void isoMetricPlane::_mouseReleased(ofMouseEventArgs &e) {

}


//--------------------------------------------------------------
void isoMetricPlane::_keyPressed(ofKeyEventArgs &e) {
    int key = e.key;
    microInspector _ins;

//    float _posxx = mousepos_src.x - _isoPlaneCenterX;
//    float _posyy = (mousepos_src.y - _isoPlaneCenterY) * -1.0f;
//    ofLog() << "posx " << ofToString(_posxx)  << "posy " << ofToString(_posyy);

    _ins.setup(&imgSourceVideo, &isoPlaneRect);
    _ins.setPos(mousepos_src.x, mousepos_src.y);

//    _ins.setPos(_posxx, _posyy);

    if (key == 'a') microManager.inspectors.push_back(_ins);
    if (key == 'z') scene = 0;
    if (key == 'x') scene = 1;

}

//--------------------------------------------------------------
void isoMetricPlane::_keyReleased(ofKeyEventArgs &e) {


}


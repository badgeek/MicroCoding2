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


}
//--------------------------------------------------------------

isoMetricPlane::~isoMetricPlane() {
    disableMouseEvents();
}
//--------------------------------------------------------------

void isoMetricPlane::setup()
{
    mesh.setMeshGrid(150,150);
    mesh.setMeshSize(0.0066,0.0066);
    mesh.updateMeshTexCoord();
    mesh.updateMeshVerCoord();
    mesh.updateDisplayList();

    isoGridWidth = 500;

    corner[0].set(-isoGridWidth, -isoGridWidth,0,1);
    corner[1].set(-isoGridWidth, isoGridWidth,0,1);
    corner[2].set(isoGridWidth, isoGridWidth,0,1);
    corner[3].set(isoGridWidth, -isoGridWidth,0,1);

    warp.setup(1000,1000); //isometric area - fromScreenToWarpCoord

    warp.toogleActive();

    updateOrtho();
    updateIsoToScreen();
    updateCorner();

    enableMouseEvents();
}
//--------------------------------------------------------------

void isoMetricPlane::update()
{
    //mousepos.set(    (float)  ofGetMouseX() - (float) ofGetWidth()/2.0f ,(float)  ofGetMouseY() -(float)  ofGetHeight()/2.0f, 0, 1);
    updateScreenToSourceCoord();
}
//--------------------------------------------------------------

void isoMetricPlane::draw()
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

            //screen coord to isometric coord
            float _posxx = mousepos_src.x - 500;
            float _posyy = (mousepos_src.y - 500) * -1.0f;

//            ofLine(-500, _posyy, 500, _posyy);
//                    ofLine(_posxx, -500, _posxx, 500);
//            ofCircle(_posxx,_posyy, 20);

                ofPushMatrix();
                    ofNoFill();
                    ofCircle(0,0,ofGetWidth());
                ofPopMatrix();

                ofPushMatrix();
                    glScalef(1,1,-1);
                    ofTranslate(_posxx,_posyy);
                    ofDrawAxis(100);
                ofPopMatrix();

                ofPushStyle();
                    ofSetColor(0,255,0);
                    ofCircle(corner[0], 5);
                    ofCircle(corner[1], 10);
                    ofCircle(corner[2], 15);
                    ofCircle(corner[3], 20);
                ofPopStyle();

                ofDrawGrid(isoGridWidth,5.0f,false,false,false,true);

                glPushMatrix();
                    glTranslatef(1*mousepos.x, 1*mousepos.y,0);
                    ofPushStyle();
                    ofSetColor(255,0,0);
                    ofCircle(0,0,5);
                    ofPopStyle();
                glPopMatrix();

            glPopMatrix(); //2
        glPopMatrix(); //1


        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(matproj.getPtr()); //restore original projection
        glMatrixMode(GL_MODELVIEW);


            ofPoint test1 = getIsoToScreenCoord(corner[0]);
            ofPoint test2 = getIsoToScreenCoord(corner[1]);
            ofPoint test3 = getIsoToScreenCoord(corner[2]);
            ofPoint test4 = getIsoToScreenCoord(corner[3]);

            ofCircle(test1.x, test1.y, 10);
            ofCircle(test2.x, test2.y, 20);
            ofCircle(test3.x, test3.y, 30);
            ofCircle(test4.x, test4.y, 40);


            ofRect(ofGetWidth()/2  - 500 , ofGetHeight()/2 - 500, 1000,1000);

            float _posx = mousepos_src.x;
            float _posy = mousepos_src.y;

            ofCircle((float)ofGetWidth()/2.0f - 500.0f + _posx, ((float)ofGetHeight()-1000)/2 +  _posy ,30);

            ofDrawBitmapString(ofToString(_posx-500) + " " + ofToString((_posy-500)*-1), ofGetMouseX(), ofGetMouseY(), 1);

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




#include "isometricplane.h"

isoMetricPlane::isoMetricPlane()
{


}

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

    warp.setup();
   // warp.toogleActive();


    makeOrtho();
    makeIsoToScreen();
    makeCorner();


}

void isoMetricPlane::update()
{
    mousepos.set(    (float)  ofGetMouseX() - (float) ofGetWidth()/2.0f ,(float)  ofGetMouseY() -(float)  ofGetHeight()/2.0f, 0, 1);
}

void isoMetricPlane::draw()
{

    glGetFloatv(GL_PROJECTION_MATRIX, matproj.getPtr());
//1024 768
    glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glLoadMatrixf(matprojortho.getPtr());
    glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); //1
            glLoadIdentity();

//            glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
//            glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
//            ofRotateX(90);
//            ofRotateZ(-90);

            glMultMatrixf(matmv.getPtr());
           // glScalef(1, scaleProportion, 1);
            glScalef(1,-1,1);
            ofRotateZ(-90);

            //ofLog() << scaleProportion;

            glPushMatrix(); //2

                ofPushMatrix();
                ofNoFill();
                ofCircle(0,0,ofGetWidth());
                ofPopMatrix();

                ofPushMatrix();
                glScalef(1,1,-1);
                ofDrawAxis(20);
                ofPopMatrix();

                ofPushStyle();
                ofSetColor(0,255,0);
                ofCircle(corner[0], 5);
                ofCircle(corner[1], 10);
                ofCircle(corner[2], 15);
                ofCircle(corner[3], 20);
                ofPopStyle();

                ofPushStyle();
                ofDrawGrid(isoGridWidth,5.0f,false,false,false,true);
                ofPopStyle();

                glPushMatrix();
                    glTranslatef(1*mousepos.x, 1*mousepos.y,0);
                    ofPushStyle();
                    ofSetColor(255,0,0);
                    ofCircle(0,0,5);
                    ofPopStyle();
                glPopMatrix();


//                ofDisableBlendMode();

//                float microwidth = 500;

//                ofEnableBlendMode(OF_BLENDMODE_ADD);


//                shader.begin();
//                shader.setUniform1f("eyeMultiply", 200.0f);
//                shader.setUniformTexture("eyeTexDepth", vidGrabber.getTextureReference(),0);
//                glPushMatrix();
//                    glTranslatef(ofGetWidth()/2 - microwidth/2.0f, ofGetHeight()/2 - ofGetHeight()/2.0f + microwidth/2,0);
//                    glPushMatrix();
//                        glScalef(microwidth,microwidth,-1);
//                        glPushAttrib(GL_POLYGON_BIT);
//                        glFrontFace(GL_CW);
//                        glPolygonMode(GL_FRONT, GL_LINE);
//                        glPolygonMode(GL_BACK, GL_LINE);
//                        mesh.drawDisplayList(GL_LINES);
//                        glPopAttrib();
//                    glPopMatrix();
//                glPopMatrix();
//                shader.end();

            glPopMatrix(); //2
        glPopMatrix(); //1


            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(matproj.getPtr());
            glMatrixMode(GL_MODELVIEW);



            ofPoint test1 = getWorldToScreenPos(corner[0]);
            ofPoint test2 = getWorldToScreenPos(corner[1]);
            ofPoint test3 = getWorldToScreenPos(corner[2]);
            ofPoint test4 = getWorldToScreenPos(corner[3]);

            ofCircle(test1.x, test1.y, 10);
            ofCircle(test2.x, test2.y, 20);
            ofCircle(test3.x, test3.y, 30);
            ofCircle(test4.x, test4.y, 40);


            ofRect(ofGetWidth()/2  - 500 , ofGetHeight()/2 - 500, 1000,1000);

//            warp.begin();
//            warp.draw();
//            warp.end();


            ofVec4f ww = warp.fromScreenToWarpCoord(ofGetMouseX(), ofGetMouseY(), 0);

            float _w = ofGetWidth();
            float _h = ofGetHeight();

            float _posx = ((ww.x/_w) * 1000);
            float _posy = ((ww.y/_h) * 1000.0f);

            ofCircle((float)ofGetWidth()/2.0f - 500.0f + _posx, ((float)ofGetHeight()-1000)/2 +  _posy ,30);
//            ofCircle( getWorldToScreenPos(ww),30);

            ofDrawBitmapString(ofToString(_posx) + " " + ofToString(_posy), ofGetMouseX(), ofGetMouseY(), 1);


//            glPushMatrix();

//                ofPushStyle();
//                ofSetColor(0,0,255);
//                ofCircle(world_iso.x  , world_iso.y ,20);
//                ofPopStyle();

//            glPopMatrix();


    //ofRect(0,0,100,100);
    //vidGrabber.draw(0,0);
    //ofDrawBox(3,1,3);
//camera.end();
//    glPushMatrix();
//            //unflip the camera
//            glScalef(20.0, 20.0, 1.0);
//        //    glScalef(4.0, -3.0, 1);

//    shader.begin();
//    shader.setUniform1f("eyeMultiply", 100.0f);
//    shader.setUniformTexture("eyeTexDepth", vidGrabber.getTextureReference(),0);

//    glPushMatrix();
//            glTranslatef(0, 0, -2);
//                glPushAttrib(GL_POLYGON_BIT);
//                    glFrontFace(GL_CW);
//                    glPointSize(2.5f);
//                    mesh.drawDisplayList(GL_POINTS);
//                glPopAttrib();
//        glPopMatrix();


////        glPushMatrix();
////            glTranslatef(0, 0, -10);
////                glPushAttrib(GL_POLYGON_BIT);
////                     glFrontFace(GL_CW);
////                     glPolygonMode(GL_FRONT, GL_LINE);
////                     glPolygonMode(GL_BACK, GL_LINE);
////                     mesh.drawDisplayList(GL_TRIANGLE_STRIP);
////                glPopAttrib();
////        glPopMatrix();


//    glPushAttrib(GL_POLYGON_BIT);
//             glFrontFace(GL_CW);
//             glPolygonMode(GL_FRONT, GL_LINE);
//             glPolygonMode(GL_BACK, GL_LINE);
//             mesh.drawDisplayList(GL_LINES);
//    glPopAttrib();

//    shader.end();

//    glPopMatrix();

            //camera.end();
}

void isoMetricPlane::makeOrtho()
{
    matprojortho.makeOrthoMatrix(-ofGetWidth()/2, ofGetWidth()/2,  ofGetHeight()/2, -ofGetHeight()/2 ,-1000, 1000);
}

void isoMetricPlane::makeIsoToScreen()
{

    //multiplication sequencec is different with opengl / reversed
    matmv.makeIdentityMatrix();

//    matmv.scale(1,-1,1);
    matmv.rotate(0.0f, 0.0f, 0.0f, 1.0f);
    matmv.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    matmv.rotate(-45.0f, 0.0f, 1.0f, 0.0f);
    matmv.rotate(35.264f, 1.0f, 0.0f, 0.0f);


    matworldtoiso.makeIdentityMatrix();
    matworldtoiso.rotate(35.264f, 1.0f, 0.0f, 0.0f);
    matworldtoiso.rotate(-45.0f, 0.0f, 1.0f, 0.0f);
    matworldtoiso.rotate(90.0f, 1.0f, 0.0f, 0.0f);


//       matworldtoiso.getInverseOf(matmv * matprojortho);
    ofLog() << matworldtoiso;
}

void isoMetricPlane::makeCorner()
{
    warp.setCorner(warp.BOTTOM_LEFT, getWorldToScreenPos(corner[0]) );
    warp.setCorner(warp.TOP_LEFT, getWorldToScreenPos(corner[1]) );
    warp.setCorner(warp.TOP_RIGHT, getWorldToScreenPos(corner[2]) );
    warp.setCorner(warp.BOTTOM_RIGHT, getWorldToScreenPos(corner[3]) );
}

ofPoint isoMetricPlane::getWorldToScreenPos(const ofVec4f &_srcpos)
{
    matprojected = matmv * matprojortho ;
    ofVec4f result 	 = _srcpos * matprojected ;
    ofPoint coord;
    coord.set( ((float)ofGetWidth()/2.0f) + result.x * ofGetWidth()/2.0f  , (ofGetHeight()/2.0f) + result.y * ofGetHeight()/2.0f, result.z);
    return coord;
}

ofPoint isoMetricPlane::getWorldToScreenPos(const ofPoint &_srcpos)
{
    ofVec4f pos;
    pos.set(_srcpos.x, _srcpos.y, _srcpos.z, 1);
    return getWorldToScreenPos(pos);
}

